#include "ports.h"

unsigned char port_byte_in (unsigned short port) {
    unsigned char result;
#ifndef VISUAL_STUDIO
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
#else 
	result = 0;
#endif
    return result;
}

void port_byte_out (unsigned short port, unsigned char data) {
#ifndef VISUAL_STUDIO
	__asm__("out %%al, %%dx" : : "a" (data), "d" (port));
#endif
}

unsigned short port_word_in (unsigned short port) {
    unsigned short result;
#ifndef VISUAL_STUDIO
	__asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
#else 
	result = 0;
#endif
	return result;
}

void port_word_out (unsigned short port, unsigned short data) {
#ifndef VISUAL_STUDIO
	__asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
#endif
}