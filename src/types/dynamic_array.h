#ifndef _H_MYLANG_TYPES_DYNAMIC_ARRAY
#define _H_MYLANG_TYPES_DYNAMIC_ARRAY

#include <stdint.h>

typedef struct darray {
    uint64_t capacity;
    uint64_t length;
    uint64_t stride;
    void* elements;
} darray;

void darray_create(darray* array, uint64_t stride, uint64_t capacity);
void darray_destroy(darray* array);

void darray_resize(darray* array, uint64_t capacity);

void darray_push(darray* array, void* data);
void darray_pop(darray* array, void* dest);

void darray_push_at(darray* array, uint64_t index, void* data);
void darray_pop_at(darray* array, uint64_t index, void* dest);

void darray_at(darray* array, uint64_t index, void* dest);
void darray_replace(darray* array, uint64_t index, void* data);

#endif