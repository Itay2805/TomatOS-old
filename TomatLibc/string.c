#include "string.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"

// optimized for 32bit
void* memcpy(void* destptr, const void* srcptr, size_t num) {
    uint8_t* dest = destptr;
    const uint8_t* src = srcptr;

    while(num >= 4) {
        *((uint32_t*)dest) = *((uint32_t*)src);
        num -= 4;
        dest += 4;
        src += 4;
    }

    if(num >= 2) {
        *((uint16_t*)dest) = *((uint16_t*)src);
        num -= 2;
        dest += 2;
        src += 2;
    }

    if(num >= 1) {
        *dest = *src;
    }

    return destptr;
}

// optimized for pointer overlap
void* memmove(void* destptr, const void* srcptr, size_t num) {
    uint8_t* dest = destptr;
    const uint8_t* src = srcptr;
    if((size_t)abs((int)(src - dest)) < num) {
        // pointers overlap, will need to copy to a temp buffer before moving
        uint8_t tmp[num];
        memcpy(tmp, src, num);
        memcpy(dest, tmp, num);        
    }else {
        // no overlap, safe to just copy
        memcpy(dest, src, num);
    }
    return dest;
}

int memcmp(const void* ptr1, const void* ptr2, size_t num) {
    // could be optimized...
    
    const uint8_t* buf1 = ptr1;
    const uint8_t* buf2 = ptr2;
    while(num) {
        if(*buf1 != *buf2) {
            return *buf1 - *buf2; 
        }
        buf1++;
        buf2++;
        num--;
    }

    return 0;
}

void* memchr(const void* ptr, int value, size_t num) {
    const uint8_t* buf = ptr;
    while(num) {
        if(*buf == (uint8_t)value) {
            return (void*)buf;
        }
        buf++;
    }
    return NULL;
}

// optimized for 32bit
void* memset(void* ptr, int value, size_t num) {
    uint32_t val32 = (uint32_t)(((uint8_t)value << 24) | ((uint8_t)value << 16) | ((uint8_t)value << 8) | ((uint8_t)value));   
    uint16_t val16 = (uint16_t)(((uint8_t)value << 8) | ((uint8_t)value));
    uint8_t val8 = (uint8_t)(value);   

    uint8_t* buf = ptr;

    while(num >= 4) {
        *((uint32_t*)buf) = val32;
        num -= 4;
        buf += 4;
    }

    if(num >= 2) {
        *((uint16_t*)buf) = val16;
        num -= 2;
        buf += 2;
    }

    if(num >= 1) {
        *buf = val8;
    }

    return ptr;
}

char* strcpy(char* dest, const char* src) {
    char* ret = dest;
    while(*src) {
        *dest++ = *src++;
    }
    *dest = 0;
    return ret;
}

char* strncpy(char* dest, const char* src, size_t num) {
    char* ret = dest;
    while(num-- && *src) {
        *dest++ = *src++;
    }
    while(num--) {
        *dest++ = 0;
    }
    return ret;
}

char* strcat(char* dest, const char* src) {
    char* ret = dest;
    while(*dest++);
    strcpy(dest, src);
    return ret;
}

char* strncat(char* dest, const char* src, size_t num) {
    char* ret = dest;
    while(*dest++);
    strncpy(dest, src, num);
    return ret;
}

int strcmp(const char* str1, const char* str2) {
    while(*str1++ == *str2++ && *str1 != 0);
    return *str1 - *str2;
}

int strncmp(const char* str1, const char* str2, size_t num) {
    return memcmp(str1, str2, num);
}

char* strchr(const char* str, int character) {
    while(*str) {
        if(*str == character) {
            return (char*)str;
        }
        str++;
    }   
    return NULL;
}

size_t strcspn(const char* str1, const char* str2) {
    size_t span = 0;
    const char* str2start = str2;
    while(*str1) {
        while(*str2) {
            if(*str1 == *str2) {
                return span;
            }
            str2++;
        }
        str2 = str2start;
        span++;
    }
    return span;
}

size_t strspn(const char* str1, const char* str2) {
    size_t count = 0;
    while(*str1) {
        bool found = false;
        while(*str2) {
            if(*str1 == *str2) {
                found = true;
                count++;
                str1++;
                break;
            }
            str2++;
        }
        if(!found) return count;
    }
    return count;
}

char* strstr(const char* str1, const char* str2) {
    while(*str1) {
        if(strcmp(str1, str2)) {
            return (char*)str1;
        }
        str1++;
    }
    return NULL;
}

size_t strlen(const char* str) {
    const char* start = str;
    while(*str++);
    return start - str;
}
