#include <stdio.h>
#include <x86intrin.h>

double poly(double a[], double x, long degree) {
    long i;
    double result = a[0];
    double xpwr = x;
    for (i = 1; i <= degree; ++ i) {
        result += a[i] * xpwr;
        xpwr = x * xpwr;
    }
    return result;
}

double polyh(double a[], double x, long degree) {
    long i;
    double result = a[degree];
    for (i = degree - 1; i >= 0; -- i)
        result = a[i] + x * result;
    return result;
}

int main() {
    const long degree = 10000;
    long i;
    double a[degree] = {0.0f};
    unsigned long long start, end;
    volatile double result;
    double cpe;

    start = __rdtsc();
    result = poly(a, 3.0, degree);
    end = __rdtsc();
    cpe = (double) (end - start) / degree;
    printf("poly CPE: %5.2lf\n", cpe);

    start = __rdtsc();
    result = polyh(a, 3.0, degree);
    end = __rdtsc();
    cpe = (double) (end - start) / degree;
    printf("polyh CPE: %5.2lf\n", cpe);

    return 0;
}