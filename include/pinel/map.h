#ifndef PINEL_MAP_H
#define PINEL_MAP_H

#include <stddef.h>

#include "pinel/core.h"
#include "pinel/linear.h"

#define PINEL_MAP_INITIAL_CAPACITY 16

typedef struct pinel_map_t pinel_map_t;

/**
 * @brief Creates a new hash map.
 *
 * Keys and values are copied in and out of the map by size, so the map owns
 * its own copies. Collisions are resolved with separate chaining, and every
 * bucket chain is backed by either an array or a linked list, as selected by
 * `chain_kind`.
 *
 * If `key_cmp` is `NULL`, keys are compared with `memcmp` over `key_size` bytes.
 * If `hash` is `NULL`, keys are hashed with FNV-1a over `key_size` bytes.
 *
 * The `key_dealloc` and `value_dealloc` functions (optional) are called with a
 * pointer to the stored key/value bytes before the map releases that storage.
 * They should free resources owned by the key/value, not the storage itself.
 *
 * @param key_size The size (in bytes) of each key.
 * @param value_size The size (in bytes) of each value.
 * @param key_cmp A comparison function for keys (optional).
 * @param hash A hash function for keys (optional).
 * @param key_dealloc A function to deallocate keys (optional).
 * @param value_dealloc A function to deallocate values (optional).
 * @param chain_kind The linear container used for collision chains.
 * @return A pointer to the created hash map.
 */
pinel_map_t* pinel_map_create(size_t key_size, size_t value_size,
                              pinel_cmp_t key_cmp, pinel_hash_t hash,
                              pinel_dealloc_t key_dealloc,
                              pinel_dealloc_t value_dealloc,
                              pinel_lin_kind_t chain_kind);

/**
 * @brief Removes all entries from the map, freeing their keys, values and chains.
 *
 * The map itself remains intact with a size of zero.
 *
 * @param self The map to clear.
 */
void pinel_map_clear(pinel_map_t* self);

/**
 * @brief Destroys the map, freeing all of its entries and the map itself.
 *
 * @param self The map to be destroyed.
 */
void pinel_map_destroy(pinel_map_t** self);

/**
 * @brief Gets the number of entries in the map.
 *
 * @param self The map to inspect.
 * @return The number of stored key/value pairs.
 */
size_t pinel_map_length(pinel_map_t* self);

/**
 * @brief Gets the number of buckets in the map.
 *
 * @param self The map to inspect.
 * @return The bucket count, which is zero before the first insertion.
 */
size_t pinel_map_capacity(pinel_map_t* self);

/**
 * @brief Inserts or replaces a key/value pair.
 *
 * The key and value are copied into the map. If the key already exists, its
 * value is replaced.
 *
 * @param self The map to insert into.
 * @param key A pointer to the key to insert.
 * @param value A pointer to the value to insert.
 * @return 0 on success, 1 on failure.
 */
int pinel_map_put(pinel_map_t* self, const void* key, const void* value);

/**
 * @brief Looks up the value associated with a key.
 *
 * The returned pointer refers to storage owned by the map. It stays valid until
 * the map is modified, cleared or destroyed.
 *
 * @param self The map to search.
 * @param key The key to look up.
 * @return A pointer to the stored value, or `NULL` if the key is not present.
 */
void* pinel_map_get(pinel_map_t* self, const void* key);

/**
 * @brief Checks whether a key is present in the map.
 *
 * @param self The map to search.
 * @param key The key to look up.
 * @return 1 if the key is present, 0 otherwise.
 */
int pinel_map_contains(pinel_map_t* self, const void* key);

/**
 * @brief Removes a key/value pair from the map.
 *
 * @param self The map to remove from.
 * @param key The key to remove.
 * @return 0 on success, 1 if the key is not present or on failure.
 */
int pinel_map_remove(pinel_map_t* self, const void* key);


#endif // PINEL_MAP_H
