#ifndef STRING_H
#define STRING_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

    void* memcpy(void* dest, const void* src, size_t num);
    void* memmove(void* dest, const void* src, size_t num);
    int memcmp(const void* ptr1, const void* ptr2, size_t num);
    void* memchr(const void * ptr, int value, size_t num);
    void* memset(void* ptr, int value, size_t num);

    char* strcpy(char* dest, const char* src);
    char* strncpy(char* dest, const char* src, size_t num);
    char* strcat(char* dest, const char* src);
    char* strncat(char* dest, const char* src, size_t num);
    char* strcmp(char* str1, const char* str2);
    char* strncmp(char* str1, const char* str2, size_t num);
    char* strchr (const char* str, int character);
    size_t strcspn(const char* str1, const char* str2);
    // strpbrk
    // strrchr
    // strspn
    // strtok
    size_t strlen(const char* str);


#ifdef __cplusplus
}
#endif
#endif