#include "fs.h"

#include "../drivers/disk.h"

#include "../heap.h"

#include "../syscalls/term.h"
#include "../syscalls.h"

#include <string.h>
#include <tomato.h>

#define TOMATO_FS_MAGIC 0x53466f74616d6f54

#define NODE_SIZE(n) ((n).size + (n).name_size)
#define TOTAL_NODE_SIZE(n) ((n).size + (n).name_size + sizeof(raw_node_t))
#define REAL_SIZE(n) ((n).next_node_address - (n).address  + sizeof(raw_node_t))
#define TOTAL_REAL_SIZE(n) ((n).node.next_node_address - (n).address)
#define FOLDER_SIZE(folder) ((folder)->node.size / sizeof(folder_node_entry_t))

#if 0
	#if 0
		#define FS_DEBUG_NODE_IO(x) term_kwrite(x)
	#else 
		#define FS_DEBUG_NODE_IO(x)
	#endif


	#if 0
		#define FS_DEBUG_NODE_ALLOCATION(x) term_kwrite(x)
	#else 
		#define FS_DEBUG_NODE_ALLOCATION(x)
	#endif


	#if 1
		#define FS_DEBUG_NODE_SEARCH(x) term_kwrite(x)
	#else 
		#define FS_DEBUG_NODE_SEARCH(x)
	#endif

	#if 0
		#define FS_DEBUG_FOLDER(x) term_kwrite(x)
	#else
		#define FS_DEBUG_FOLDER(x) 
	#endif

	#define FS_DEBUG(x) (x)
	#define FS_DEBUG_PRINT_NODE_NAME(node) { char* name = heap_allocate(node.name_size); term_kwrite(name); heap_free(name); } 
#else
	#define FS_DEBUG(x)
	#define FS_DEBUG_PRINT_NODE_NAME(node)
	#define FS_DEBUG_NODE_SEARCH(x)
	#define FS_DEBUG_NODE_ALLOCATION(x)
	#define FS_DEBUG_NODE_IO(x)
	#define FS_DEBUG_FOLDER(x) 
#endif


//////////////////////////////////////////////////////////////
//// file system node structures
//////////////////////////////////////////////////////////////

typedef struct header_t {
	uint64_t magic;
	uint64_t root_folder_address;

	uint64_t last_node;
} header_t;

#define NODE_UNUSED 0x53554e55
#define NODE_FILE	0x454c4946 
#define NODE_FOLDER 0x52444c46 

typedef struct raw_node_t {
	uint32_t type;
	size_t size;
	size_t name_size;
	uint64_t parent_folder_address;

	uint64_t prev_node_address;
	uint64_t next_node_address;
} raw_node_t;

typedef struct folder_node_entry_t {
	uint64_t file_address;
} folder_node_entry_t;

