#ifndef PINEL_LINEAR_H
#define PINEL_LINEAR_H

#include <stddef.h>
#include <stdint.h>

#include "pinel/core.h"
#include "pinel/array.h"
#include "pinel/list.h"

typedef enum pinel_lin_kind_t {
    PINEL_LIN_ARRAY,
    PINEL_LIN_LIST,
} pinel_lin_kind_t;

typedef struct pinel_lin_t pinel_lin_t;

/**
 * @brief Creates a new linear container backed by an array or a linked list.
 *
 * @param kind The underlying implementation to use.
 * @param elem_size The size (in bytes) of each element that the container will hold.
 * @param dealloc A function to deallocate elements when the container is modified (optional).
 * @param cmp A comparison function for sorting and comparisons (optional).
 * @return A pointer to the created container.
 */
pinel_lin_t* pinel_lin_create(pinel_lin_kind_t kind, size_t elem_size, pinel_dealloc_t dealloc, pinel_cmp_t cmp);

/**
 * @brief Gets the kind of the underlying implementation.
 *
 * @param self The container to inspect.
 * @return The kind of the container.
 */
pinel_lin_kind_t pinel_lin_kind(pinel_lin_t* self);

/**
 * @brief Clears all elements from the container, freeing any allocated memory.
 *
 * @param self The container to clear.
 */
void pinel_lin_clear(pinel_lin_t* self);

/**
 * @brief Destroys the container, freeing its memory and the memory of its elements.
 *
 * @param self The container to be destroyed.
 */
void pinel_lin_destroy(pinel_lin_t** self);

/**
 * @brief Gets the length of the container.
 *
 * @param self The container to get the length of.
 * @return The number of elements in the container.
 */
size_t pinel_lin_length(pinel_lin_t* self);

/**
 * @brief Gets the capacity of the container.
 *
 * @param self The container to get the capacity of.
 * @return The total capacity of the container.
 */
size_t pinel_lin_capacity(pinel_lin_t* self);

/**
 * @brief Gets the element at the specified index in the container.
 *
 * @param self The container to access.
 * @param index The index of the element to retrieve.
 * @return A pointer to the element, or `NULL` if the index is out of bounds.
 */
void* pinel_lin_at(pinel_lin_t* self, size_t index);

/**
 * @brief Inserts an element at the specified index in the container.
 *
 * @param self The container to insert into.
 * @param elem A pointer to the element to insert.
 * @param index The index the element will be inserted at.
 * @return 0 on success, 1 on failure.
 */
int pinel_lin_push(pinel_lin_t* self, void* elem, size_t index);

/**
 * @brief Inserts an element at the front of the container.
 *
 * @param self The container to insert into.
 * @param elem A pointer to the element to insert.
 * @return 0 on success, 1 on failure.
 */
int pinel_lin_pushF(pinel_lin_t* self, void* elem);

/**
 * @brief Inserts an element at the back of the container.
 *
 * @param self The container to insert into.
 * @param elem A pointer to the element to insert.
 * @return 0 on success, 1 on failure.
 */
int pinel_lin_pushB(pinel_lin_t* self, void* elem);

/**
 * @brief Removes the element at the specified index from the container.
 *
 * @param self The container to remove from.
 * @param index The index of the element to remove.
 * @param elem Where to copy the removed element, or `NULL`.
 * @return 0 on success, 1 on failure.
 */
int pinel_lin_pop(pinel_lin_t* self, size_t index, void* elem);

/**
 * @brief Removes the first element from the container.
 *
 * @param self The container to remove from.
 * @param elem Where to copy the removed element, or `NULL`.
 * @return 0 on success, 1 on failure.
 */
int pinel_lin_popF(pinel_lin_t* self, void* elem);

/**
 * @brief Removes the last element from the container.
 *
 * @param self The container to remove from.
 * @param elem Where to copy the removed element, or `NULL`.
 * @return 0 on success, 1 on failure.
 */
int pinel_lin_popB(pinel_lin_t* self, void* elem);


#endif // PINEL_LINEAR_H
