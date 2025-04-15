#include <stdlib.h>
#include <stdio.h>
#include <x86intrin.h>

#define IDENT 0.0f
#define OP +
typedef float data_t;

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

int main() {
    const long length = 1000000;
    void (*combines[4])(vec_ptr, data_t*) = {
        combine1, combine2, combine3, combine4
    };
    vec_ptr v;
    data_t dest;
    clock_t start, end;
    double cpe;

    for (int i = 0; i < 4; ++ i) {
        v = new_vec(length);
        dest;
        start = __rdtsc();
        combines[i](v, &dest);
        end = __rdtsc();
        cpe = (double) (end - start) / length;
        printf("combine%d: %5.2lf\n", i + 1, cpe);
    }

    return 0;
}