typedef struct node_t {
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
	FS_DEBUG_NODE_ALLOCATION("> resize_node_data\n");

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

	FS_DEBUG_NODE_ALLOCATION("resize_node_data: resized\n");
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
	FS_DEBUG_NODE_ALLOCATION("> allocate_node\n");

	node_t cur;
	cur.address = 0;
	while (find_unused(&cur)) {
		if (cur.is_new) {
			if (NODE_SIZE(node->node) >= disk_size - cur.address) {
				// not enough space from this new node
				// because this is a new node it means no other node fits
				// so no more space is available
				FS_DEBUG_NODE_ALLOCATION("allocate_node: not enough space\n");
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
		FS_DEBUG_NODE_ALLOCATION("allocate_node: before allocated\n");
		write_node(node);
		FS_DEBUG_NODE_ALLOCATION("allocate_node: allocated\n");
		return true;
	}

	FS_DEBUG_NODE_ALLOCATION("allocate_node: no free node found\n");
	return false;
}

static void free_node(node_t* node) {
	FS_DEBUG_NODE_ALLOCATION("> free_node\n");
	node->node.type = NODE_UNUSED;
	write_node(node);
	FS_DEBUG_NODE_ALLOCATION("free_node: freed\n");
}

static bool read_next_node(node_t* node) {
	FS_DEBUG_NODE_ALLOCATION("> read_next_node\n");

	if (node->node.next_node_address == 0) {
		node->is_new = true;
		uint64_t prev_node = node->address;
		node->address = prev_node + TOTAL_NODE_SIZE(node->node);
		if (node->address >= disk_size) {
			FS_DEBUG_NODE_ALLOCATION("read_next_node: not enough space\n");
			return false;
		}
		node->node.type = NODE_UNUSED;
		node->node.name_size = 0;
		node->node.size = 0;
		node->node.next_node_address = 0;
		node->node.prev_node_address = prev_node;
		FS_DEBUG_NODE_ALLOCATION("read_next_node: new node\n");
		return true;
	}
	node->address = node->node.next_node_address;
	read_node(node);

	FS_DEBUG_NODE_ALLOCATION("read_next_node: found node\n");
	return true;
}

static bool find_unused(node_t* node) {
	FS_DEBUG_NODE_ALLOCATION("> find_unused\n");
	// TODO: loop

	if (node->address == 0) {
		node->address = 512;
	}
	read_node(node);
	do {
		if (node->node.type == NODE_UNUSED) {
			FS_DEBUG_NODE_ALLOCATION("find_unused: found\n");
			return true;
		}
	} while (read_next_node(node));

	FS_DEBUG_NODE_ALLOCATION("find_unused: no unused node found\n");
	return false;
}

//////////////////////////////////////////////////////////////
//// node search
//////////////////////////////////////////////////////////////

static bool find_node(node_t* parent, node_t* node, char* name);
static bool traverse_path(node_t* node, char* path);

static bool traverse_path(node_t* node, char* path) {
	FS_DEBUG_NODE_SEARCH("> traverse_path\n");

	char* path_orig = path;

	if (*path_orig == '\\' || *path_orig == '/') {
		path_orig++;
	}

	if (strlen(path_orig) == 0) {
		node->address = header.root_folder_address;
		read_node(node);
		FS_DEBUG_NODE_SEARCH("traverse_path: root\n");
		return true;
	}

	char* path_start = heap_allocate(strlen(path_orig) + 1);
	path = path_start;

	int count = 1;
	while (*path_orig) {
		if (*path_orig == '\\' || *path_orig == '/') {
			*path = 0;
			count++;
		}
		else {
			*path = *path_orig;
		}
		path++;
		path_orig++;
	}
	*path = 0;
	path = path_start;

	node_t current;
	current.address = header.root_folder_address;
	read_node(&current);

	while (count--) {
		if (!find_node(&current, &current, path)) {
			heap_free(path_start);
			FS_DEBUG_NODE_SEARCH("traverse_path: was not found\n");
			return false;
		}
		// go to the next file
		while (*path) {
			path++;
		}
		path++;
	}

	*node = current;

	heap_free(path_start);
	FS_DEBUG_NODE_SEARCH("traverse_path: found\n");
	return true;
}

static bool find_node(node_t* parent, node_t* node, char* name) {	
	FS_DEBUG_NODE_SEARCH("> find_node\n");
	if (parent->node.type != NODE_FOLDER) {
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
			if (node != 0) *node = temp;
			FS_DEBUG_NODE_SEARCH("find_node: found\n");
			return true;
		}
		heap_free(temp_name);
	}
	heap_free(nodes);

	FS_DEBUG_NODE_SEARCH("find_node: not found\n");
	return false;
}

//////////////////////////////////////////////////////////////
//// folder manipulation
//////////////////////////////////////////////////////////////

static int count_folder_entries(folder_node_entry_t* nodes, int count);
static bool add_folder_entry(node_t* folder, node_t* add);
static bool create_folder(node_t* parent, node_t* new_folder, char* name);

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
	FS_DEBUG_FOLDER("> add_folder_entry\n");
	if (folder->node.type != NODE_FOLDER) {
		FS_DEBUG_FOLDER("add_folder_entry: not a folder\n");
		return false;
	}

	folder_node_entry_t* nodes = heap_allocate(folder->node.size);
	read_node_data(folder, nodes);

	if (FOLDER_SIZE(folder) <= count_folder_entries(nodes, FOLDER_SIZE(folder)) + 1) {
		FS_DEBUG_FOLDER("add_folder_entry: growing folder\n");
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
	FS_DEBUG_FOLDER("add_folder_entry: entry added\n");
	return true;
}

static bool create_folder(node_t* parent, node_t* new_folder, char* name) {
	FS_DEBUG_FOLDER("> create_folder\n");
	if (parent->node.type != NODE_FOLDER) {
		FS_DEBUG_FOLDER("create_folder: not a folder\n");
		return false;
	}

	if (find_node(parent, 0, name)) {
		FS_DEBUG_FOLDER("create_folder: already exists\n");
		return false;
	}
	
	new_folder->node.type = NODE_FOLDER;
	new_folder->node.parent_folder_address = parent->address;
	new_folder->node.name_size = strlen(name) + 1;
	new_folder->node.size = sizeof(folder_node_entry_t) * 10; // automatically have enough space for 10 entries
	if (!allocate_node(new_folder)) {
		merge_nodes();
		if (!allocate_node(new_folder)) {
			FS_DEBUG_FOLDER("create_folder: failed to allocate\n");
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

	update_header();

	FS_DEBUG_FOLDER("create_folder: created\n");
	return true;
}

//////////////////////////////////////////////////////////////
//// filesystem manipulation
//////////////////////////////////////////////////////////////

static void update_header() {
	if (header_changed) {
		driver_disk_write(0, &header, sizeof(header_t));
		header_changed = false;
	}
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
	for (int i = 0; i < root_folder.node.size / sizeof(folder_node_entry_t); i++) {
		entries[i].file_address = 0;
	}
	write_node(&root_folder);
	write_node_full(&root_folder, "<root>", entries);
	heap_free(entries);

	update_header();
}

//////////////////////////////////////////////////////////////
//// syscalls
//////////////////////////////////////////////////////////////

static void syscall_list(registers_t* regs) {
	int* ret_count = regs->ecx;
	node_t folder;
	if (traverse_path(&folder, (char*)regs->ebx)) {
		if (folder.node.type == NODE_FOLDER) {
			
			folder_node_entry_t* entries = heap_allocate(folder.node.size);
			read_node_data(&folder, entries);
			
			int count = count_folder_entries(entries, folder.node.size / sizeof(folder_node_entry_t));
			uintptr_t list_base = heap_allocate(sizeof(size_t) + sizeof(tomato_list_entry_t) * count);
			size_t* list_size = list_base;
			tomato_list_entry_t* list = (tomato_list_entry_t*)(list_base + sizeof(tomato_list_entry_t));
			*ret_count = count;
			*list_size = count;

			for (int i = 0; i < folder.node.size / sizeof(folder_node_entry_t); i++) {
				if (entries[i].file_address != 0) {
					node_t file;
					file.address = entries[i].file_address;
					read_node(&file);
					list->name = heap_allocate(file.node.name_size);
					read_node_name(&file, list->name);
					list++;
				}
			}

			regs->eax = (uint32_t)(list_base + sizeof(size_t));
			heap_free(entries);
		}
		else {
			*ret_count = 0;
		}
	}
	else {
		*ret_count = 0;
	}
}

static void syscall_list_release(registers_t* regs) {
	tomato_list_entry_t* entries = regs->ebx;
	size_t size = *(size_t*)(((uintptr_t*)entries) - sizeof(size_t));

	for (int i = 0; i < size; i++) {
		heap_free(entries[i].name);
	}

	heap_free(entries);
}

static void syscall_exists(registers_t* regs) {
	node_t node;
	regs->eax = traverse_path(&node, (char*)regs->ebx);
}

static void syscall_open(registers_t* regs) {
	uintptr_t handle = heap_allocate(sizeof(tomato_file_handle_t) + sizeof(node_t));
	node_t* node = handle;
	tomato_file_handle_t* fh = (handle + sizeof(node_t));

	if (traverse_path(node, regs->ebx)) {
		fh->name = heap_allocate(node->node.name_size);
		read_node_name(node, fh->name);
		fh->size = node->node.size;
		fh->type = node->node.type;
	}
	else {
		heap_free(handle);
		regs->eax = 0;
	}

	regs->eax = fh;
}

static void syscall_close(registers_t* regs) {
	uintptr_t handle = regs->ebx;
	if (handle == 0) {
		return;
	}

	node_t* node = (handle - sizeof(node_t));
	tomato_file_handle_t* fh = handle;

	heap_free(fh->name);
	heap_free(node);
}

static void syscall_make_dir(registers_t* regs) {
	char* path = regs->ebx;
	char* path_orig = path;

	if (*path_orig == '\\' || *path_orig == '/') {
		path_orig++;
	}

	if (strlen(path_orig) == 0) {
		return;
	}

	path = heap_allocate(strlen(path_orig) + 1);
	char* path_start = path;

	int count = 1;
	while (*path_orig) {
		if (*path_orig == '\\' || *path_orig == '/') {
			*path = 0;
			count++;
		}
		else {
			*path = *path_orig;
		}
		path++;
		path_orig++;
	}
	*path = 0;
	path = path_start;

	node_t current;
	current.address = header.root_folder_address;
	read_node(&current);

	while (count--) {
		if (!find_node(&current, &current, path)) {
			node_t parent = current;

			create_folder(&parent, &current, path);
		}
		// go to the next file
		while (*path) {
			path++;
		}
		path++;
	}
	
	heap_free(path_start);
}

void syscall_fs_init() {
	disk_size = driver_disk_size();
	term_kwrite("syscall_fs_init: Disk size: ");
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

	term_kwrite("syscall_fs_init: registering FS syscalls\n");
	register_syscall(TOMATO_SYSCALL_FS_LIST, syscall_list);
	register_syscall(TOMATO_SYSCALL_FS_LIST_RELEASE, syscall_list_release);
	register_syscall(TOMATO_SYSCALL_FS_EXISTS, syscall_exists);
	register_syscall(TOMATO_SYSCALL_FS_MAKE_DIR, syscall_make_dir);
	register_syscall(TOMATO_SYSCALL_FS_OPEN, syscall_open);
	register_syscall(TOMATO_SYSCALL_FS_CLOSE, syscall_close);
}

