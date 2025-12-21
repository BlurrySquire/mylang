#include "dynamic_array.h"

#include <stdlib.h>
#include <string.h>

#define GROWTH_FACTOR 2

void darray_create(darray* array, uint64_t stride, uint64_t capacity) {
    array->capacity = capacity;
	array->length = 0;
    array->stride = stride;
	array->elements = NULL;
    
	darray_resize(array, array->capacity);
}

void darray_destroy(darray* array) {
    free(array->elements);
}

void darray_resize(darray* array, uint64_t capacity) {
    array->elements = realloc(array->elements, capacity * array->stride);
	array->capacity = capacity;
}

void darray_push(darray* array, void* data) {
    if (array->length + 1 > array->capacity) {
		if (array->capacity == 0) {
			darray_resize(array, 1);
		}
		else {
			darray_resize(array, array->capacity * GROWTH_FACTOR);
		}
    }
    
    void* dest = (char*)array->elements + (array->length * array->stride);
    memcpy(dest, data, array->stride);

    array->length++;
}

void darray_pop(darray* array, void* dest) {
    void* source = (char*)array->elements + ((array->length - 1) * array->stride);

    if (dest != NULL) {
        memcpy(dest, source, array->stride);
    }

    array->length--;
}

void darray_push_at(darray* array, uint64_t index, void* data) {
    if (index > array->length) {
        return;
    }

    if (array->length >= array->capacity) {
        darray_resize(array, array->capacity * GROWTH_FACTOR);
    }

    void* mid = array->elements + (index * array->stride);
    
    memmove((char*)mid + array->stride, mid, (array->length - index) * array->stride);
    memcpy(mid, data, array->stride);

    array->length++;
}

void darray_pop_at(darray* array, uint64_t index, void* dest) {
    if (index >= array->length) {
        return;
    }

    void* mid = array->elements + (index * array->stride);
    void* next = (char*)mid + array->stride;

    if (dest != NULL) {
        memcpy(dest, mid, array->stride);
    }
    memmove(mid, next, (array->length - index - 1) * array->stride);

    array->length--;
}

void darray_at(darray* array, uint64_t index, void* dest) {
    void* source = array->elements + (index * array->stride);

    memcpy(dest, source, array->stride);
}

void darray_replace(darray* array, uint64_t index, void* data) {
    if (index >= array->length) {
        return;
    }

    void* dest = array->elements + (index * array->stride);
    memcpy(dest, data, array->stride);
}