#ifndef LIBC_STRING_H
#define LIBC_STRING_H

#include "stdint.h"
#include "stdbool.h"

char* itoa(int32_t n, char* str, int8_t radix);

char* strrev(char *str);
int strcmp(const char* src, const char* dst);
char* strcpy(char* dst, const char* src);
size_t strlen(const char* str);

int memcmp(const void* src, const void* dst, size_t n);
void* memcpy(void* dst, const void* src, size_t length);
void* memmove(void* dst, void* src, size_t length);
void* memset(void* dst, uint8_t val, size_t length);

#endif