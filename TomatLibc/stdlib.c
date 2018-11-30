#include <stdlib.h>

#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

void* bsearch(const void* key, const void* baseptr, size_t num, size_t size, int (*compar)(const void*,const void*)) {
    const uint8_t* base = baseptr;
    size_t l = 0;
    size_t r = num - 1;
    while(l <= r) {
        size_t m = (l + r) / 2;
        if(m == 0) {
            // step size is too small
            return NULL;
        }
        int cmp = compar(base + (m * size), key);
        if(cmp < 0) {
            l = m + 1;
        }else if(cmp > 0) {
            r = m - 1;
        }else {
            return (void*)(base + (m * size));
        }
    }
    return NULL;
}

static size_t partition(void* base, size_t num, size_t size, int (*compar)(const void*,const void*), size_t lo, size_t hi) {
    uint8_t buffer[size];

    void* pivot = (base + (hi * size));
    size_t i = lo;
    for(size_t j = lo; j < hi; j++) {
        if(compar(base + (j * size), pivot) < 0) {
            if(i != j) {
                // swap base[i] with base[j]
                memcpy(buffer, base + (i * size), size);
                memcpy(base + (i * size), base + (j * size), size);
                memcpy(base + (j * size), buffer, size);
            }
            i++;
        }
    }

    // swap base[i] with base[hi]
    memcpy(buffer, base + (i * size), size);
    memcpy(base + (i * size), base + (hi * size), size);
    memcpy(base + (hi * size), buffer, size);
    
    return i;
}

static void quicksort(void* base, size_t num, size_t size, int (*compar)(const void*,const void*), size_t lo, size_t hi) {
    if(lo < hi) {
        size_t p = partition(base, num, size, compar, lo, hi);
        quicksort(base, num, size, compar, lo, p - 1);
        quicksort(base, num, size, compar, p + 1, hi);
    }
}

void qsort (void* base, size_t num, size_t size, int (*compar)(const void*,const void*)) {
    quicksort(base, num, size, compar, 0, num);
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
