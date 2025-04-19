#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>

typedef unsigned long pack_t;
typedef unsigned char byte;

void *basic_memset(void *s, int c, size_t n) {
    size_t cnt = 0;
    unsigned char *schar = (unsigned char*) s;
    while (cnt < n) {
        *schar++ = (unsigned char) c;
        cnt ++;
    }
    return s;
}

void *pack_memset(void *s, int c, size_t n) {
    size_t cnt = 0;
    unsigned long K = sizeof(pack_t);
    byte *sp = (byte *) s;
    byte *aligned_begin = (byte*) (((unsigned long) sp + K - 1) & ~(K - 1)); 
    pack_t data;

    for (byte *p = (byte*) &data; p < (byte*) &data + K; ++ p) {
        *p = (byte) c;
    }

    // handle unaligned head
    while (sp < aligned_begin) {
        *sp++ = (byte) c;
        cnt ++;
    }

    // handle aligned middle
    for (; cnt < n; cnt += K) {
        *(pack_t *) sp = data;
        sp += K; 
    }

    // handle unaligned tail
    for (; cnt < n; ++ cnt) {
        *sp++ = (byte) c;
    }

    return s;
}

int main() {
    const int bytes = 10000001;
    char *data = (char*) malloc(bytes + 1);
    unsigned long long start, end;
    double cpe;
    int i, repeats = 1;

    // Basic memset
    start = __rdtsc();
    for (i = 0; i < repeats; ++ i)
        basic_memset(data + 1, 2, bytes);
    end = __rdtsc();
    cpe = (double) (end - start) / (bytes * repeats);
    printf("basic_memset:   %5.2lf\n", cpe);

    // Packed memset
    start = __rdtsc();
    for (i = 0; i < repeats; ++ i)
        pack_memset(data + 1, 'a', bytes);
    end = __rdtsc();
    cpe = (double) (end - start) / (bytes * repeats);
    printf("pack_memset:   %5.2lf\n", cpe);

    return 0;
}