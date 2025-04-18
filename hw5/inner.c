#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>

typedef double data_t;

typedef struct
{
    long len;
    data_t *data;
} vec_rec, *vec_ptr;

vec_ptr new_vec(long len) {
    vec_ptr result = (vec_ptr)malloc(sizeof(vec_rec));
    data_t *data = NULL;

    if (!result)
        return NULL;
    result->len = len;
    if (len > 0) {
        data = (data_t *)calloc(len, sizeof(data_t));
        if (!data) {
            free((void *)result);
            return NULL;
        }
    }

    result->data = data;
    return result;
}

int get_vec_element(vec_ptr v, long index, data_t *dest) {
    if (index < 0 || index >= v->len)
        return 0;
    *dest = v->data[index];
    return 1;
}

int vec_length(vec_ptr v) {
    return v->len;
}

data_t *get_vec_start(vec_ptr v) {
    return v->data;
}

void inner1(vec_ptr u, vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(u);
    data_t *u_data = u->data;
    data_t *v_data = v->data;
    data_t sum = (data_t)0;

    for (i = 0; i < length; ++i) {
        sum = sum + u_data[i] * v_data[i];
    }
    *dest = sum;
}

void inner2(vec_ptr u, vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(u);
    data_t *u_data = u->data;
    data_t *v_data = v->data;
    data_t sum = (data_t)0;

    for (i = 0; i < length - 5; i += 5) {
        sum = sum + u_data[i] * v_data[i];
        sum = sum + u_data[i + 1] * v_data[i + 1];
        sum = sum + u_data[i + 2] * v_data[i + 2];
        sum = sum + u_data[i + 3] * v_data[i + 3];
        sum = sum + u_data[i + 4] * v_data[i + 4];
        sum = sum + u_data[i + 5] * v_data[i + 5];
    }

    for (; i < length; ++i) {
        sum = sum + u_data[i] * v_data[i];
    }

    *dest = sum;
}

void inner3(vec_ptr u, vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(u);
    data_t *u_data = u->data;
    data_t *v_data = v->data;
    data_t sum0 = (data_t)0;
    data_t sum1 = (data_t)0;
    data_t sum2 = (data_t)0;
    data_t sum3 = (data_t)0;
    data_t sum4 = (data_t)0;
    data_t sum5 = (data_t)0;

    for (i = 0; i < length - 5; i += 5) {
        sum0 = sum0 + u_data[i] * v_data[i];
        sum1 = sum1 + u_data[i + 1] * v_data[i + 1];
        sum2 = sum2 + u_data[i + 2] * v_data[i + 2];
        sum3 = sum3 + u_data[i + 3] * v_data[i + 3];
        sum4 = sum4 + u_data[i + 4] * v_data[i + 4];
        sum5 = sum5 + u_data[i + 5] * v_data[i + 5];
    }

    for (; i < length; ++i) {
        sum1 = sum1 + u_data[i] * v_data[i];
    }

    *dest = sum0 + sum1 + sum2 + sum3 + sum4 + sum5;
}

void inner4(vec_ptr u, vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(u);
    data_t *u_data = u->data;
    data_t *v_data = v->data;
    data_t sum = (data_t)0;

    for (i = 0; i < length - 5; i += 5) {
        sum += u_data[i] * v_data[i] + u_data[i + 1] * v_data[i + 1] + u_data[i + 2] * v_data[i + 2] +
               u_data[i + 3] * v_data[i + 3] + u_data[i + 4] * v_data[i + 4] + u_data[i + 5] * v_data[i + 5];
    }

    for (; i < length; ++i) {
        sum = sum + u_data[i] * v_data[i];
    }

    *dest = sum;
}

int main() {
    const long length = 100000000;
    vec_ptr u, v;
    data_t dest;
    unsigned long long start, end;
    double cpe;

    u = new_vec(length);
    v = new_vec(length);

    // inner1 - baseline
    start = __rdtsc();
    inner1(u, v, &dest);
    end = __rdtsc();
    cpe = (double)(end - start) / length;
    printf("inner1 (baseline):          %5.2lf\n", cpe);

    // inner2 - 6x1 unrolling
    start = __rdtsc();
    inner2(u, v, &dest);
    end = __rdtsc();
    cpe = (double)(end - start) / length;
    printf("inner2 (6x1 unrolling):     %5.2lf\n", cpe);

    // inner3 - 6x6 unrolling
    start = __rdtsc();
    inner3(u, v, &dest);
    end = __rdtsc();
    cpe = (double)(end - start) / length;
    printf("inner3 (6x6 unrolling):     %5.2lf\n", cpe);

    // inner4 - 6x1a unrolling
    start = __rdtsc();
    inner4(u, v, &dest);
    end = __rdtsc();
    cpe = (double)(end - start) / length;
    printf("inner3 (6x1a unrolling):    %5.2lf\n", cpe);

    return 0;
}