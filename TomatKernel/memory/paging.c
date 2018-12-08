#include "paging.h"

#include <string.h>

#include <kernel.h>

#define PAGE_COUNT 1024 * 1024
#define BITMAP_SIZE_BYTES PAGE_COUNT / 8
#define BITMAP_HAS_FREE(x) (page_bitmap[(x)] != 0b11111111)

static uint8_t page_bitmap[BITMAP_SIZE_BYTES];
static uint16_t first_free;

uintptr_t paging_allocate_new_page(void) {
	int i = 0;
	for (; i < 8; i++) {
		if ((page_bitmap[first_free] & (1 << i)) == 0) {
			page_bitmap[first_free] |= (1 << i);
			break;
		}
	}
	
	int currentByte = first_free;

	// go over until finding a free spot
	while (!BITMAP_HAS_FREE(first_free)) {
		first_free++;
	}

	// failed to allocate (the first_free didn't have any free)
	// now after we found the real first free, we can reallocate
	if (i == 8) {
		return paging_allocate_new_page();
	}
	// success! calculate the physical page address
	else {
		return (uintptr_t)((currentByte * 8 + i) * 4096);
	}
}

void paging_allocate_page(uintptr_t physAddr) {
	// calculate the page bit and index
	int bit = ((size_t)physAddr / 4096) % 8;
	int index = (((size_t)physAddr / 4096) - bit) / 8;

	page_bitmap[index] |= 1 << bit;
}

void paging_free_page(uintptr_t physAddr) {
	// calculate the page bit and index
	int bit = ((size_t)physAddr / 4096) % 8;
	int index = (((size_t)physAddr / 4096) - bit) / 8;

	// if the index is before the first_free, set it to first_free
	if (index < first_free) {
		first_free = index;
	}

	// turn of the bit
	page_bitmap[index] &= ~(1 << bit);
}

void paging_init(void) {
	// all the pages from 0 to the end of the kernel size are always identity pages

	// prepare page bitmap 
	first_free = 0;
	memset(page_bitmap, 0, BITMAP_SIZE_BYTES);

    // set areas which are important (ex. memory mapped IO) as used

	// don't allow the first page to be allocated
	paging_allocate_page(0);

    // set the kernel area as used
	int kernelPageCount = (((size_t)&kernel_size - 1) / 4096) + 1;
	for (int i = 0; i < kernelPageCount; i++) {
		paging_allocate_page((uintptr_t)&kernel_start + i * 4096);
	}

	// set the kernel heap area as used
	int kernelHeapPageCount = ((MB(5) - 1) / 4096) + 1;
	for (int i = 0; i < kernelHeapPageCount; i++) {
		paging_allocate_page(MB(5) + i * 4096);
	}

	// set the VGA text buffer as used 
	paging_allocate_page((uintptr_t)0xB8000);
}

void paging_init_directory(page_directory_t page_directory) {
	// this is not a long term solution since it will not work in long mode..
	// but we must do this in order to have direct memory access without having some
	// full identity mapping of physical memory
	bool paging_was_enabled = paging_is_enabled(); 
	if(paging_was_enabled) paging_disable(); 

	memset(page_directory, 0, 4096);

    // identity map the kernel
	paging_map_identity_range(page_directory, (uintptr_t)&kernel_start, (size_t)&kernel_size, false, false);

	// mapping the kernel heap
	paging_map_identity_range(page_directory, MB(5), MB(5), false, false);

	// identity map the VGA-text buffer
	paging_map_identity(page_directory, 0xb8000, false, false);

	// re-enable if it was enabled
	if(paging_was_enabled) paging_enable();
}

void paging_map_identity_range(page_directory_t page_directory_index, uintptr_t addr, uint32_t size, bool user, bool read_write) {
	int pageCount = ((size - 1) / 4096) + 1;

	for (int i = 0; i < pageCount; i++) {
		paging_map_identity(page_directory_index, ((uintptr_t)addr) + i * 4096, user, read_write);
	}
}

