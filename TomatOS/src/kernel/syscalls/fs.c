#include "fs.h"

#include "../drivers/disk.h"

#include "../heap.h"

#include "../syscalls/term.h"

#include <string.h>

#define TOMATO_FS_MAGIC 0x544f4d41544f4f53

#define NODE_SIZE(n) ((n).size + (n).name_size)
#define TOTAL_NODE_SIZE(n) ((n).size + (n).name_size + sizeof(raw_node_t))
#define REAL_SIZE(n) ((n).next_node_address - (n).address  + sizeof(raw_node_t))
#define TOTAL_REAL_SIZE(n) ((n).node.next_node_address - (n).address)
#define FOLDER_SIZE(folder) ((folder)->node.size / sizeof(folder_node_entry_t))

//////////////////////////////////////////////////////////////
//// file system node structures
//////////////////////////////////////////////////////////////

typedef struct {
	uint64_t magic;
	uint64_t root_folder_address;

	uint64_t last_node;
} header_t;

typedef enum {
	NODE_UNUSED,
	NODE_FILE,
	NODE_FOLDER,
} node_type_t;

typedef struct {
	node_type_t type;
	size_t size;
	size_t name_size;
	uint64_t parent_folder_address;

	uint64_t prev_node_address;
	uint64_t next_node_address;
} raw_node_t;

typedef struct {
	uint64_t file_address;
} folder_node_entry_t;

typedef struct {
	raw_node_t node;
	bool is_new;
	uint64_t address;
} node_t;

static header_t header;
static bool header_changed = false;
static uint64_t disk_size;

//////////////////////////////////////////////////////////////
//// node io
//////////////////////////////////////////////////////////////

static void update_header();

static void write_node_full(node_t* node, char* name, uintptr_t data);
static void write_node(node_t* node);
static void write_node_name(node_t* node, char* str);
static void write_node_data(node_t* node, uintptr_t data);

static void read_node(node_t* node);
static void read_node_data(node_t* node, uintptr_t buffer);
static void read_node_name(node_t* node, char* str);

static void read_node_data(node_t* node, uintptr_t buffer) {
	driver_disk_read(node->address + sizeof(raw_node_t) + node->node.name_size, buffer, node->node.size);
}

static void read_node_name(node_t* node, char* str) {
	driver_disk_read(node->address + sizeof(raw_node_t), str, node->node.name_size);
}

static void read_node(node_t* node) {
	driver_disk_read(node->address, &node->node, sizeof(raw_node_t));
	node->is_new = false;
}

static void write_node(node_t* node) {
	driver_disk_write(node->address, &node->node, sizeof(raw_node_t));
	node->is_new = false;
}

static void write_node_name(node_t* node, char* str) {
	driver_disk_write(node->address + sizeof(raw_node_t), str, node->node.name_size);
}

static void write_node_data(node_t* node, uintptr_t data) {
	driver_disk_write(node->address + sizeof(raw_node_t) + node->node.name_size, data, node->node.size);
}

static void write_node_full(node_t* node, char* name, uintptr_t data) {
	char* full_buffer = heap_allocate(node->node.size + node->node.name_size);
	
	// copy the name
	memcpy(full_buffer, name, node->node.name_size);
	
	// copy the data
	memcpy(full_buffer + node->node.name_size, data, node->node.size);
	
	driver_disk_write(node->address + sizeof(raw_node_t), full_buffer, node->node.size + node->node.name_size);

	heap_free(full_buffer);
}

//////////////////////////////////////////////////////////////
//// node allocation
//////////////////////////////////////////////////////////////

static bool read_next_node(node_t* node);
static bool find_unused(node_t* node);

static bool allocate_node(node_t* node);
static void resize_node_data(node_t* node, size_t newsize);
static void free_node(node_t* node);
static void merge_nodes(void);

static void resize_node_data(node_t* node, size_t newsize) {
	uintptr_t old_address = node->address;

	size_t size = NODE_SIZE(node->node) > newsize ? NODE_SIZE(node->node) : newsize;

	// save the old data
	uintptr_t buffer = heap_allocate(size);
	read_node_name(node, buffer);
	read_node_data(node, buffer + node->node.name_size);
	
	// free and reallocate the node
	free_node(node);
	node->node.size = newsize;
	allocate_node(node);

	// copy back the data
	write_node_full(node, buffer, buffer + node->node.name_size);
	heap_free(buffer);
}

