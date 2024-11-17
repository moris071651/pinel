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

/**
 * @brief Clears all elements from the array, freeing any allocated memory.
 *
 * This function removes all elements from the array, and depending on the 
 * `dealloc` function provided during creation, it may free the memory of the elements.
 * The array itself remains intact with a size of zero.
 * 
 * @param self The array to clear.
 */
void pinel_array_clear(pinel_array_t* self);

/**
 * @brief Destroys the array, freeing the memory of the array and its elements.
 *
 * This function destroys the array by deallocating its internal data and freeing
 * the array itself. After calling this function, the array pointer will be invalid.
 * 
 * @param self The array to be destroyed.
 */
void pinel_array_destroy(pinel_array_t* self);

/**
 * @brief Gets the length of the array.
 *
 * This function returns the current number of elements in the array.
 * 
 * @param self The array to get the length of.
 * @return The number of elements in the array.
 */
size_t pinel_array_length(pinel_array_t* self);

/**
 * @brief Gets the capacity of the array.
 *
 * This function returns the total number of elements the array can hold
 * before needing to resize. It may be greater than the length of the array
 * if there is unused capacity.
 * 
 * @param self The array to get the capacity of.
 * @return The total capacity of the array.
 */
size_t pinel_array_capacity(pinel_array_t* self);

/**
 * @brief Gets the element at the specified index in the array.
 *
 * This function retrieves a pointer to the element at the given index in
 * the array. If the provided index is out of bounds,
 * the function will return `NULL`.
 * 
 * @param self The array to access.
 * @param index The index of the element to retrieve.
 * @return A pointer to the element at the specified index,
 * or `NULL` if the index is out of bounds.
 */
void* pinel_array_get(pinel_array_t* self, size_t index);

#endif // PINEL_ARRAY_H