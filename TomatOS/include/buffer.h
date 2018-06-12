#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
#include <stddef.h>

typedef struct buffer_t {
	size_t len;
	size_t cap;
	char buf[0];
} buffer_t;

#define __buffer_hdr(b) ((buffer_t*)((char*)(b) - offsetof(buffer_t, buf)))
#define __buffer_fits(b, n) (buffer_length(b) + (n) <= buffer_capacity(b))
#define buffer_fit(b, n) (__buffer_fits(b, n) ? 0 : ((b) = __buffer_grow((b), buffer_length(b) + (n), sizeof(*(b)))))

#define buffer_length(b) ((b) ? __buffer_hdr(b)->len : 0)
#define buffer_capacity(b) ((b) ? __buffer_hdr(b)->cap : 0)
#define buffer_push(b, x) (buffer_fit(b, 1), b[buffer_length(b)] = (x), __buffer_hdr(b)->len++)
#define buffer_free(b) ((b) ? free(__buffer_hdr(buf)) : 0)

void* __buffer_grow(const void* buf, size_t new_len, size_t elem_size) {
	if (new_len <= buffer_capacity(buf)) {
		return __buffer_hdr(buf)->buf;
	}
	size_t new_cap = (1 + 2 * buffer_capacity(buf)) > new_len ? (1 + 2 * buffer_capacity(buf)) : new_len;
	size_t new_size = offsetof(buffer_t, buf) + new_cap * elem_size;
	buffer_t* newhdr;
	if (buf) {
		newhdr = (buffer_t*)realloc(__buffer_hdr(buf), new_size);
	}
	else {
		newhdr = (buffer_t*)malloc(new_size);
		newhdr->len = 0;
	}
	newhdr->cap = new_cap;
	return newhdr->buf;
}

#endif