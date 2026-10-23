#ifndef PINEL_LIST_H
#define PINEL_LIST_H

#include <stddef.h>
#include <stdint.h>

#include "pinel/core.h"

typedef struct pinel_list_t pinel_list_t;

/**
 * @brief Creates a new linked list.
 *
 * This function initializes and returns a new linked list that can store elements
 * of a given size. The list will be initialized empty, and you can specify the
 * deallocation and comparison functions.
 *
 * @param elem_size The size (in bytes) of each element that the list will hold.
 * @param dealloc A function to deallocate elements when the list is modified (optional).
 * @param cmp A comparison function for sorting and comparisons (optional).
 * @return A pointer to the created linked list.
 */
pinel_list_t* pinel_list_create(size_t elem_size, pinel_dealloc_t dealloc, pinel_cmp_t cmp);

/**
 * @brief Clears all elements from the list, freeing any allocated memory.
 *
 * This function removes all elements from the list, and depending on the
 * `dealloc` function provided during creation, it may free the memory of the elements.
 * The list itself remains intact with a size of zero.
 *
 * @param self The list to clear.
 */
void pinel_list_clear(pinel_list_t* self);

/**
 * @brief Destroys the list, freeing the memory of the list and its elements.
 *
 * This function destroys the list by deallocating its nodes and freeing
 * the list itself. After calling this function, the list pointer will be invalid.
 *
 * @param self The list to be destroyed.
 */
void pinel_list_destroy(pinel_list_t** self);

/**
 * @brief Gets the length of the list.
 *
 * This function returns the current number of elements in the list.
 *
 * @param self The list to get the length of.
 * @return The number of elements in the list.
 */
size_t pinel_list_length(pinel_list_t* self);

/**
 * @brief Gets the capacity of the list.
 *
 * A linked list allocates memory one node at a time, so it has no unused
 * preallocated space. The capacity is therefore always equal to the length.
 *
 * @param self The list to get the capacity of.
 * @return The total capacity of the list.
 */
size_t pinel_list_capacity(pinel_list_t* self);

/**
 * @brief Gets the element at the specified index in the list.
 *
 * This function retrieves a pointer to the element at the given index in
 * the list. If the provided index is out of bounds,
 * the function will return `NULL`.
 *
 * @param self The list to access.
 * @param index The index of the element to retrieve.
 * @return A pointer to the element at the specified index,
 * or `NULL` if the index is out of bounds.
 */
void* pinel_list_at(pinel_list_t* self, size_t index);

/**
 * @brief Inserts an element at the specified index in the list.
 *
 * @param self The list to insert into.
 * @param elem A pointer to the element to insert.
 * @param index The index the element will be inserted at.
 * @return 0 on success, 1 if the list or the index is invalid or on failure.
 */
int pinel_list_push(pinel_list_t* self, void* elem, size_t index);

/**
 * @brief Inserts an element at the front of the list.
 *
 * @param self The list to insert into.
 * @param elem A pointer to the element to insert.
 * @return 0 on success, 1 on failure.
 */
int pinel_list_pushF(pinel_list_t* self, void* elem);

/**
 * @brief Inserts an element at the back of the list.
 *
 * @param self The list to insert into.
 * @param elem A pointer to the element to insert.
 * @return 0 on success, 1 on failure.
 */
int pinel_list_pushB(pinel_list_t* self, void* elem);

/**
 * @brief Removes the element at the specified index from the list.
 *
 * If `elem` is not `NULL`, the removed element is copied into it.
 * Otherwise, the `dealloc` function provided during creation is called (if any).
 *
 * @param self The list to remove from.
 * @param index The index of the element to remove.
 * @param elem Where to copy the removed element, or `NULL`.
 * @return 0 on success, 1 on failure.
 */
int pinel_list_pop(pinel_list_t* self, size_t index, void* elem);

/**
 * @brief Removes the first element from the list.
 *
 * @param self The list to remove from.
 * @param elem Where to copy the removed element, or `NULL`.
 * @return 0 on success, 1 on failure.
 */
int pinel_list_popF(pinel_list_t* self, void* elem);

/**
 * @brief Removes the last element from the list.
 *
 * @param self The list to remove from.
 * @param elem Where to copy the removed element, or `NULL`.
 * @return 0 on success, 1 on failure.
 */
int pinel_list_popB(pinel_list_t* self, void* elem);


#endif // PINEL_LIST_H
