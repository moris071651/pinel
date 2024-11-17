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
