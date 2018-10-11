#include "stdlib.h"

#include "ctype.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

void* bsearch(const void* key, const void* baseptr, size_t num, size_t size, int (*compar)(const void*,const void*)) {
    uint8_t* base = baseptr;
    size_t l = 0;
    size_t r = num - 1;
    while(l <= r) {
        size_t m = (l + r) / 2;
        int cmp = compar(base + (m * size), key);
        if(cmp < 0) {
            l = m + 1;
        }else if(cmp > 0) {
            r = m - 1;
        }else {
            return base + (m * size);
        }
    }
    return NULL;
}

void qsort (void* base, size_t num, size_t size, int (*compar)(const void*,const void*)) {
    // TODO: I don't really remember how to implement this
}

int atoi(const char* str) {
    return (int)atoll(str);    
}

long atol(const char* str) {
    return (long)atoll(str);
}

long long atoll(const char* str) {
    while(isspace(*str)) {
        str++;
    }
    long long num = 0;
    bool neg = false;
    if(*str == '-') {
        neg = true;
        str++;
    }else if(*str == '+') {
        str++;
    }
    while(isdigit(*str)) {
        num *= 10;
        num += *str - '0';
    }
    if(neg) {
        num = -num;
    }
    return 0;
}

int abs(int n) {
    return n <= 0 ? n : -n;
}

int labs(long int n) {
    return n <= 0 ? n : -n;
}
