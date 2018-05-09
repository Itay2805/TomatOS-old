#include "memory.h"
#include "stddef.h"
#include "string.h"

#include <api/term.h>
#include <api/color.h>

//////////////////////////////////////////////////////////////
/// the block header
//////////////////////////////////////////////////////////////


#define LIBC_MEMORY_MAGIC 0xFACC0FF
#define LIBC_MEMORY_START 0x10000

typedef struct __memory_block_t {
	struct __memory_block_t* before;
	size_t size;
	bool allocated;
	uint32_t magic;
} __memory_block_t;

//////////////////////////////////////////////////////////////
/// internal functions
//////////////////////////////////////////////////////////////

__memory_block_t* __get_block(uintptr_t ptr, bool* valid);
__memory_block_t* __get_next_block(__memory_block_t* from, bool* valid);
__memory_block_t* __get_next_free_block(__memory_block_t* from, bool* valid);
__memory_block_t* __merge_empty_blocks(__memory_block_t* from);

__memory_block_t* __get_block(uintptr_t ptr, bool* valid) {
	__memory_block_t* header = (__memory_block_t*)(ptr - sizeof(__memory_block_t));
	if (valid != nullptr) {
		*valid = header->magic == LIBC_MEMORY_MAGIC;
	}
	return header;
}

__memory_block_t* __get_next_block(__memory_block_t* from, bool* valid) {
	__memory_block_t* next = (__memory_block_t*)(((uintptr_t)from) + sizeof(__memory_block_t) + from->size);
	if (valid != nullptr) {
		*valid = next->magic == LIBC_MEMORY_MAGIC;
	}
	return next;
}

__memory_block_t* __get_next_free_block(__memory_block_t* from, bool* valid) {
	*valid = from->magic == LIBC_MEMORY_MAGIC;
	do {
		from = __get_next_block(from, valid);
	} while (*valid && !from->allocated);
	return from;
}

__memory_block_t* __merge_empty_blocks(__memory_block_t* from) {
	bool valid;

	// get next block
	__memory_block_t* next = __get_next_block(from, &valid);

	// if the block is valid and is not allocated, let's merge with it
	if (valid && !next->allocated) {
		from->size = sizeof(__memory_block_t) + next->size;
		// invalidate block, just incase
		next->size = 0;
		next->magic = 0;
	}

	// is the block before empty and valid? if so lets attempt to merge with it
	if (from->before != nullptr && from->before->magic == LIBC_MEMORY_MAGIC && !from->before->allocated) {
		return __merge_empty_blocks(from->before);
	}
	return from;
}

//////////////////////////////////////////////////////////////
/// needed variables
//////////////////////////////////////////////////////////////

uintptr_t heap_start_address;
__memory_block_t* first_free_block;

//////////////////////////////////////////////////////////////
/// the implementation
//////////////////////////////////////////////////////////////

void kernel_memory_init(void) {
	// setup our heap's start address
	heap_start_address = (uintptr_t)LIBC_MEMORY_START;

	// setup the first block, always free
	first_free_block = (__memory_block_t*)heap_start_address;
	first_free_block->allocated = false;
	first_free_block->before = nullptr;
	first_free_block->magic = LIBC_MEMORY_MAGIC;
	first_free_block->size = INT32_MAX;
}

void* malloc(size_t size) {
	if (size == 0) return nullptr;

	// we start from the first free block
	bool valid = true; // assume the first block is valid
	__memory_block_t* block = first_free_block;
	do {
		// do we have enough space
		if (block->size >= size) {

			// do we have enough space to divide the block into two blocks?
			if ((block->size - size - sizeof(__memory_block_t)) > 0) {
				// divide the memory block
				__memory_block_t* newblock = (__memory_block_t*)((uintptr_t)block + sizeof(__memory_block_t) + size);
				newblock->allocated = false;
				newblock->size = block->size - size;
				newblock->magic = LIBC_MEMORY_MAGIC;
				newblock->before = block;

				// set the block size
				block->size = size;

				// set the previous block of the next block, only if valid
				bool nextBlockValid;
				__memory_block_t* next = __get_next_block(newblock, &nextBlockValid);
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
			return (void*)((uintptr_t)block + sizeof(__memory_block_t));
		}

		// this block isn't good, get the next free one
		block = __get_next_free_block(block, &valid);
	} while (valid);

	// failed to allocate memory
	return nullptr;
}

bool free(void* ptr) {
	bool valid;
	__memory_block_t* block = __get_block(ptr, &valid);
	if (!valid) {
		return false;
	}

	block->allocated = false;

	// merge empty blocks, to save on space
	block = __merge_empty_blocks(block);

	// is this before the current first fre block
	if (block < first_free_block) {
		// if so this is now the first free block
		first_free_block = block;
	}

	return true;
}

void* realloc(void* ptr, size_t newsize) {
	// no pointer given? just return a new block
	if (ptr == nullptr) return malloc(newsize);

	bool valid;
	__get_block(ptr, &valid);
	if (!valid) {
		return nullptr;
	}

	// free the old block
	free(ptr);

	// allocate the new block
	void* newPtr = malloc(newsize);

	// if it happen to be the same block no need to copy the data
	if (newPtr == ptr) return newPtr;

	// copy the data
	memcpy(newPtr, ptr, newsize);

	return newPtr;
}

void* calloc(size_t nitems, size_t size) {
	void* mem = malloc(nitems * size);
	if(mem != nullptr) memset(mem, NULL, nitems * size);
	return mem;
}