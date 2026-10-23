#include "pinel/map.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct pinel_map_entry_t {
    void* key;
    void* value;
} pinel_map_entry_t;

struct pinel_map_t {
    pinel_lin_t** buckets;
    size_t capacity;
    size_t length;

    size_t key_size;
    size_t value_size;

    pinel_cmp_t key_cmp;
    pinel_hash_t hash;
    pinel_dealloc_t key_dealloc;
    pinel_dealloc_t value_dealloc;

    pinel_lin_kind_t chain_kind;
};

static size_t hash_default(const void* key, size_t size) {
    const uint8_t* bytes = key;

    size_t hash = sizeof(size_t) >= 8 ? (size_t)14695981039346656037ULL
                                     : (size_t)2166136261U;
    size_t prime = sizeof(size_t) >= 8 ? (size_t)1099511628211ULL
                                       : (size_t)16777619U;

    for (size_t i = 0; i < size; i++) {
        hash ^= bytes[i];
        hash *= prime;
    }

    return hash;
}

static size_t map_hash(pinel_map_t* self, const void* key) {
    if (self->hash) {
        return self->hash(key);
    }

    return hash_default(key, self->key_size);
}

static size_t map_index(pinel_map_t* self, const void* key) {
    return map_hash(self, key) % self->capacity;
}

static int key_equal(pinel_map_t* self, const void* a, const void* b) {
    if (self->key_cmp) {
        return self->key_cmp(a, b) == 0;
    }

    return memcmp(a, b, self->key_size) == 0;
}

static size_t chain_find(pinel_map_t* self, pinel_lin_t* chain, const void* key) {
    size_t length = pinel_lin_length(chain);

    for (size_t i = 0; i < length; i++) {
        pinel_map_entry_t* entry = pinel_lin_at(chain, i);

        if (key_equal(self, key, entry->key)) {
            return i;
        }
    }

    return (size_t)-1;
}

static pinel_lin_t* chain_create(pinel_map_t* self) {
    return pinel_lin_create(self->chain_kind, sizeof(pinel_map_entry_t), NULL, NULL);
}

static void buckets_destroy(pinel_lin_t** buckets, size_t capacity) {
    if (!buckets) {
        return;
    }

    for (size_t i = 0; i < capacity; i++) {
        if (buckets[i]) {
            pinel_lin_destroy(&buckets[i]);
        }
    }

    free(buckets);
}

static void entry_destroy(pinel_map_t* self, pinel_map_entry_t* entry) {
    if (self->key_dealloc) {
        self->key_dealloc(entry->key);
    }

    if (self->value_dealloc) {
        self->value_dealloc(entry->value);
    }

    free(entry->key);
    free(entry->value);
}

static int map_reserve(pinel_map_t* self, size_t capacity) {
    pinel_lin_t** buckets = calloc(capacity, sizeof(pinel_lin_t*));
    if (!buckets) {
        return 1;
    }

    for (size_t i = 0; i < self->capacity; i++) {
        pinel_lin_t* chain = self->buckets[i];
        if (!chain) {
            continue;
        }

        size_t length = pinel_lin_length(chain);

        for (size_t j = 0; j < length; j++) {
            pinel_map_entry_t* entry = pinel_lin_at(chain, j);
            size_t index = map_hash(self, entry->key) % capacity;

            if (!buckets[index]) {
                buckets[index] = chain_create(self);
                if (!buckets[index]) {
                    buckets_destroy(buckets, capacity);
                    return 1;
                }
            }

            if (pinel_lin_pushB(buckets[index], entry)) {
                buckets_destroy(buckets, capacity);
                return 1;
            }
        }
    }

    buckets_destroy(self->buckets, self->capacity);

    self->buckets = buckets;
    self->capacity = capacity;

    return 0;
}

pinel_map_t* pinel_map_create(size_t key_size, size_t value_size,
                              pinel_cmp_t key_cmp, pinel_hash_t hash,
                              pinel_dealloc_t key_dealloc,
                              pinel_dealloc_t value_dealloc,
                              pinel_lin_kind_t chain_kind) {
    if (key_size == 0 || value_size == 0) {
        return NULL;
    }

    if (chain_kind != PINEL_LIN_ARRAY && chain_kind != PINEL_LIN_LIST) {
        return NULL;
    }

    pinel_map_t* self = malloc(sizeof(pinel_map_t));
    if (!self) {
        return NULL;
    }

    self->buckets = NULL;
    self->capacity = 0;
    self->length = 0;

    self->key_size = key_size;
    self->value_size = value_size;

    self->key_cmp = key_cmp;
    self->hash = hash;
    self->key_dealloc = key_dealloc;
    self->value_dealloc = value_dealloc;

    self->chain_kind = chain_kind;

    return self;
}

