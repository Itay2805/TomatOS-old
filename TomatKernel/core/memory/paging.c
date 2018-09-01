#include "paging.h"

#include <string.h>
#include <math.h>

#include <core/term.h>

#include <kernel.h>

#define PAGE_COUNT 1024 * 1024
#define BITMAP_SIZE_BYTES PAGE_COUNT / 8
#define BITMAP_HAS_FREE(x) (page_bitmap[(x)] != 0b11111111)

#define PAGIN_ALWAYS_DIRECT_MAPPING 

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

//////////////////////////////////////////////////////////////////////////////////////
// Tomat Kernel Memory Mapping
//////////////////////////////////////////////////////////////////////////////////////
// Physically on RAM:
//
// Page 0:
//		Always set to taken, this is just because otherwise it might cause a fault if the page is allocated
//
// Page 1-255:
//		Free to be used by whatever
//
// Page 256-kernel_end:
//		Kernel Pages, always identity mapped
//		the reason we have a ? is because idk the actual size of the kernel
//		and the kernel will determine how many pages it needs
// 
// Pages ?-1048576
//		Free to be used by whatever
//
// Special Pages:
//	
//		Page 184 (Address: 0xb8000):
//			This is the VGA text buffer, it is identity mapped only to the kernel
//			Maybe Programs should also have direct access to it?
//
//////////////////////////////////////////////////////////////////////////////////////

void paging_init(void) {
	term_write("[paging] Initializing\n");

	// all the pages from 0 to the end of the kernel size are always identity pages
	int identityPageCount = ceil((size_t)(&tomatkernel_size) / 4096.0);

	// prepare page bitmap 
	first_free = 0;
	memset(page_bitmap, 0, BITMAP_SIZE_BYTES);

	// make sure the null page won't ever get allocated
	paging_allocate_page(0);

	// make sure the kernel pages are already set as used
	int kernelPageCount = (((size_t)&tomatkernel_size - 1) / 4096) + 1;
	for (int i = 0; i < kernelPageCount; i++) {
		paging_allocate_page((size_t)&tomatkernel_start + i * 4096);
	}

	// make sure the kernel heap is allocated
	uintptr_t kernelHeapStart = (((size_t)&tomatkernel_end + 1024 * 1024) >> 12) << 12;
	int kernelHeapPageCount = (((1024 * 1024 * 45) - 1) / 4096) + 1;
	for (int i = 0; i < kernelHeapPageCount; i++) {
		paging_allocate_page((size_t)(kernelHeapStart) + i * 4096);
	}

	// make sure the VGA text buffer is already set as used
	paging_allocate_page(0xb8000);
}

void paging_init_directory(page_directory_t pageDirectory) {
	memset(pageDirectory, 0, 4096);


	// identity map the kernel
	int kernelPageCount = (((size_t)(&tomatkernel_size) - 1) / 4096) + 1;
	for (int i = 0; i < kernelPageCount; i++) {
		paging_map_identity(pageDirectory, (uintptr_t)(&tomatkernel_start) + i * 4096, false, false);
	}

	// identity map the kernel heap
	uintptr_t kernelHeapStart = (((size_t)&tomatkernel_end + 1024 * 1024) >> 12) << 12;
	int kernelHeapPageCount = (((1024 * 1024 * 45) - 1) / 4096) + 1;
	for (int i = 0; i < kernelHeapPageCount; i++) {
		paging_map_identity(pageDirectory, (uintptr_t)(kernelHeapStart) + i * 4096, true, false);
	}

	// identity map the VGA video address
	paging_map_identity(pageDirectory, (uintptr_t)0xb8000, true, true);
}

void paging_map_identity(page_directory_t pageDirectory, uintptr_t addr, bool user, bool read_write) {
	int pdindex = (size_t)addr >> 22;
	int ptindex = (size_t)addr >> 12 & 0x03FF;

	if (!pageDirectory[pdindex].present) {
		page_table_t table = (page_table_t)paging_allocate_new_page();
		memset(table, 0, 4096);
		pageDirectory[pdindex].present = true;
		pageDirectory[pdindex].read_write = true;
		pageDirectory[pdindex].user = true;
		pageDirectory[pdindex].address = (size_t)table >> 12;
	}

	// get the page table
	page_table_t pageTable = (page_table_t)(pageDirectory[pdindex].address << 12);

	// if the table is not presented, init it
	if (!pageTable[ptindex].present) {
		pageTable[ptindex].present = true;
	}
	// otherwise set it's page as free
	else {
		uintptr_t addr = (uintptr_t)(pageTable[ptindex].address << 12);
		paging_free_page(addr);
	}

	// set the flags and new address
	pageTable[ptindex].read_write = read_write;
	pageTable[ptindex].user = user;
	pageTable[ptindex].address = (size_t)(addr) >> 12;
}

void paging_map_range(page_directory_t pageDirectory, uintptr_t virtAddr, size_t size) {
	int pageCount = ((size - 1) / 4096) + 1;

	for (int i = 0; i < pageCount; i++) {
		paging_map(pageDirectory, ((uintptr_t)virtAddr) + i * 4096);
	}
}

void paging_map(page_directory_t directory, uintptr_t virtAddr) {
	int pdindex = (size_t)virtAddr >> 22;
	int ptindex = (size_t)virtAddr >> 12 & 0x03FF;

	if (!directory[pdindex].present) {
		page_table_t pageTable = (page_table_t)paging_allocate_new_page();
		memset(pageTable, 0, 4096);
		directory[pdindex].present = true;
		directory[pdindex].read_write = true;
		directory[pdindex].user = true;
		directory[pdindex].address = (size_t)pageTable >> 12;
	}

	// get the page table
	page_table_t pageTable = (page_table_t)(directory[pdindex].address << 12);
	
	// if the table is not presented, init it
	if (!pageTable[ptindex].present) {
		pageTable[ptindex].present = true;
	}
	// otherwise set it's page as free
	else {
		uintptr_t addr = (uintptr_t)(pageTable[ptindex].address << 12);
		paging_free_page(addr);
	}

	// set the flags and new address
	pageTable[ptindex].read_write = true;
	pageTable[ptindex].user = true;
	pageTable[ptindex].address = (size_t)paging_allocate_new_page() >> 12;
}

void paging_free_directory(page_directory_t directory) {
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
#ifndef _MBCS
	asm volatile("mov %0, %%cr3":: "r"(directory));
#endif
}

