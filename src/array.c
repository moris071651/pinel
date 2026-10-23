#include "pinel/array.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>

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

int pinel_array_push(pinel_array_t* self, void* elem, size_t index) {
    if (!self) {
        return 1;
    }

    if (index > self->length) {
        return 1;
    }

    if (self->length == self->capacity) {
        size_t capacity = (self->capacity == 0) ? 1 : self->capacity * 2;

        uint8_t* array = realloc(self->array, capacity * self->elem_size);
        if (!array) {
            return 1;
        }

        self->array = array;
        self->capacity = capacity;
    }

    memmove(self->array + (index + 1) * self->elem_size, 
            self->array + index * self->elem_size, 
            (self->length - index) * self->elem_size);

    memcpy(self->array + index * self->elem_size, elem, self->elem_size);

    self->length++;

    return 0;
}

int pinel_array_pushF(pinel_array_t* self, void* elem) {
    return pinel_array_push(self, elem, 0);
}

int pinel_array_pushB(pinel_array_t* self, void* elem) {
    if (!self) {
        return 1;
    }

    return pinel_array_push(self, elem, self->length);
}

int pinel_array_pop(pinel_array_t* self, size_t index, void* elem) {
    if (!self) {
        return 1;
    }

    if (index >= self->length) {
        return 1;
    }

    void* slot = self->array + index * self->elem_size;

    if (elem) {
        memcpy(elem, slot, self->elem_size);
    }
    else if (self->dealloc) {
        self->dealloc(slot);
    }

    memmove(slot,
            slot + self->elem_size,
            (self->length - index - 1) * self->elem_size);

    self->length--;

    return 0;
}

int pinel_array_popF(pinel_array_t* self, void* elem) {
    return pinel_array_pop(self, 0, elem);
}

int pinel_array_popB(pinel_array_t* self, void* elem) {
    if (!self) {
        return 1;
    }

    return pinel_array_pop(self, self->length - 1, elem);
}

void pinel_array_clear(pinel_array_t* self) {
    if (!self) {
        return;
    }

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

void pinel_array_destroy(pinel_array_t** self) {
    if (!self) {
        return;
    }

    if (!*self) {
        return;
    }

    pinel_array_clear(*self);
    free(*self);

    *self = NULL;
}

size_t pinel_array_length(pinel_array_t* self) {
    if (!self) {
        return 0;
    }

    return self->length;
}

size_t pinel_array_capacity(pinel_array_t* self) {
    if (!self) {
        return 0;
    }

    return self->capacity;
}

void* pinel_array_at(pinel_array_t* self, size_t index) {
    if (!self) {
        return NULL;
    }

    if (index >= self->length) {
        return NULL;
    }

    return (self->array + self->elem_size * index);
}
