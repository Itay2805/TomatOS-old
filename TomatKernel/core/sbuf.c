#include "sbuf.h"

void* buf__grow(const void *buf, size_t new_len, size_t elem_size) {
	assert(buf_cap(buf) <= (SIZE_MAX - 1) / 2);
	size_t new_cap = CLAMP_MIN(2 * buf_cap(buf), MAX(new_len, 16));
	assert(new_len <= new_cap);
	assert(new_cap <= (SIZE_MAX - offsetof(BufHdr, buf)) / elem_size);
	size_t new_size = offsetof(BufHdr, buf) + new_cap * elem_size;
	BufHdr *new_hdr;
	if (buf) {
		new_hdr = realloc(buf__hdr(buf), new_size);
	}
	else {
		new_hdr = malloc(new_size);
		new_hdr->len = 0;
	}
	new_hdr->cap = new_cap;
	return new_hdr->buf;
}