static void merge_nodes(void) {

	node_t cur;
	cur.address = 0;
	while (cur.address < disk_size) {
		if (cur.node.type != NODE_UNUSED) {
			// this is a used node, just read the next one
			if (read_next_node(&cur)) {
				if (cur.is_new) {
					// this is a new node, it means we have no more nodes
					if (cur.node.prev_node_address != header.last_node) {
						// update last node
						header.last_node = cur.node.prev_node_address;
						header_changed = true;
					}
					break; 
				}
				continue;
			}
			else {
				// this was the last node
				// we can exit the merge
				return;
			}
		}
		else {
			node_t temp;
			temp.address = cur.address;
			while (read_next_node(&temp)) {
				if (temp.is_new) {
					// this is a new node, meaning we can simply delete
					// the current free one by setting the prev's node
					// next to 0
					cur.address = cur.node.prev_node_address;
					read_node(&cur);
					cur.node.next_node_address = 0;
					write_node(&cur);

					// update last node
					if (cur.address != header.last_node) {
						header.last_node = cur.address;
						header_changed = true;
					}
					return;
				}
				if (temp.node.type != NODE_UNUSED) {
					// this is a used one, we can exit the loop
					break;
				}
				cur.node.size += TOTAL_REAL_SIZE(temp);
				cur.node.next_node_address = temp.node.next_node_address;
				cur.is_new = true;
			}

			if (cur.is_new) {
				// update the node after all the merged ones
				temp.node.prev_node_address = cur.address;
				write_node(&temp);

				// update the new merged node
				write_node(&cur);
			}

			// the next node is stored in temp
			cur = temp;
		}
	}
}

static bool allocate_node(node_t* node) {
	
	node_t cur;
	cur.address = 512;
	read_node(&cur);
	while (find_unused(&cur)) {
		if (cur.is_new) {
			if (NODE_SIZE(node->node) >= disk_size - cur.address) {
				// not enough space from this new node
				// because this is a new node it means no other node fits
				// so no more space is available
				term_kwrite("allocate_node: not enough space\n");
				return false;
			}

			// this is a new node, we need to update the last one
			node_t prev;
			prev.address = cur.node.prev_node_address;
			read_node(&prev);
			prev.node.next_node_address = cur.address;
			write_node(&prev);

			// also need to update the header this is the last node
			header.last_node = cur.address;
			header_changed = true;
		}
		else if (NODE_SIZE(node->node) < NODE_SIZE(cur.node)) {
			// we have enough space for this node

			// check if we can split this node
			if (NODE_SIZE(cur.node) - TOTAL_NODE_SIZE(node->node) > sizeof(raw_node_t)) {
				// create and write the new node
				node_t new_node;
				new_node.node.name_size = 0;
				new_node.node.size = NODE_SIZE(cur.node) - TOTAL_NODE_SIZE(node->node);
				new_node.node.next_node_address = cur.node.next_node_address;
				new_node.node.prev_node_address = cur.address;
				new_node.node.parent_folder_address = 0;
				new_node.node.type = NODE_UNUSED;
				new_node.address = cur.address + TOTAL_NODE_SIZE(new_node.node);
				write_node(&new_node);

				cur.node.next_node_address = new_node.address;
			}
		}
		else {
			// not enough space, continue searching
			continue;
		}

		// set the prev and next nodes and write the node
		node->address = cur.address;
		node->node.next_node_address = cur.node.next_node_address;
		node->node.prev_node_address = cur.node.prev_node_address;
		write_node(node);
		return true;
	}

	term_kwrite("allocate_node: no free node found\n");
	return false;
}

static void free_node(node_t* node) {
	node->node.type = NODE_UNUSED;
	write_node(node);
}

