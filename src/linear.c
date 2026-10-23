#include "pinel/linear.h"

#include <stdlib.h>

struct pinel_lin_t {
    pinel_lin_kind_t kind;
    union {
        pinel_array_t* array;
        pinel_list_t* list;
    } as;
};

pinel_lin_t* pinel_lin_create(pinel_lin_kind_t kind, size_t elem_size, pinel_dealloc_t dealloc, pinel_cmp_t cmp) {
    pinel_lin_t* self = malloc(sizeof(pinel_lin_t));
    if (!self) {
        return NULL;
    }

    self->kind = kind;

    switch (kind) {
        case PINEL_LIN_ARRAY:
            self->as.array = pinel_array_create(elem_size, dealloc, cmp);
            if (!self->as.array) {
                free(self);
                return NULL;
            }
            break;

        case PINEL_LIN_LIST:
            self->as.list = pinel_list_create(elem_size, dealloc, cmp);
            if (!self->as.list) {
                free(self);
                return NULL;
            }
            break;

        default:
            free(self);
            return NULL;
    }

    return self;
}

pinel_lin_kind_t pinel_lin_kind(pinel_lin_t* self) {
    if (!self) {
        return PINEL_LIN_ARRAY;
    }

    return self->kind;
}

void pinel_lin_clear(pinel_lin_t* self) {
    if (!self) {
        return;
    }

    switch (self->kind) {
        case PINEL_LIN_ARRAY:
            pinel_array_clear(self->as.array);
            break;

        case PINEL_LIN_LIST:
            pinel_list_clear(self->as.list);
            break;
    }
}

void pinel_lin_destroy(pinel_lin_t** self) {
    if (!self) {
        return;
    }

    if (!*self) {
        return;
    }

    switch ((*self)->kind) {
        case PINEL_LIN_ARRAY:
            pinel_array_destroy(&(*self)->as.array);
            break;

        case PINEL_LIN_LIST:
            pinel_list_destroy(&(*self)->as.list);
            break;
    }

    free(*self);

    *self = NULL;
}

size_t pinel_lin_length(pinel_lin_t* self) {
    if (!self) {
        return 0;
    }

    switch (self->kind) {
        case PINEL_LIN_ARRAY:
            return pinel_array_length(self->as.array);

        case PINEL_LIN_LIST:
            return pinel_list_length(self->as.list);
    }

    return 0;
}

size_t pinel_lin_capacity(pinel_lin_t* self) {
    if (!self) {
        return 0;
    }

    switch (self->kind) {
        case PINEL_LIN_ARRAY:
            return pinel_array_capacity(self->as.array);

        case PINEL_LIN_LIST:
            return pinel_list_capacity(self->as.list);
    }

    return 0;
}

void* pinel_lin_at(pinel_lin_t* self, size_t index) {
    if (!self) {
        return NULL;
    }

    switch (self->kind) {
        case PINEL_LIN_ARRAY:
            return pinel_array_at(self->as.array, index);

        case PINEL_LIN_LIST:
            return pinel_list_at(self->as.list, index);
    }

    return NULL;
}

int pinel_lin_push(pinel_lin_t* self, void* elem, size_t index) {
    if (!self) {
        return 1;
    }

    switch (self->kind) {
        case PINEL_LIN_ARRAY:
            return pinel_array_push(self->as.array, elem, index);

        case PINEL_LIN_LIST:
            return pinel_list_push(self->as.list, elem, index);
    }

    return 1;
}

int pinel_lin_pushF(pinel_lin_t* self, void* elem) {
    if (!self) {
        return 1;
    }

    switch (self->kind) {
        case PINEL_LIN_ARRAY:
            return pinel_array_pushF(self->as.array, elem);

        case PINEL_LIN_LIST:
            return pinel_list_pushF(self->as.list, elem);
    }

    return 1;
}

int pinel_lin_pushB(pinel_lin_t* self, void* elem) {
    if (!self) {
        return 1;
    }

    switch (self->kind) {
        case PINEL_LIN_ARRAY:
            return pinel_array_pushB(self->as.array, elem);

        case PINEL_LIN_LIST:
            return pinel_list_pushB(self->as.list, elem);
    }

    return 1;
}

int pinel_lin_pop(pinel_lin_t* self, size_t index, void* elem) {
    if (!self) {
        return 1;
    }

    switch (self->kind) {
        case PINEL_LIN_ARRAY:
            return pinel_array_pop(self->as.array, index, elem);

        case PINEL_LIN_LIST:
            return pinel_list_pop(self->as.list, index, elem);
    }

    return 1;
}

int pinel_lin_popF(pinel_lin_t* self, void* elem) {
    if (!self) {
        return 1;
    }

    switch (self->kind) {
        case PINEL_LIN_ARRAY:
            return pinel_array_popF(self->as.array, elem);

        case PINEL_LIN_LIST:
            return pinel_list_popF(self->as.list, elem);
    }

    return 1;
}

int pinel_lin_popB(pinel_lin_t* self, void* elem) {
    if (!self) {
        return 1;
    }

    switch (self->kind) {
        case PINEL_LIN_ARRAY:
            return pinel_array_popB(self->as.array, elem);

        case PINEL_LIN_LIST:
            return pinel_list_popB(self->as.list, elem);
    }

    return 1;
}
