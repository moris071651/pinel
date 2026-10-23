#ifndef PINEL_CORE_H
#define PINEL_CORE_H

#include <stddef.h>

typedef void (*pinel_dealloc_t)(void*);
typedef int (*pinel_cmp_t)(const void*, const void*);
typedef size_t (*pinel_hash_t)(const void*);

#endif // PINEL_CORE_H
