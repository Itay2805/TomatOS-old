#include "string.h"
#include "stdint.h"
#include "stdlib.h"

// optimized for 32bit
void* memcpy(void* destptr, const void* srcptr, size_t num) {
    uint8_t* dest = destptr;
    uint8_t* src = srcptr;

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
    uint8_t* src = srcptr;
    if(abs((int)(src - dest)) < num) {
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

    uint8_t* buf1 = ptr1;
    uint8_t* buf2 = ptr2;
    while(num) {
        if(*buf1 != *buf2) {
            return *buf1 - *buf2; 
        }
        buf1++;
        buf2++;
        num--;
    }
}

void* memchr(const void* ptr, int value, size_t num) {
    uint8_t* buf = ptr;
    while(num) {
        if(*buf == (uint8_t)value) {
            return buf;
        }
        buf++;
    }
    return NULL;
}

// optimized for 32bit
void* memset(void* ptr, int value, size_t num) {
    uint32_t val32 = (uint32_t)((value << 24) | (value << 16) | (value << 8) | (value));   
    uint16_t val16 = (uint16_t)((value << 8) | (value));   
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

char* strcmp(char* str1, const char* str2) {
    while(*str1++ == *str2++);
    return *str1 - *str2;
}

char* strncmp(char* str1, const char* str2, size_t num) {
    return memcmp(str1, str2, num);
}

char* strchr (const char* str, int character) {
    while(*str) {
        if(*str == character) {
            return str;
        }
        *str;
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

size_t strlen(const char* str) {
    char* start = str;
    while(*str++);
    return start - str;
}
