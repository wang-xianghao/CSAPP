#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>

void merge(long src1[], long src2[], long dest[], long n) {
    long i1 = 0;
    long i2 = 0;
    long id = 0;
    while (i1 < n && i2 < n) {
        if (src1[i1] < src2[i2])
            dest[id ++] = src1[i1 ++];
        else
            dest[id ++] = src2[i2 ++];
    }
    while (i1 < n)
        dest[id ++] = src1[i1 ++];
    while (i2 < n)
        dest[id ++] = src2[i2 ++];
}

void merge_cond(long src1[], long src2[], long dest[], long n) {
    long i1 = 0;
    long i2 = 0;
    long id = 0;
    while (i1 < n && i2 < n) {
        long v1 = src1[i1];
        long v2 = src2[i2];
        long take1 = v1 < v2;
        dest[id ++] = take1 ? v1 : v2;
        i1 += take1;
        i2 += (1 - take1);
    }
    while (i1 < n)
        dest[id ++] = src1[i1 ++];
    while (i2 < n)
        dest[id ++] = src2[i2 ++];
}

int main() {
    const long length = 1000000;
    unsigned long long start, end;
    double cpe;
    long *src1 = malloc(length * sizeof(long));
    long *src2 = malloc(length * sizeof(long));
    long *dest = malloc(2 * length * sizeof(long));
    
    // Initialize data
    for (int i = 0; i < length; ++ i) {
        src1[i] = rand();
        src2[i] = rand();
    }

    // Prevent cache effects
    start = __rdtsc();
    merge(src1, src2, dest, length);
    end = __rdtsc();
    cpe = (double) (end - start) / (2 * length);
    printf("merge warmup:   %5.2lf\n", cpe);

    // merge baseline
    start = __rdtsc();
    merge(src1, src2, dest, length);
    end = __rdtsc();
    cpe = (double) (end - start) / (2 * length);
    printf("merge baseline: %5.2lf\n", cpe);

    // merge with conditional move
    start = __rdtsc();
    merge_cond(src1, src2, dest, length);
    end = __rdtsc();
    cpe = (double) (end - start) / (2 * length);
    printf("merge cmov:     %5.2lf\n", cpe);


    return 0;
}