void pinel_map_clear(pinel_map_t* self) {
    if (!self) {
        return;
    }

    for (size_t i = 0; i < self->capacity; i++) {
        pinel_lin_t* chain = self->buckets[i];
        if (!chain) {
            continue;
        }

        size_t length = pinel_lin_length(chain);

        for (size_t j = 0; j < length; j++) {
            entry_destroy(self, pinel_lin_at(chain, j));
        }

        pinel_lin_destroy(&self->buckets[i]);
    }

    free(self->buckets);

    self->buckets = NULL;
    self->capacity = 0;
    self->length = 0;
}

void pinel_map_destroy(pinel_map_t** self) {
    if (!self) {
        return;
    }

    if (!*self) {
        return;
    }

    pinel_map_clear(*self);
    free(*self);

    *self = NULL;
}

size_t pinel_map_length(pinel_map_t* self) {
    if (!self) {
        return 0;
    }

    return self->length;
}

size_t pinel_map_capacity(pinel_map_t* self) {
    if (!self) {
        return 0;
    }

    return self->capacity;
}

int pinel_map_put(pinel_map_t* self, const void* key, const void* value) {
    if (!self || !key || !value) {
        return 1;
    }

    if (self->length + 1 > self->capacity - self->capacity / 4) {
        size_t capacity = self->capacity ? self->capacity * 2 : PINEL_MAP_INITIAL_CAPACITY;

        if (map_reserve(self, capacity)) {
            return 1;
        }
    }

    size_t index = map_index(self, key);
    pinel_lin_t* chain = self->buckets[index];

    if (chain) {
        size_t found = chain_find(self, chain, key);

        if (found != (size_t)-1) {
            pinel_map_entry_t* entry = pinel_lin_at(chain, found);

            if (self->value_dealloc) {
                self->value_dealloc(entry->value);
            }

            memcpy(entry->value, value, self->value_size);

            return 0;
        }
    }

    if (!chain) {
        chain = chain_create(self);
        if (!chain) {
            return 1;
        }

        self->buckets[index] = chain;
    }

    pinel_map_entry_t entry = {
        .key = malloc(self->key_size),
        .value = malloc(self->value_size),
    };

    if (!entry.key || !entry.value) {
        free(entry.key);
        free(entry.value);

        if (pinel_lin_length(chain) == 0) {
            pinel_lin_destroy(&self->buckets[index]);
        }

        return 1;
    }

    memcpy(entry.key, key, self->key_size);
    memcpy(entry.value, value, self->value_size);

    if (pinel_lin_pushB(chain, &entry)) {
        free(entry.key);
        free(entry.value);

        if (pinel_lin_length(chain) == 0) {
            pinel_lin_destroy(&self->buckets[index]);
        }

        return 1;
    }

    self->length++;

    return 0;
}

void* pinel_map_get(pinel_map_t* self, const void* key) {
    if (!self || !key || self->capacity == 0) {
        return NULL;
    }

    pinel_lin_t* chain = self->buckets[map_index(self, key)];
    if (!chain) {
        return NULL;
    }

    size_t found = chain_find(self, chain, key);
    if (found == (size_t)-1) {
        return NULL;
    }

    return ((pinel_map_entry_t*)pinel_lin_at(chain, found))->value;
}

int pinel_map_contains(pinel_map_t* self, const void* key) {
    if (!self || !key || self->capacity == 0) {
        return 0;
    }

    pinel_lin_t* chain = self->buckets[map_index(self, key)];
    if (!chain) {
        return 0;
    }

    return chain_find(self, chain, key) != (size_t)-1;
}

int pinel_map_remove(pinel_map_t* self, const void* key) {
    if (!self || !key || self->capacity == 0) {
        return 1;
    }

    size_t index = map_index(self, key);
    pinel_lin_t* chain = self->buckets[index];

    if (!chain) {
        return 1;
    }

    size_t found = chain_find(self, chain, key);
    if (found == (size_t)-1) {
        return 1;
    }

    entry_destroy(self, pinel_lin_at(chain, found));

    if (pinel_lin_pop(chain, found, NULL)) {
        return 1;
    }

    self->length--;

    if (pinel_lin_length(chain) == 0) {
        pinel_lin_destroy(&self->buckets[index]);
    }

    return 0;
}
