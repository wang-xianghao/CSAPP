#include <stdlib.h>
#include <stdio.h>
#include <x86intrin.h>

#define IDENT 1.0
#define OP *
typedef double data_t;

typedef struct
{
    long len;
    data_t *data;
} vec_rec, *vec_ptr;

vec_ptr new_vec(long len) {
    vec_ptr result = (vec_ptr) malloc(sizeof(vec_rec));
    data_t *data = NULL;

    if (!result)
        return NULL;
    result->len = len;
    if (len > 0) {
        data = (data_t *) calloc(len, sizeof(data_t));
        if (!data) {
            free((void *) result);
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

// Raw implementation
void combine1(vec_ptr v, data_t *dest) {
    long i;
    *dest = IDENT;
    for (i = 0; i < vec_length(v); i ++) {
        data_t val;
        get_vec_element(v, i, &val);
        *dest = *dest OP val;
    }
}

// Code motion: extract repeated vec_length
void combine2(vec_ptr v, data_t *dest) {
    long i;
    *dest = IDENT;
    long length = vec_length(v);

    for (i = 0; i < length; i ++) {
        data_t val;
        get_vec_element(v, i, &val);
        *dest = *dest OP val;
    }
}

// Reduce procedure calls
void combine3(vec_ptr v, data_t *dest) {
    long i;
    *dest = IDENT;
    long length = vec_length(v);
    data_t *data = get_vec_start(v);

    for (i = 0; i < length; i ++) {
        *dest = *dest OP data[i];
    }
}

// Eliminate unneeded references
void combine4(vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(v);
    data_t *data = get_vec_start(v);
    data_t acc = IDENT;

    for (i = 0; i < length; i ++) {
        acc = acc OP data[i];
    }

    *dest = acc;
}

// Loop unrolling 2x1
void combine5(vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(v);
    data_t *data = get_vec_start(v);
    data_t acc = IDENT;

    for (i = 0; i < length - 1; i += 2) {
        acc = acc OP data[i];
        acc = acc OP data[i + 1];
    }

    for (; i < length; i ++) {
        acc = acc OP data[i];
    }

    *dest = acc;
}

// Loop unrolling 2x2
void combine6(vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(v);
    data_t *data = get_vec_start(v);
    data_t acc0 = IDENT;
    data_t acc1 = IDENT;

    for (i = 0; i < length - 1; i += 2) {
        acc0 = acc0 OP data[i];
        acc1 = acc1 OP data[i + 1];
    }

    for (; i < length; i ++) {
        acc0 = acc0 OP data[i];
    }

    *dest = acc0 OP acc1;
}

// Reassociation transformation
void combine7(vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(v);
    data_t *data = get_vec_start(v);
    data_t acc = IDENT;

    for (i = 0; i < length - 1; i += 2) {
        acc = acc OP (data[i] OP data[i + 1]);
    }

    for (; i < length; i ++) {
        acc = acc OP data[i];
    }

    *dest = acc;
}

int main() {
    const int NUM_COMBINES = 7;
    const long length = 100000000;
    void (*combines[])(vec_ptr, data_t*) = {
        combine1, combine2, combine3, combine4, combine5,
        combine6, combine7
    };
    vec_ptr v;
    data_t dest;
    unsigned long long start, end;
    double cpe;

    for (int i = 0; i < NUM_COMBINES; ++ i) {
        v = new_vec(length);
        start = __rdtsc();
        combines[i](v, &dest);
        end = __rdtsc();
        cpe = (double) (end - start) / length;
        printf("combine%d: %5.2lf\n", i + 1, cpe);
    }

    return 0;
}