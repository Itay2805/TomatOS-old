#include "heap.h"

#include <stddef.h>
#include <kernel.h>

#include <core/process/process.h>
#include <core/process/syscall.h>

//////////////////////////////////////////////////////////////
//// internal functions
//////////////////////////////////////////////////////////////

static heap_block_t* get_block(heap_context_t* context, uintptr_t ptr, bool* valid);
static heap_block_t* get_next_block(heap_context_t* context, heap_block_t* from, bool* valid);
static heap_block_t* get_next_free_block(heap_context_t* context, heap_block_t* from, bool* valid);
static heap_block_t* merge_empty_blocks(heap_context_t* context, heap_block_t* from);
static void expand(heap_context_t* context, size_t size);

static heap_block_t* get_block(heap_context_t* context, uintptr_t ptr, bool* valid) {
	heap_block_t* header = (heap_block_t*)(ptr - sizeof(heap_block_t));
	if (valid != NULL) {
		if ((size_t)header >= (size_t)context->start + context->size) {
			*valid = false;
		}
		else {
			*valid = header->magic == HEAP_MAGIC;
		}
	}
	return header;
}

static heap_block_t* get_next_block(heap_context_t* context, heap_block_t* from, bool* valid) {
	heap_block_t* next = (heap_block_t*)(((uintptr_t)from) + sizeof(heap_block_t) + from->size);
	if (valid != NULL) {
		if ((uintptr_t)next >= (uintptr_t)context->start + context->size) {
			*valid = false;
		}
		else {
			*valid = next->magic == HEAP_MAGIC;
		}
	}
	return next;
}

static heap_block_t* get_next_free_block(heap_context_t* context, heap_block_t* from, bool* valid) {
	*valid = from->magic == HEAP_MAGIC;
	do {
		from = get_next_block(context, from, valid);
	} while (*valid && from->allocated);
	return from;
}

static heap_block_t* merge_empty_blocks(heap_context_t* context, heap_block_t* from) {
	bool valid;

	// get next block
	heap_block_t* next = get_next_block(context, from, &valid);

	// if the block is valid and is not allocated, let's merge with it
	if (valid && !next->allocated) {
		// set the previous block of the next block after the delete
		heap_block_t* n = get_next_block(context, next, &valid);
		if (valid) {
			n->before = from;
		}

		from->size += sizeof(heap_block_t) + next->size;
		// invalidate block, just incase
		next->size = 0;
		next->magic = 0;

		// remove the removed header from the used size
		context->used_size -= sizeof(heap_block_t);

		if (next == context->top_block) {
			context->top_block = from;
		}
	}

	// is the block before empty and valid? if so lets attempt to merge with it
	if (from->before != NULL && from->before->magic == HEAP_MAGIC && !from->before->allocated) {
		return merge_empty_blocks(context, from->before);
	}
	return from;
}

static void expand(heap_context_t* context, size_t size) {
	uintptr_t top = (uintptr_t)((uintptr_t)context->top_block + sizeof(heap_block_t) + context->top_block->size);
	for (; context->used_size + size >= context->size; context->top_block->size += PAGE_SIZE, top += PAGE_SIZE, context->size += PAGE_SIZE) {
		paging_map(context->pd, top);
	}
}

//////////////////////////////////////////////////////////////
//// syscalls
//////////////////////////////////////////////////////////////

static void syscall_allocate(registers_t* regs) {
	size_t size = (size_t)regs->ebx;
	process_t* process = process_get_running();
	if (process == NULL) {
		kpanic("[heap] attempted to call allocate with no running process");
	}
	regs->eax = heap_allocate(&process->heap, size);
}

static void syscall_reallocate(registers_t* regs) {
	process_t* process = process_get_running();
	if (process == NULL) {
		kpanic("[heap] attempted to call reallocate with no running process");
	}
	uintptr_t ptr = (uintptr_t)regs->ebx;
	size_t size = (size_t)regs->ecx;
	regs->eax = heap_reallocate(&process->heap, ptr, size);
}

static void syscall_free(registers_t* regs) {
	uintptr_t ptr = (uintptr_t)regs->ebx;
	process_t* process = process_get_running();
	if (process == NULL) {
		kpanic("[heap] attempted to call free with no running process");
	}
	heap_free(&process->heap, ptr);
}

static void syscall_get_used_size(registers_t* regs) {
	process_t* process = process_get_running();
	if (process == NULL) {
		kpanic("[heap] attempted to call get_used_size with no running process");
	}
	regs->eax = (uint32_t)process->heap.used_size;
}

static void syscall_get_ptr_size(registers_t* regs) {
	process_t* process = process_get_running();
	if(process == NULL) {
		kpanic("[heap] attempted to call get_ptr_size with no running process");
	}
	uintptr_t ptr = (uintptr_t)regs->ebx;
	bool valid;
	heap_block_t* block = get_block(&process->heap, ptr, &valid);
	if(!valid) {
		regs->eax = 0;
	}else {
		regs->eax = block->size;
	}
}

//////////////////////////////////////////////////////////////
//// the implementation
//////////////////////////////////////////////////////////////

