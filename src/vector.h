//
// Created by brych on 16/01/2021.
//

#ifndef _RTS_VECTOR_H
#define _RTS_VECTOR_H

#include <stdio.h>
#include <stdbool.h>

typedef struct {
    void* buf;
    size_t type_size;
    size_t length;
    size_t capacity;
} Vector;

#define VEC_DEFAULT_CAPACITY 4

#define GENERATE_VECTOR_DEFINITION(type)           \
Vector_##type vec_##type##_new() {                           \
    Vector_##type res = {.inner=vec_new(sizeof(type))}; \
    return res;\
}                                                  \
Vector_##type vec_##type##_with_capacity(size_t capacity) {  \
    Vector_##type res = {.inner=vec_with_capacity(sizeof(type), capacity)}; \
    return res;                                                  \
}                                                   \
void vec_##type##_push(Vector_##type vec, type item) {               \
    vec_push(vec.inner, &item);                                                \
}                                                   \
type vec_##type##_get(Vector_##type vec, size_t index) {             \
    type el = *(type *) vec_get(vec.inner, index);  \
    return el;\
}                                                   \
void vec_##type##_clear(Vector_##type vec) {        \
    vec_clear(vec.inner);                                                    \
}                                                  \
type * vec_##type##_get_ptr(Vector_##type vec, size_t index) {       \
    return (type * ) vec_get(vec.inner, index);                                               \
}                                                  \
void vec_##type##_push_zero(Vector_##type vec) {   \
    vec_push_zero(vec.inner);                                                 \
}                                                  \
void vec_##type##_destroy(Vector_##type vec) {     \
    vec_destroy(vec.inner);                                                 \
}
#define GENERATE_VECTOR_DECLARATION(type) typedef struct { \
    Vector* inner;                                                   \
} Vector_##type ;                                   \
                                                    \
Vector_##type vec_##type##_new() ;                         \
Vector_##type vec_##type##_with_capacity(size_t capacity);                                                           \
void vec_##type##_push(Vector_##type vec, type item) ;               \
type vec_##type##_get(Vector_##type vec, size_t index) ;             \
void vec_##type##_clear(Vector_##type vec) ;               \
type * vec_##type##_get_ptr(Vector_##type vec, size_t index);        \
void vec_##type##_push_zero(Vector_##type vec);            \
void vec_##type##_destroy(Vector_##type vec);
#define VEC_LEN(vec) vec.inner->length

// generate useful vector types
GENERATE_VECTOR_DECLARATION(int)
GENERATE_VECTOR_DECLARATION(bool)

Vector* vec_new(size_t type_size);

Vector* vec_with_capacity(size_t type_size, size_t capacity);

void vec_push(Vector* vec, void* item);

void vec_push_zero(Vector* vec);

void* vec_get(Vector* vec, size_t index);

void vec_destroy(Vector* vec);

void vec_clear(Vector* vec);


#endif //_RTS_VECTOR_H
