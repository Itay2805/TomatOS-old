#ifndef BOOT_PAGING_H
#define BOOT_PAGING_H

#include <kernel.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define PAGE_SIZE 4096

typedef struct page_directory_entry {
	// If the bit is set, the page is actually in physical memory at the moment. For example, when a page is swapped out, it is not in physical memory and therefore not 'Present'. If a page is called, but not present, a page fault will occur, and the OS should handle it.
    uint32_t present : 1;
	// permissions flag. If the bit is set, the page is read/write. Otherwise when it is not set, the page is read-only. The WP bit in CR0 determines if this is only applied to userland, always giving the kernel write access (the default) or both userland and the kernel (see Intel Manuals 3A 2-20).
    uint32_t read_write : 1;
	// controls access to the page based on privilege level. If the bit is set, then the page may be accessed by all; if the bit is not set, however, only the supervisor can access it. For a page directory entry, the user bit controls access to all the pages referenced by the page directory entry. Therefore if you wish to make a page a user page, you must set the user bit in the relevant page directory entry as well as the page table entry.
	uint32_t user : 1;
	// abilities of the page. If the bit is set, write-through caching is enabled. If not, then write-back is enabled instead.
    uint32_t write_through : 1;
	// If the bit is set, the page will not be cached. Otherwise, it will be.
    uint32_t cache_disabled : 1;
	// Used to discover whether a page has been read or written to. If it has, then the bit is set, otherwise, it is not. Note that, this bit will not be cleared by the CPU, so that burden falls on the OS (if it needs this bit at all).
    uint32_t accessed : 1;
    uint32_t always_0 : 1;
	// Stores the page size for that specific entry. If the bit is set, then pages are 4 MiB in size. Otherwise, they are 4 KiB. Please note that 4-MiB pages require PSE to be enabled.
    uint32_t page_size : 1;
    uint32_t unused : 4;
	// the actual address
	uint32_t address : 20;
} PACKED page_directory_entry_t;

typedef struct page_table_entry {
	// If the bit is set, the page is actually in physical memory at the moment. For example, when a page is swapped out, it is not in physical memory and therefore not 'Present'. If a page is called, but not present, a page fault will occur, and the OS should handle it.
    uint32_t present : 1;
	// permissions flag. If the bit is set, the page is read/write. Otherwise when it is not set, the page is read-only. The WP bit in CR0 determines if this is only applied to userland, always giving the kernel write access (the default) or both userland and the kernel (see Intel Manuals 3A 2-20).
	uint32_t read_write : 1;
	// controls access to the page based on privilege level. If the bit is set, then the page may be accessed by all; if the bit is not set, however, only the supervisor can access it. For a page directory entry, the user bit controls access to all the pages referenced by the page directory entry. Therefore if you wish to make a page a user page, you must set the user bit in the relevant page directory entry as well as the page table entry.
    uint32_t user : 1;
	// abilities of the page. If the bit is set, write-through caching is enabled. If not, then write-back is enabled instead.
	uint32_t write_through : 1;
	// If the bit is set, the page will not be cached. Otherwise, it will be.
	uint32_t cache_disabled : 1;
	//  is used to discover whether a page has been read or written to. If it has, then the bit is set, otherwise, it is not. Note that, this bit will not be cleared by the CPU, so that burden falls on the OS (if it needs this bit at all).
	uint32_t accessed : 1;
	// if is set, then the page has been written to. This flag is not updated by the CPU, and once set will not unset itself.
    uint32_t dirty : 1;
    uint32_t always_0 : 1;
	// if set, prevents the TLB from updating the address in its cache if CR3 is reset. Note, that the page global enable bit in CR4 must be set to enable this feature.
    uint32_t global : 1;
    uint32_t unused : 3;
	// the actual address
    uint32_t address : 20;
} PACKED page_table_entry_t;

typedef page_table_entry_t* page_table_t;
typedef page_directory_entry_t* page_directory_t;

typedef struct address {
	union {
		struct {
			uint32_t offset : 12;
			uint32_t page_table_index : 10;
			uint32_t page_directory_index : 10;
		};
		uintptr_t ptr;
	};
} address_t;

// init paging 
void paging_init(void);

// allocate a page, returns the physical address
uintptr_t paging_allocate_new_page(void);

// set a page as allocated
void paging_allocate_page(uintptr_t physAddr);

// free a page, using it's physical page
void paging_free_page(uintptr_t pageAddress);

// init a page directory, it will basically make sure the kernel and VGA buffer are identity mapped
void paging_init_directory(page_directory_t directory);

// map a virtual address, will automatically allocate a physical page for it
void paging_map(page_directory_t directory, uintptr_t virtAddr);

// map a virtual address, will automatically allocate a physical pages needed for all the size it takes
void paging_map_range(page_directory_t directory, uintptr_t virtAddr, size_t size);

// free a directory
void paging_free_directory(page_directory_t directory);

// set the current page directory
void paging_set(page_directory_t directory);

// create identity mapping for physical address
void paging_map_identity(page_directory_t pageDirectory, uintptr_t addr, bool user, bool read_write);

// identity map a range of addresses
void paging_map_identity_range(page_directory_t pageDirectory, uintptr_t addr, uint32_t size, bool user, bool read_write);

// returns the physical address of a page
uintptr_t paging_get_physical_address(page_directory_t pageDirectory, uintptr_t address);

// enable paging
extern void paging_enable(void);

// disable paging
extern void paging_disable(void);

// check if paging is enabled
extern bool paging_is_enabled(void);

#endif