static bool read_next_node(node_t* node) {
	if (node->node.next_node_address == 0) {
		node->is_new = true;
		uint64_t prev_node = node->address;
		node->address = prev_node + TOTAL_NODE_SIZE(node->node);
		if (node->address >= disk_size) {
			term_kwrite("read_next_node: not enough space\n");
			return false;
		}
		node->node.type = NODE_UNUSED;
		node->node.name_size = 0;
		node->node.size = 0;
		node->node.next_node_address = 0;
		node->node.prev_node_address = prev_node;
		return true;
	}
	node->address = node->node.next_node_address;
	read_node(node);
	return true;
}

static bool find_unused(node_t* node) {
	// TODO: loop

	if (node->address == 0) {
		node->address = header.last_node;
		read_node(node);
	}
	do {
		if (node->node.type == NODE_UNUSED) {
			return true;
		}
	} while (read_next_node(node));

	term_kwrite("find_unused: no unused node found\n");
	return false;
}

//////////////////////////////////////////////////////////////
//// folder manipulation
//////////////////////////////////////////////////////////////

static int count_folder_entries(folder_node_entry_t* nodes, int count) {
	int size = 0;
	for (int i = 0; i < count; i++) {
		if (nodes[i].file_address != 0) {
			size++;
		}
	}
	return size;
}

static bool add_folder_entry(node_t* folder, node_t* add) {
	if (folder->node.type != NODE_FOLDER) {
		term_kwrite("add_folder_entry: parent is not a folder\n");
		return false;
	}

	folder_node_entry_t* nodes = heap_allocate(folder->node.size);
	read_node_data(folder, nodes);

	if (FOLDER_SIZE(folder) <= count_folder_entries(nodes, FOLDER_SIZE(folder)) + 1) {
		uint64_t old_address = folder->address;
		
		// needs to grow folder
		heap_free(nodes);

		resize_node_data(folder, folder->node.size + sizeof(folder_node_entry_t) * 10);
		
		nodes = heap_allocate(folder->node.size);
		read_node_data(folder, nodes);
		
		for (int i = 0; i < FOLDER_SIZE(folder) - 10; i++) {
			// update files parent folder address
			if (nodes[i].file_address != 0) {
				node_t file;
				file.address = nodes[i].file_address;
				read_node(&file);
				file.node.parent_folder_address = folder->address;
				write_node(&file);
			}
		}
		
		// reset new allocated entries
		for (int i = FOLDER_SIZE(folder) - 10; i < FOLDER_SIZE(folder); i++) {
			nodes[i].file_address = 0;
		}

		// update the parent's folder new location
		if (folder->node.parent_folder_address != 0) {
			node_t parent;
			parent.address = folder->node.parent_folder_address;
			read_node(&parent);

			folder_node_entry_t* nodes = heap_allocate(parent.node.size);
			read_node_data(&parent, nodes);
			for (int i = 0; i < parent.node.size / sizeof(folder_node_entry_t); i++) {
				if (nodes[i].file_address == old_address) {
					nodes[i].file_address = folder->address;
					break;
				}
			}
			write_node_data(&parent, nodes);
			heap_free(nodes);
		}
		else {
			// this is the root folder, need to update the header
			header.root_folder_address = folder->address;
			header_changed = true;
		}
	}

	// add the entry
	for (int i = 0; i < FOLDER_SIZE(folder); i++) {
		if (nodes[i].file_address == 0) {
			nodes[i].file_address = add->address;
			break;
		}
	}
	write_node_data(folder, nodes);

	heap_free(nodes);
	return true;
}

static bool create_folder(node_t* parent, node_t* new_folder, char* name) {
	if (parent->node.type != NODE_FOLDER) {
		term_kwrite("create_folder: parent is not a folder\n");
		return false;
	}
	
	new_folder->node.type = NODE_FOLDER;
	new_folder->node.parent_folder_address = parent->address;
	new_folder->node.name_size = strlen(name);
	new_folder->node.size = sizeof(folder_node_entry_t) * 10; // automatically have enough space for 10 entries
	if (!allocate_node(new_folder)) {
		merge_nodes();
		if (!allocate_node(new_folder)) {
			term_kwrite("create_folder: failed to allocate\n");
			return false;
		}
	}

	folder_node_entry_t* entries = heap_allocate(new_folder->node.size);
	for (int i = 0; i < FOLDER_SIZE(new_folder); i++) {
		entries[i].file_address = 0;
	}
	write_node_full(new_folder, name, entries);
	heap_free(entries);

	add_folder_entry(parent, new_folder);

	if (header_changed) {
		update_header();
	}

	return true;
}

