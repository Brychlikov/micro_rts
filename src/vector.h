//
// Created by brych on 16/01/2021.
//

#ifndef _RTS_VECTOR_H
#define _RTS_VECTOR_H

#include <stdio.h>

typedef struct {
    void** buf;
    size_t type_size;
    size_t length;
    size_t capacity;
} Vector;

#define VEC_DEFAULT_CAPACITY 4

Vector vec_new(size_t type_size);

Vector vec_with_capacity(size_t type_size, size_t capacity);

void vec_push(Vector* vec, void* item);

void* vec_get(Vector* vec, size_t index);

#endif //_RTS_VECTOR_H
