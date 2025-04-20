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

double poly_opt1(double a[], double x, long degree) {
    const long K = 4;
    long i;
    double y = x * x * x * x;
    double xpwr0 = x;
    double xpwr1 = xpwr0 * x;
    double xpwr2 = xpwr1 * x;
    double xpwr3 = xpwr2 * x;
    double result0 = a[0];
    double result1 = 0.0;
    double result2 = 0.0;
    double result3 = 0.0;
    

    for (i = 1; i <= degree - K + 1; i += K) {
        result0 += a[i] * xpwr0;
        xpwr0 = y * xpwr0;;
        result1 += a[i + 1] * xpwr1;
        xpwr1 = y * xpwr1;
        result2 += a[i + 2] * xpwr2;
        xpwr2 = y * xpwr2;
        result3 += a[i + 3] * xpwr3;
        xpwr3 = y * xpwr3;
    }

    for (; i <= degree; ++ i) {
        result0 += a[i] * xpwr0;
        xpwr0 = x * xpwr0;
    }

    return result0 + result1 + result2 + result3;
}

double poly_opt2(double a[], double x, long degree) {
    const long K = 4;
    long i;
    double y = x * x * x * x;
    double xpwr0 = x;
    double xpwr1 = xpwr0 * x;
    double xpwr2 = xpwr1 * x;
    double xpwr3 = xpwr2 * x;
    double result = a[0];
    

    for (i = 1; i <= degree - K + 1; i += K) {
        result += a[i] * xpwr0 + a[i + 1] * xpwr1 + a[i + 2] * xpwr2 + a[i + 3] * xpwr3;
        xpwr0 = y * xpwr0;;
        xpwr1 = y * xpwr1;
        xpwr2 = y * xpwr2;
        xpwr3 = y * xpwr3;
    }

    for (; i <= degree; ++ i) {
        result += a[i] * xpwr0;
        xpwr0 = x * xpwr0;
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

double polyh_opt(double a[], double x, long degree) {
    double x2 = x * x;
    double x3 = x2 * x;
    double x4 = x3 * x;
    const long K = 4;
    long i;
    double result = a[degree];
    
    for (i = degree - 1; i >= K - 1; i -= 4) {
        result = (a[i-3] + x * a[i-2] + x2 * a[i-1] + x3 * a[i]) + x4 * result;
    }

    for (; i >= 0; -- i)
        result = a[i] + x * result;
        
    return result;
}

int main() {
    const long degree = 100000;
    const int repeats = 100;
    long i;
    double a[degree + 1];
    unsigned long long start, end;
    volatile double result;
    double cpe;

    for (i = 0; i < degree; ++ i)
        a[i] = i / 2.0;

    start = __rdtsc();
    for (i = 0; i < repeats; ++ i)
        result = poly(a, 0.5, degree);
    end = __rdtsc();
    cpe = (double) (end - start) / degree / repeats;
    printf("poly:               %5.2lf\n", cpe);
    // printf("%lf\n", result);

    start = __rdtsc();
    for (i = 0; i < repeats; ++ i)
        result = poly_opt1(a, 0.5, degree);
    end = __rdtsc();
    cpe = (double) (end - start) / degree / repeats;
    printf("poly kxk unroll:    %5.2lf\n", cpe);
    // printf("%lf\n", result);

    start = __rdtsc();
    for (i = 0; i < repeats; ++ i)
        result = poly_opt2(a, 0.5, degree);
    end = __rdtsc();
    cpe = (double) (end - start) / degree / repeats;
    printf("poly kx1a unroll:   %5.2lf\n", cpe);
    // printf("%lf\n", result);

    start = __rdtsc();
    for (i = 0; i < repeats; ++ i)
        result = polyh(a, 0.5, degree);
    end = __rdtsc();
    cpe = (double) (end - start) / degree / repeats;
    printf("polyh:              %5.2lf\n", cpe);
    // printf("%lf\n", result);

    start = __rdtsc();
    for (i = 0; i < repeats; ++ i)
        result = polyh_opt(a, 0.5, degree);
    end = __rdtsc();
    cpe = (double) (end - start) / degree / repeats;
    printf("polyh opt:          %5.2lf\n", cpe);
    // printf("%lf\n", result);

    return 0;
}