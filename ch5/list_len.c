#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>

typedef struct ELE {
    struct ELE *next;
    long data;
} list_ele, *list_ptr;

list_ptr new_list(long len) {
    if (len == 0)
        return NULL;

    list_ptr head = (list_ptr) calloc(1, sizeof(list_ele));
    list_ptr node = head;

    while (-- len) {
        node->next = (list_ptr) calloc(1, sizeof(list_ele));
        node = node->next;
    }

    return head;
}

long list_len(list_ptr ls) {
    long len = 0;
    while (ls) {
        len ++;
        ls = ls->next;
    }
    return len;
}

int main(int argc, char** argv) {
    long length = atol(argv[1]);
    list_ptr ls = new_list(length);
    volatile long len;
    unsigned long long start, end;
    double cpe;
    
    start = __rdtsc();
    len = list_len(ls);
    end = __rdtsc();

    cpe = (double) (end -start) / length;
    printf("length: %ld\n", len);
    printf("list_len: %5.2lf\n", cpe);

    return 0;
}