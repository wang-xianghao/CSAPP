#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <x86intrin.h>

double aprod1(double a[], long n) {
    long i;
    double x, y, z;
    double r = 1;

    for (i = 0; i < n - 2; i += 3) {
        x = a[i]; y = a[i + 1]; z = a[i + 2];
        r = ((r * x) * y) * z;
    }

    for (; i < n; ++ i) {
        r *= a[i];
    }

    return r;
}

double aprod2(double a[], long n) {
    long i;
    double x, y, z;
    double r = 1;

    for (i = 0; i < n - 2; i += 3) {
        x = a[i]; y = a[i + 1]; z = a[i + 2];
        r= (r * (x * y)) * z;
    }

    for (; i < n; ++ i) {
        r *= a[i];
    }

    return r;
}

double aprod3(double a[], long n) {
    long i;
    double x, y, z;
    double r = 1;

    for (i = 0; i < n - 2; i += 3) {
        x = a[i]; y = a[i + 1]; z = a[i + 2];
        r = r * ((x * y) * z);
    }

    for (; i < n; ++ i) {
        r *= a[i];
    }

    return r;
}

double aprod4(double a[], long n) {
    long i;
    double x, y, z;
    double r = 1;

    for (i = 0; i < n - 2; i += 3) {
        x = a[i]; y = a[i + 1]; z = a[i + 2];
        r = r * (x * (y * z));
    }

    for (; i < n; ++ i) {
        r *= a[i];
    }

    return r;
}

double aprod5(double a[], long n) {
    long i;
    double x, y, z;
    double r = 1;

    for (i = 0; i < n - 2; i += 3) {
        x = a[i]; y = a[i + 1]; z = a[i + 2];
        r = (r * x) * (y * z);
    }

    for (; i < n; ++ i) {
        r *= a[i];
    }

    return r;
}

int main() {
    const int NUM_FUNCS = 5;
    const long length = 100000000;
    double (*aprods[])(double [], long) = {
        aprod1, aprod2, aprod3, aprod4, aprod5
    };
    unsigned long long start, end;
    double cpe;
    double *A = (double *) calloc(length, sizeof(double));

    for (int i = 0; i < NUM_FUNCS; ++ i) {

        start = __rdtsc();
        aprods[i](A, length);
        end = __rdtsc();
        cpe = (double) (end - start) / length;
        printf("aprod%d: %5.2lf\n", i + 1, cpe);
    }

    return 0;
}