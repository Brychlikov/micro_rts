//
// Created by brych on 16/01/2021.
//
#include "vector.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// generate useful vector types
GENERATE_VECTOR_DEFINITION(int)

Vector* vec_new(size_t type_size) {
    return vec_with_capacity(type_size, VEC_DEFAULT_CAPACITY);
}

Vector* vec_with_capacity(size_t type_size, size_t capacity) {
    Vector res = {
            .buf=malloc(type_size * capacity),
            .type_size=type_size,
            .length=0,
            .capacity=capacity
    };
    Vector* ptr = malloc(sizeof(Vector));
    assert(ptr != NULL);
    assert(res.buf != NULL);
    *ptr = res;
    return ptr;
}

void vec_push(Vector* vec, void* item) {
    if(vec->length + 1 > vec->capacity) {
        vec->buf = realloc(vec->buf, vec->type_size * vec->capacity * 2);
        assert(vec->buf != NULL);
    }
    memcpy(vec->buf + vec->type_size * vec->length, item, vec->type_size);
    vec->length++;
}

void* vec_get(Vector* vec, size_t index) {
    assert(index < vec->length);
    return vec->buf + vec->type_size * index;
}

void vec_clear(Vector *vec) {
    // clear vec without releasing its buffer
    vec->length = 0;
}

void vec_destroy(Vector *vec) {
    free(vec->buf);
    free(vec);
}

