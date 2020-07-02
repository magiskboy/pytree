#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stddef.h>

struct node_t {
    int32_t key;
    void* value;
};

struct heap_t {
    struct node_t* data;
    size_t capacity;
    size_t size;
};

void make_heap(struct heap_t*, size_t);

void heapify(struct heap_t*, int32_t);

void insert(struct heap_t*, int32_t, void*);

void extract(struct heap_t*, void**);

void top(struct heap_t*, void**);

#endif
