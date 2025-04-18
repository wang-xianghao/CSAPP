#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>

void clear_array(long *dest, long n) {
    long i;
    for (i = 0; i < n; i ++)
        dest[i] = 0;
}

void copy_array(long *src, long *dest, long n) {
    long i;
    for (i = 0; i < n; ++ i)
        dest[i] = src[i];
}

int main() {
    const int length = 1000000;
    long *arr = (long *) malloc(length * sizeof(long));
    unsigned long long start, end;
    double cpe;
    long i;

    // cache array
    clear_array(arr, length);

    const int repeats = 100000;
    // copy a+1 to a
    start = __rdtsc();
    for (i = 0; i < repeats; ++ i)
        copy_array(arr + 1, arr, 999);
    end = __rdtsc();
    cpe = (double) (end - start) / (repeats * 999);
    printf("copy_array (a+1 -> a): %5.2lf\n", cpe);

    // copy array a to a+1
    start = __rdtsc();
    for (i = 0; i < repeats; ++ i)
        copy_array(arr, arr + 1, 999);
    end = __rdtsc();
    cpe = (double) (end - start) / (repeats * 999);
    printf("copy_array (a -> a + 1): %5.2lf\n", cpe);

    // copy array a to a
    start = __rdtsc();
    for (i = 0; i < repeats; ++ i)
        copy_array(arr, arr, 999);
    end = __rdtsc();
    cpe = (double) (end - start) / (repeats * 999);
    printf("copy_array (a -> a + 1): %5.2lf\n", cpe);

    return 0;
}