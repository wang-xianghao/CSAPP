#include <stdio.h>
#include <stdlib.h>
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

// Raw implementation
void combine1(vec_ptr v, data_t *dest) {
    long i;
    *dest = IDENT;
    for (i = 0; i < vec_length(v); i++) {
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

    for (i = 0; i < length; i++) {
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

    for (i = 0; i < length; i++) {
        *dest = *dest OP data[i];
    }
}

// Eliminate unneeded references
void combine4(vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(v);
    data_t *data = get_vec_start(v);
    data_t acc = IDENT;

    for (i = 0; i < length; i++) {
        acc = acc OP data[i];
    }

    *dest = acc;
}

// Loop unrolling 8x1
void combine5(vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(v);
    data_t *data = get_vec_start(v);
    data_t acc = IDENT;

    for (i = 0; i < length - 7; i += 8) {
        acc = acc OP data[i];
        acc = acc OP data[i + 1];
        acc = acc OP data[i + 2];
        acc = acc OP data[i + 3];
        acc = acc OP data[i + 4];
        acc = acc OP data[i + 5];
        acc = acc OP data[i + 6];
        acc = acc OP data[i + 7];
    }

    for (; i < length; i++) {
        acc = acc OP data[i];
    }

    *dest = acc;
}

// Loop unrolling 8x8
void combine6(vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(v);
    data_t *data = get_vec_start(v);
    data_t acc0 = IDENT;
    data_t acc1 = IDENT;
    data_t acc2 = IDENT;
    data_t acc3 = IDENT;
    data_t acc4 = IDENT;
    data_t acc5 = IDENT;
    data_t acc6 = IDENT;
    data_t acc7 = IDENT;

    for (i = 0; i < length - 7; i += 8) {
        acc0 = acc0 OP data[i];
        acc1 = acc1 OP data[i + 1];
        acc2 = acc2 OP data[i + 2];
        acc3 = acc3 OP data[i + 3];
        acc4 = acc4 OP data[i + 4];
        acc5 = acc5 OP data[i + 5];
        acc6 = acc6 OP data[i + 6];
        acc7 = acc7 OP data[i + 7];
    }

    for (; i < length; i++) {
        acc0 = acc0 OP data[i];
    }

    *dest = acc0 OP acc1 OP acc2 OP acc3 OP acc4 OP acc5 OP acc6 OP acc7;
}

// Reassociation transformation with 2x1 unrolling
void combine7(vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(v);
    data_t *data = get_vec_start(v);
    data_t acc = IDENT;

    for (i = 0; i < length - 1; i += 2) {
        acc = acc OP(data[i] OP data[i + 1]);
    }

    for (; i < length; i++) {
        acc = acc OP data[i];
    }

    *dest = acc;
}

// unrolling 20x20 (register spilling)
void combine8(vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(v);
    data_t *data = get_vec_start(v);
    data_t acc00 = IDENT;
    data_t acc01 = IDENT;
    data_t acc02 = IDENT;
    data_t acc03 = IDENT;
    data_t acc04 = IDENT;
    data_t acc05 = IDENT;
    data_t acc06 = IDENT;
    data_t acc07 = IDENT;
    data_t acc08 = IDENT;
    data_t acc09 = IDENT;
    data_t acc10 = IDENT;
    data_t acc11 = IDENT;
    data_t acc12 = IDENT;
    data_t acc13 = IDENT;
    data_t acc14 = IDENT;
    data_t acc15 = IDENT;
    data_t acc16 = IDENT;
    data_t acc17 = IDENT;
    data_t acc18 = IDENT;
    data_t acc19 = IDENT;

    for (i = 0; i < length - 19; i += 20) {
        acc00 = acc00 OP data[i];
        acc01 = acc01 OP data[i + 1];
        acc02 = acc02 OP data[i + 2];
        acc03 = acc03 OP data[i + 3];
        acc04 = acc04 OP data[i + 4];
        acc05 = acc05 OP data[i + 5];
        acc06 = acc06 OP data[i + 6];
        acc07 = acc07 OP data[i + 7];
        acc08 = acc08 OP data[i + 8];
        acc09 = acc09 OP data[i + 9];
        acc10 = acc10 OP data[i + 10];
        acc11 = acc11 OP data[i + 11];
        acc12 = acc12 OP data[i + 12];
        acc13 = acc13 OP data[i + 13];
        acc14 = acc14 OP data[i + 14];
        acc15 = acc15 OP data[i + 15];
        acc16 = acc16 OP data[i + 16];
        acc17 = acc17 OP data[i + 17];
        acc18 = acc18 OP data[i + 18];
        acc19 = acc19 OP data[i + 19];
    }

    for (; i < length; i++) {
        acc00 = acc00 OP data[i];
    }

    *dest = acc00 OP acc01 OP acc02 OP acc03 OP acc04 OP acc05 OP acc06 OP
            acc07 OP acc08 OP acc09 OP acc10 OP acc11 OP acc12 OP acc13 OP
            acc14 OP acc15 OP acc16 OP acc17 OP acc18 OP acc19;
}

int main() {
    const int NUM_COMBINES = 8;
    const long length = 100000000;
    void (*combines[])(vec_ptr, data_t *) = {
        combine1, combine2, combine3, combine4, combine5,
        combine6, combine7, combine8};
    char *descrip[] = {
        "baseline", "code motion", "avoid calls",
        "reduce references", "unrolling 8x1",
        "unrolling 8x8", "reassociation with 2x1 unrolling",
        "unrolling 20x20 (register spilling)"};
    vec_ptr v;
    data_t dest;
    unsigned long long start, end;
    double cpe;

    for (int i = 0; i < NUM_COMBINES; ++i) {
        v = new_vec(length);
        start = __rdtsc();
        combines[i](v, &dest);
        end = __rdtsc();
        cpe = (double)(end - start) / length;
        printf("combine%d: %5.2lf; %s\n", i + 1, cpe, descrip[i]);
    }

    return 0;
}