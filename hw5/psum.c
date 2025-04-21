#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>

void clear_array(float *dest, long n) {
    long i;
    for (i = 0; i < n; i ++)
        dest[i] = 0.0f;
}

void psum(const float a[], float p[], long n) {
    long i;
    float prev;
    p[0] = a[0];
    prev = p[0];

    for (i = 1; i < n; i ++) {
        prev = prev + a[i];
        p[i] = prev;
    }
}

void psum_opt1(const float a[], float p[], long n) {
    const int K = 4;
    long i;
    float lastval;

    p[0] = a[0];
    lastval = p[0];

    for (i = 1; i < n - K + 1; i += K) {
        float t0 = lastval + a[i];
        float t1 = lastval + (a[i] + a[i + 1]);
        float t2 = t1 + a[i + 2];
        float t3 = t2 + a[i + 3];

        p[i] = t0;
        p[i + 1] = t1;
        p[i + 2] = t2;
        p[i + 3] = t3;
        lastval = t3;
    }
    
    for (; i < n; i ++) {
        lastval = lastval + a[i];
        p[i] = lastval;
    }
}

void psum_opt2(const float a[], float p[], long n) {
    const int K = 2;
    long i;
    float lastval;

    p[0] = a[0];
    lastval = p[0];

    for (i = 1; i < n - K + 1; i += K) {
        float t0 = lastval + a[i];
        float t1 = lastval + (a[i] + a[i + 1]);

        p[i] = t0;
        p[i + 1] = t1;
        lastval = t1;
    }
    
    for (; i < n; i ++) {
        lastval = lastval + a[i];
        p[i] = lastval;
    }
}

int main() {
    const int length = 1000000;
    float *arr = (float*) malloc(length * sizeof(float));
    float *sum = (float*) malloc(length * sizeof(float));
    unsigned long long start, end;
    double cpe;
    long i;

    // Fill array
    clear_array(sum, length);
    for (i = 0; i < length; ++ i)
        arr[i] = i / 5.0f;

    // psum: no w/r depdendency 
    start = __rdtsc();
    psum(arr, sum, length);
    end = __rdtsc();
    cpe = (double) (end - start) / length;
    printf("psum (no w/r):                  %5.2lf\n", cpe);

    // psum_opt with 4x unrolling
    start = __rdtsc();
    psum_opt1(arr, sum, length);
    end = __rdtsc();
    cpe = (double) (end - start) / length;
    printf("psum optimized (4x unrolling):  %5.2lf\n", cpe);
    for (i = 0; i < length; ++ i) 

    // psum_opt with 2x unrolling
    start = __rdtsc();
    psum_opt2(arr, sum, length);
    end = __rdtsc();
    cpe = (double) (end - start) / length;
    printf("psum optimized (2x unrolling):  %5.2lf\n", cpe);
    for (i = 0; i < length; ++ i) 

    return 0;
}