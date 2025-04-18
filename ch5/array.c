#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>

void clear_array(long *dest, long n) {
    long i;
    for (i = 0; i < n; i ++)
        dest[i] = 0;
}

void write_read(long *src, long *dst, long n) {
    long cnt = n;
    long val = 0;

    while (cnt) {
        *dst = val;
        val = (*src) + 1;
        cnt --;
    }
}

int main() {
    const int length = 1000000;
    long *arr = (long *) malloc(length * sizeof(long));
    unsigned long long start, end;
    double cpe;

    // Warmup
    clear_array(arr, length);

    // Clear array - CPE is store issue cycle
    start = __rdtsc();
    clear_array(arr, length);
    end = __rdtsc();
    cpe = (double) (end - start) / length;
    printf("clear_array:                    %5.2lf\n", cpe);

    const int n = 1000000;
    // Write-read - no dependency
    start = __rdtsc();
    write_read(&arr[0], &arr[1], n);
    end = __rdtsc();
    cpe = (double) (end - start) / n;
    printf("write_read (no dependency):     %5.2lf\n", cpe);

    // Write-read - write/read dependency
    // It may be noticed that it achieves the same CPE as the one with no dependency
    // This is becuase modern CPU may be equipped with memory dependency prediction.
    start = __rdtsc();
    write_read(&arr[2], &arr[2], n);
    end = __rdtsc();
    cpe = (double) (end - start) / n;
    printf("write_read (w/r dependency):    %5.2lf\n", cpe);

    return 0;
}