//////////////////////////////////////////////////////////////
//// node search
//////////////////////////////////////////////////////////////

static bool find_node(node_t* parent, node_t* node, char* name) {
	if (parent->node.type != NODE_FOLDER) {
		term_kwrite("find_node: parent is not a folder\n");
		return false;
	}

	folder_node_entry_t* nodes = heap_allocate(parent->node.size);
	read_node_data(parent, nodes);
	node_t temp;
	for (int i = 0; i < FOLDER_SIZE(parent); i++) {
		if (nodes[i].file_address == 0) {
			continue;
		}
		temp.address = nodes[i].file_address;
		read_node(&temp);
		char* temp_name = heap_allocate(temp.node.name_size);
		read_node_name(&temp, temp_name);
		if (strcmp(temp_name, name) == 0) {
			heap_free(temp_name);
			heap_free(nodes);
			if(node != 0) *node = temp;
			return true;
		}
		heap_free(temp_name);
	}
	heap_free(nodes);

	term_kwrite("find_node: node was not found\n");
	return false;
}

//////////////////////////////////////////////////////////////
//// filesystem manipulation
//////////////////////////////////////////////////////////////

static void update_header() {
	driver_disk_write(0, &header, sizeof(header_t));
}

static void reset_fs() {
	header.magic = TOMATO_FS_MAGIC;
	header.root_folder_address = 512;
	header.last_node = 512;

	node_t root_folder;
	root_folder.node.type = NODE_FOLDER;
	root_folder.node.parent_folder_address = 0;
	root_folder.node.name_size = sizeof("<root>");
	root_folder.node.size = sizeof(folder_node_entry_t) * 10; // automatically have enough space for 10 entries
	root_folder.address = 512;
	root_folder.node.prev_node_address = 0;
	root_folder.node.next_node_address = 0;

	folder_node_entry_t* entries = heap_allocate(root_folder.node.size);
	for (int i = 0; i < FOLDER_SIZE(&root_folder); i++) {
		entries[i].file_address = 0;
	}
	write_node(&root_folder);
	write_node_full(&root_folder, "<root>", entries);
	heap_free(entries);

	update_header();
}

void syscall_fs_init() {
	disk_size = driver_disk_size();
	term_kwrite("Disk size: ");
	uint32_t size = (disk_size / (1024 * 1024 * 1024));
	char buf[11];
	itoa(size, buf, 10);
	term_kwrite(buf);
	term_kwrite("GB\n");

	//driver_disk_read(0, &header, sizeof(header_t));

	//if (header.magic != TOMATO_FS_MAGIC) {
		term_kwrite("syscall_fs_init: no fs found, creating TomatoFS\n");
		reset_fs();
		term_kwrite("syscall_fs_init: TomatoFS created\n");
	//}

	node_t root;
	root.address = header.root_folder_address;
	read_node(&root);

	char* name = heap_allocate(root.node.name_size);
	read_node_name(&root, name);
	term_kwrite("root: ");
	term_kwrite(name);
	term_kwrite("\n");
	heap_free(name);

	term_kwrite("root address: ");
	itoa(root.address, buf, 10);
	term_kwrite(buf);
	term_kwrite("\n");

	node_t temp_folder;
	if (!create_folder(&root, &temp_folder, "test")) {
		term_kwrite("failed to create folder test\n");
	}
	else {
		term_kwrite("Folder created\n");
		term_kwrite("Folder address: ");
		itoa(temp_folder.address, buf, 10);
		term_kwrite(buf);
		term_kwrite("\n");
	}

	term_kwrite("root address: ");
	itoa(root.address, buf, 10);
	term_kwrite(buf);
	term_kwrite("\n");

	node_t found;
	if (find_node(&root, &found, "test")) {
		term_kwrite("Found folder\n");
		term_kwrite("Folder address: ");
		itoa(found.address, buf, 10);
		term_kwrite(buf);
		term_kwrite("\n");
	}
	else {
		term_kwrite("Did not find folder\n");
	}
}

