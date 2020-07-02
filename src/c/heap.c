#include "heap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SWAP(u, v)\
    struct node_t _t = *(u);\
    *(u) = *(v);\
    *(v) = _t;

void make_heap(struct heap_t* h, size_t cap)
{
    h->capacity = cap;
    h->size = 0;
    h->data = (struct node_t*)malloc(cap * sizeof(struct node_t));
}

void heapify(struct heap_t* h, int32_t i)
{
    uint32_t largest, lc, rc;
    while (i < h->size)
    {
        lc = (i << 1) + 1;
        rc = (i << 1) + 2;
        largest = i;
        if (lc < h->size && h->data[largest].key < h->data[lc].key)
            largest = lc;
        if (rc < h->size && h->data[largest].key < h->data[rc].key)
            largest = rc;
        if (largest == i) break;
        SWAP(&h->data[largest], &h->data[i]);
        i = largest;
    }
}

void insert(struct heap_t* h, int32_t key, void* val)
{
    if (h->size + 1 == h->capacity) return;
    h->data[h->size] = (struct node_t){.key = key, .value = val};
    int32_t curr = (h->size - 1) >> 1;
    ++h->size;
    while (curr >= 0)
    {
        heapify(h, curr);
        curr = (curr - 1) >> 1;
    }
}

void extract(struct heap_t* h, void** val)
{
    if (h->size == 0) {
        *val = NULL;
        return;
    }

    *val = h->data[0].value;
    h->data[0] = h->data[h->size];
    h->data[h->size] = (struct node_t){ .key = -1, .value = NULL };
    heapify(h, 0);
    --h->size;
}


void top(struct heap_t* h, void** val)
{
    if (h->size == 0) {
        *val = NULL;
        return;
    }
    *val = h->data[0].value;
}