void heap_init(void) {
	term_write("[heap] Initializing\n");

	syscall_register(SYSCALL_HEAP_ALLOCATE, syscall_allocate);
	syscall_register(SYSCALL_HEAP_REALLOCATE, syscall_reallocate);
	syscall_register(SYSCALL_HEAP_FREE, syscall_free);
	syscall_register(SYSCALL_HEAP_GET_PTR_SIZE, syscall_get_ptr_size);
	syscall_register(SYSCALL_HEAP_GET_USED_SIZE, syscall_get_used_size);
}

void heap_create(heap_context_t* context, uintptr_t start_address) {
	context->size = PAGE_SIZE;

	context->start = (heap_block_t*)start_address;
	context->first_free = (heap_block_t*)start_address;

	// the first block header takes space
	context->used_size = sizeof(heap_block_t);


	if (context->pd != NULL) {
		paging_map(context->pd, start_address);

		context->start->size = PAGE_SIZE - sizeof(heap_block_t);
	}
	else {
		// if the pd is null it means this is the kernel heap
		// and it will be used by the kernel only
		// we don't need to map anything since it is automatically 
		// identity mapped
		// the size of the kernel heap is 45MB
		context->start->size = 1024 * 1024 * 45 - sizeof(heap_block_t);
	}

	context->start->allocated = false;
	context->start->magic = HEAP_MAGIC;
	context->start->before = NULL;

	context->top_block = (context->start);
}

uintptr_t heap_allocate(heap_context_t* context, size_t size) {
	if (size == 0) return NULL;

	// we start from the first free block
	bool valid = true; // assume the first block is valid
	heap_block_t* block = context->first_free;
	do {

		// if the current page is allocated we had a problem and we need to find the actual first free
		if (block->allocated) {
			block = get_next_free_block(context, block, &valid);
			continue;
		}

		// do we have enough space
		if (block->size >= size) {

			// do we have enough space to divide the block into two blocks?
			if ((block->size - size - sizeof(heap_block_t)) > 0) {
				// divide the memory block
				heap_block_t* newblock = (heap_block_t*)(((uintptr_t)block) + sizeof(heap_block_t) + size);
				newblock->allocated = false;
				newblock->size = block->size - size - sizeof(heap_block_t);
				newblock->magic = HEAP_MAGIC;
				newblock->before = block;

				// the new header takes place from the heap
				context->used_size += sizeof(heap_block_t);

				// set the block size
				block->size = size;

				// set the previous block of the next block, only if valid
				bool nextBlockValid;
				heap_block_t* next = get_next_block(context, newblock, &nextBlockValid);
				if (nextBlockValid) {
					next->before = newblock;
				}

				if (context->first_free == block) {
					// now the first allocated block is the newly created one
					context->first_free = newblock;
				}

				if (newblock > context->top_block) {
					context->top_block = newblock;
				}
			}
			else if (context->first_free == block) {
				bool valid;
				context->first_free = get_next_free_block(context, block, &valid);
				if (!valid) {
					// std::cerr << "heap_allocate: first free block is invalid!\n" << std::endl;
					kpanic("heap_allocate: first free block is invalid!\n");
				}
			}

			// block is now allocated! 
			block->allocated = true;

			// add the size of the block to the used size
			context->used_size += block->size;

			// return the pointer to the data
			return (uintptr_t)(((uintptr_t)block) + sizeof(heap_block_t));
		}

		// this block isn't good, get the next free one
		block = get_next_free_block(context, block, &valid);
	} while (valid);

	// failed to allocate memory, expand the heap
	expand(context, size);

	// retry to allocate
	return heap_allocate(context, size);
}

void heap_free(heap_context_t* context, uintptr_t ptr) {
	if (ptr == NULL) {
		return;
	}

	bool valid;
	heap_block_t* block = get_block(context, ptr, &valid);
	if (!valid) {
		return;
	}

	block->allocated = false;

	// remove the freed size from the used size
	context->used_size -= block->size;

	// merge empty blocks, to save on space
	block = merge_empty_blocks(context, block);

	// is this before the current first fre block
	if (block < context->first_free) {
		// if so this is now the first free block
		context->first_free = block;
	}
}

uintptr_t heap_reallocate(heap_context_t* context, uintptr_t ptr, size_t newSize) {
	if(ptr == NULL) {
		// if no pointer, treat it as a normal allocate
		return heap_allocate(context, newSize);
	}

	bool valid;
	heap_block_t* block = get_block(context, ptr, &valid);
	if(!valid) {
		// Maybe this is not the correct behaviour...
		return heap_allocate(context, newSize);
	}

	size_t copySize = newSize > block->size ? block->size : newSize;

	// OPTIMIZATION: if the newsize is smaller and not that different maybe just allow it stay with
	// 				 the same pointer

	// allocate new space
	uintptr_t newptr = heap_allocate(context, newSize);

	// copy the data to the new space
	memcpy(newptr, ptr, copySize);

	// free the old pointer
	heap_free(context, ptr);

	return newptr;
}
