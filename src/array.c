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