void paging_map_identity(page_directory_t page_directory, uintptr_t addr, bool user, bool read_write) {
	int page_directory_index = (size_t)addr >> 22;
	int page_table_index = (size_t)addr >> 12 & 0x03FF;

	if (!page_directory[page_directory_index].present) {
		page_table_t table = (page_table_t)paging_allocate_new_page();
		memset(table, 0, 4096);
		page_directory[page_directory_index].present = true;
		page_directory[page_directory_index].read_write = true;
		page_directory[page_directory_index].user = true;
		page_directory[page_directory_index].address = (size_t)table >> 12;
	}

	// get the page table
	page_table_t page_table = (page_table_t)(page_directory[page_directory_index].address << 12);

	// if the table is not presented, init it
	if (!page_table[page_table_index].present) {
		page_table[page_table_index].present = true;
	}
	// otherwise set it's page as free
	else {
		uintptr_t addr = (uintptr_t)(page_table[page_table_index].address << 12);
		paging_free_page(addr);
	}

	// set the flags and new address
	page_table[page_table_index].global = true;
	page_table[page_table_index].read_write = read_write;
	page_table[page_table_index].user = user;
	page_table[page_table_index].address = (size_t)(addr) >> 12;
}

void paging_map_range(page_directory_t page_directory, uintptr_t virtAddr, size_t size) {
	int pageCount = ((size - 1) / 4096) + 1;

	for (int i = 0; i < pageCount; i++) {
		paging_map(page_directory, ((uintptr_t)virtAddr) + i * 4096);
	}
}

void paging_map(page_directory_t page_directory, uintptr_t virtAddr) {
	int page_directory_index = (size_t)virtAddr >> 22;
	int page_table_index = (size_t)virtAddr >> 12 & 0x03FF;

	if (!page_directory[page_directory_index].present) {
		page_table_t pageTable = (page_table_t)paging_allocate_new_page();
		memset(pageTable, 0, 4096);
		page_directory[page_directory_index].present = true;
		page_directory[page_directory_index].read_write = true;
		page_directory[page_directory_index].user = true;
		page_directory[page_directory_index].address = (size_t)pageTable >> 12;
	}

	// get the page table
	page_table_t page_table = (page_table_t)(page_directory[page_directory_index].address << 12);
	
	// if the table is not presented, init it
	if (!page_table[page_table_index].present) {
		page_table[page_table_index].present = true;
	}
	// otherwise set it's page as free
	else {
		uintptr_t addr = (uintptr_t)(page_table[page_table_index].address << 12);
		paging_free_page(addr);
	}

	// set the flags and new address
	page_table[page_table_index].read_write = true;
	page_table[page_table_index].user = true;
	page_table[page_table_index].address = (size_t)paging_allocate_new_page() >> 12;
}

void paging_free_directory(page_directory_t directory) {
	bool paging_was_enabled = paging_is_enabled(); 
	if(paging_was_enabled) paging_disable();

	// go over all the directory entries
	for (int i = 0; i < 1024; i++) {
		// if the entry exists
		if (directory[i].present) {
			// go over all the table entries
			page_table_t table = (page_table_t)(directory[i].address << 12);
			for (int j = 0; j < 1024; j++) {
				// if the table entry exists
				if (table[j].present) {
					// free the physical page
					paging_free_page((uintptr_t)(table[j].address << 12));
				}
			}
			// free the table
			paging_free_page((uintptr_t)table);
		}
	}
	// free the directory page
	paging_free_page((uintptr_t)directory);

	if(paging_was_enabled) paging_enable();
}

uintptr_t paging_get_physical_address(page_directory_t pageDirectory, uintptr_t address) {
	address_t addr = {0};
	addr.ptr = address;

	if (!pageDirectory[addr.page_directory_index].present) return NULL;
	
	page_table_t pageTable = (page_table_t)(pageDirectory[addr.page_directory_index].address << 12);

	if (!pageTable[addr.page_table_index].present) return NULL;

	return (uintptr_t)(pageTable[addr.page_table_index].address << 12 | addr.offset);
}

void paging_set(page_directory_t directory) {
	asm volatile("mov %0, %%cr3":: "r"(directory));
}
