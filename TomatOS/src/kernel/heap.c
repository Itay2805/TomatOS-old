#include "heap.h"

//////////////////////////////////////////////////////////////
//// the block header
//////////////////////////////////////////////////////////////

#include <stddef.h>
#include <string.h>
#include <syscall.h>

#include "syscalls.h"

#include "syscalls/term.h"

#define MEMORY_MAGIC 0xFACC0FF

typedef struct memory_block_t {
	struct memory_block_t* before;
	size_t size;
	bool allocated;
	uint32_t magic;
} memory_block_t;

#define HEAP_DEBUG 1

//////////////////////////////////////////////////////////////
//// needed variables
//////////////////////////////////////////////////////////////
static uintptr_t heap_start_address;
static size_t heap_size;
static memory_block_t* first_free_block;

//////////////////////////////////////////////////////////////
//// internal functions
//////////////////////////////////////////////////////////////

static memory_block_t* get_block(uintptr_t ptr, uint8_t* valid);
static memory_block_t* get_next_block(memory_block_t* from, uint8_t* valid);
static memory_block_t* get_next_free_block(memory_block_t* from, uint8_t* valid);
static memory_block_t* merge_empty_blocks(memory_block_t* from);

static memory_block_t* get_block(uintptr_t ptr, uint8_t* valid) {
	memory_block_t* header = (memory_block_t*)(ptr - sizeof(memory_block_t));
	if (valid != NULL) {
		*valid = header->magic == MEMORY_MAGIC;
	}
	return header;
}

static memory_block_t* get_next_block(memory_block_t* from, uint8_t* valid) {
	memory_block_t* next = (memory_block_t*)(((uintptr_t)from) + sizeof(memory_block_t) + from->size);
	if (valid != NULL) {
		*valid = next->magic == MEMORY_MAGIC;
	}
	return next;
}

static memory_block_t* get_next_free_block(memory_block_t* from, uint8_t* valid) {
	*valid = from->magic == MEMORY_MAGIC;
	do {
		from = get_next_block(from, valid);
	} while (*valid && from->allocated);
	return from;
}

static memory_block_t* merge_empty_blocks(memory_block_t* from) {
	uint8_t valid;

	// get next block
	memory_block_t* next = get_next_block(from, &valid);
	
	// if the block is valid and is not allocated, let's merge with it
	if (valid && !next->allocated) {
		// set the previous block of the next block after the delete
		memory_block_t* n = get_next_block(next, &valid);
		if (valid) {
			n->before = from;
		}

		from->size += sizeof(memory_block_t) + next->size;
		// invalidate block, just incase
		next->size = 0;
		next->magic = 0;
	}

	// is the block before empty and valid? if so lets attempt to merge with it
	if (from->before != NULL && from->before->magic == MEMORY_MAGIC && !from->before->allocated) {
		return merge_empty_blocks(from->before);
	}
	return from;
}

//////////////////////////////////////////////////////////////
//// needed variables
//////////////////////////////////////////////////////////////
static uintptr_t heap_start_address;
static size_t heap_size;
static memory_block_t* first_free_block;

//////////////////////////////////////////////////////////////
//// syscalls
//////////////////////////////////////////////////////////////

// @TODO: move to the syscall directory

static void syscall_heap_allocate(registers_t* regs) {
	regs->eax = (uint32_t)heap_allocate((size_t)regs->ebx);
}

static void syscall_heap_reallocate(registers_t* regs) {
	regs->eax = (uint32_t)heap_reallocate((void*)regs->ebx, (size_t)regs->ecx);
}

static void syscall_heap_free(registers_t* regs) {
	heap_free((void*)regs->ebx);
}

//////////////////////////////////////////////////////////////
//// the implementation
//////////////////////////////////////////////////////////////

void initialize_heap(size_t start, size_t size) {
	heap_start_address = (uintptr_t)start;
	heap_size = size;

	first_free_block = (memory_block_t*)heap_start_address;
	first_free_block->allocated = false;
	first_free_block->before = NULL;
	first_free_block->magic = MEMORY_MAGIC;
	first_free_block->size = heap_size - sizeof(memory_block_t);

	// register system calls
	register_syscall(TOMATO_SYSCALL_HEAP_ALLOCATE, syscall_heap_allocate);
	register_syscall(TOMATO_SYSCALL_HEAP_REALLOCATE, syscall_heap_reallocate);
	register_syscall(TOMATO_SYSCALL_HEAP_FREE, syscall_heap_free);
}

void* heap_allocate(size_t size) {
	if (size == 0) return NULL;

	// we start from the first free block
	bool valid = true; // assume the first block is valid
	memory_block_t* block = first_free_block;
	do {
		// do we have enough space
		if (block->size >= size) {

			// do we have enough space to divide the block into two blocks?
			if ((block->size - size - sizeof(memory_block_t)) > 0) {
				// divide the memory block
				memory_block_t* newblock = (memory_block_t*)(((uintptr_t)block) + sizeof(memory_block_t) + size);
				newblock->allocated = false;
				newblock->size = block->size - size - sizeof(memory_block_t);
				newblock->magic = MEMORY_MAGIC;
				newblock->before = block;

				// set the block size
				block->size = size;

				// set the previous block of the next block, only if valid
				bool nextBlockValid;
				memory_block_t* next = get_next_block(newblock, &nextBlockValid);
				if (nextBlockValid) {
					next->before = newblock;
				}

				if (first_free_block == block) {
					// now the first allocated block is the newly created one
					first_free_block = newblock;
				}
			}

			// block is now allocated! return the pointer to the data
			block->allocated = true;

			return (void*)(((uintptr_t)block) + sizeof(memory_block_t));
		}

		// this block isn't good, get the next free one
		block = get_next_free_block(block, &valid);
	} while (valid);

	// failed to allocate memory
	return NULL;
}

void* heap_reallocate(void* ptr, size_t newsize) {
	// no pointer given? just return a new block
	if (ptr == NULL) return heap_allocate(newsize);

	bool valid;
	get_block(ptr, &valid);
	if (!valid) {
		return NULL;
	}

	// free the old block
	heap_free(ptr);

	// allocate the new block
	void* newPtr = heap_allocate(newsize);

	// did the allocation fail? if so return NULL
	if (newPtr == NULL) {
		return NULL;
	}

	// if it happen to be the same block no need to copy the data
	if (newPtr == ptr) return newPtr;

	// copy the data
	memcpy(newPtr, ptr, newsize);

	return newPtr;
}

bool heap_valid_block(void* ptr) {
	bool valid;
	memory_block_t* block = get_block(ptr, &valid);
	return valid;
}

bool heap_free(void* ptr) {
	if (ptr == NULL) {
		return false;
	}

	bool valid;
	memory_block_t* block = get_block(ptr, &valid);
	if (!valid) {
		return false;
	}

	block->allocated = false;

	// merge empty blocks, to save on space
	block = merge_empty_blocks(block);

	// is this before the current first fre block
	if (block < first_free_block) {
		// if so this is now the first free block
		first_free_block = block;
	}

	return true;
}

#undef MEMORY_MAGIC
