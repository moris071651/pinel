#ifndef PINEL_ARRAY_H
#define PINEL_ARRAY_H

#include <stddef.h>
#include <stdint.h>

#include "pinel/core.h"

typedef struct pinel_array_t pinel_array_t;

/**
 * @brief Creates a new dynamic array.
 *
 * This function initializes and returns a new dynamic array that can store elements
 * of a given size. The array will be initialized with zero length and an initial
 * capacity, and you can specify the deallocation and comparison functions.
 * 
 * @param elem_size The size (in bytes) of each element that the array will hold.
 * @param dealloc A function to deallocate elements when the array is modified (optional).
 * @param cmp A comparison function for sorting and comparisons (optional).
 * @return A pointer to the created dynamic array.
 */
pinel_array_t* pinel_array_create(size_t elem_size, pinel_dealloc_t dealloc, pinel_cmp_t cmp);

#endif // PINEL_ARRAY_H