#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>

void clear_array(float *dest, long n) {
    long i;
    for (i = 0; i < n; i ++)
        dest[i] = 0.0f;
}

void psum1(float a[], float p[], long n) {
    long i;
    p[i] = a[i];
    for (i = 1; i < n; i ++)
        p[i] = p[i - 1] + a[i];
}

void psum2(float a[], float p[], long n) {
    long i;
    p[i] = a[i];
    for (i = 1; i < n - 1; i += 2) {
        float mid_val = p[i - 1] + a[i];
        p[i] = mid_val;
        p[i + 1] = mid_val + a[i + 1];
    }
    
    if (i < n)
        p[i] = p[i - 1] + a[i];
}

void psum3(float a[], float p[], long n) {
    long i;
    float prev;
    p[i] = a[i];
    prev = p[i];

    for (i = 1; i < n; i ++) {
        prev = prev + a[i];
        p[i] = prev;
    }
}


int main() {
    const int length = 100000000;
    float *arr = (float*) malloc(length * sizeof(float));
    float *sum = (float*) malloc(length * sizeof(float));
    unsigned long long start, end;
    double cpe;
    long i;

    // cache array
    clear_array(arr, length);
    clear_array(sum, length);

    // psum1: baseline
    start = __rdtsc();
    psum1(arr, sum, length);
    end = __rdtsc();
    cpe = (double) (end - start) / length;
    printf("psum1 (baseline):                   %5.2lf\n", cpe);

    // psum2: unrolling
    start = __rdtsc();
    psum2(arr, sum, length);
    end = __rdtsc();
    cpe = (double) (end - start) / length;
    printf("psum2 (unrolling):                  %5.2lf\n", cpe);

    // psum3: avoid w/r dependency
    start = __rdtsc();
    psum3(arr, sum, length);
    end = __rdtsc();
    cpe = (double) (end - start) / length;
    printf("psum3 (avoid w/r dependency):       %5.2lf\n", cpe);

    return 0;
}