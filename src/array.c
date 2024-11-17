#include "pinel/array.h"

#include <stddef.h>
#include <stdint.h>

struct pinel_array_t {
    uint8_t* array;
    size_t length;
    size_t capacity;
    size_t elem_size;
    pinel_cmp_t cmp;
    pinel_dealloc_t dealloc;
};

pinel_array_t* pinel_array_create(size_t elem_size, pinel_dealloc_t dealloc, pinel_cmp_t cmp) {
    pinel_array_t* self = malloc(sizeof(pinel_array_t));
    if (!self) {
        return NULL;
    }

    self->array = NULL;

    self->length = 0;
    self->capacity = 0;

    self->cmp = cmp;
    self->dealloc = dealloc;
    self->elem_size = elem_size;

    return self;
}

void pinel_array_clear(pinel_array_t* self) {
    if (!self->array) {
        return;
    }

    if (self->dealloc) {
        for (size_t i = 0; i < self->length; i++) {
            self->dealloc(self->array + self->elem_size * i);
        }
    }

    free(self->array);
    self->array = NULL;

    self->length = 0;
    self->capacity = 0;
}

void pinel_array_destroy(pinel_array_t* self) {
    pinel_array_clear(self);
    free(self);
}

size_t pinel_array_length(pinel_array_t* self) {
    return self->length;
}

size_t pinel_array_capacity(pinel_array_t* self) {
    return self->capacity;
}

void* pinel_array_get(pinel_array_t* self, size_t index) {
    if (index >= self->length) {
        return NULL;
    }

    return (self->array + self->elem_size * index);
}
