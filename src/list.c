#include "pinel/list.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>

typedef struct pinel_list_node_t {
    void* elem;
    struct pinel_list_node_t* prev;
    struct pinel_list_node_t* next;
} pinel_list_node_t;

struct pinel_list_t {
    pinel_list_node_t* head;
    pinel_list_node_t* tail;
    size_t length;
    size_t elem_size;
    pinel_cmp_t cmp;
    pinel_dealloc_t dealloc;
};

pinel_list_t* pinel_list_create(size_t elem_size, pinel_dealloc_t dealloc, pinel_cmp_t cmp) {
    pinel_list_t* self = malloc(sizeof(pinel_list_t));
    if (!self) {
        return NULL;
    }

    self->head = NULL;
    self->tail = NULL;

    self->length = 0;

    self->cmp = cmp;
    self->dealloc = dealloc;
    self->elem_size = elem_size;

    return self;
}

static pinel_list_node_t* node_create(pinel_list_t* self, void* elem) {
    pinel_list_node_t* node = malloc(sizeof(pinel_list_node_t));
    if (!node) {
        return NULL;
    }

    node->elem = malloc(self->elem_size);
    if (!node->elem) {
        free(node);
        return NULL;
    }

    memcpy(node->elem, elem, self->elem_size);

    node->prev = NULL;
    node->next = NULL;

    return node;
}

static pinel_list_node_t* node_at(pinel_list_t* self, size_t index) {
    if (index < self->length / 2) {
        pinel_list_node_t* node = self->head;
        for (size_t i = 0; i < index; i++) {
            node = node->next;
        }
        return node;
    }

    pinel_list_node_t* node = self->tail;
    for (size_t i = self->length - 1; i > index; i--) {
        node = node->prev;
    }
    return node;
}

int pinel_list_push(pinel_list_t* self, void* elem, size_t index) {
    if (!self) {
        return 1;
    }

    if (index > self->length) {
        return 1;
    }

    pinel_list_node_t* node = node_create(self, elem);
    if (!node) {
        return 1;
    }

    if (self->length == 0) {
        self->head = node;
        self->tail = node;
    }
    else if (index == 0) {
        node->next = self->head;
        self->head->prev = node;
        self->head = node;
    }
    else if (index == self->length) {
        node->prev = self->tail;
        self->tail->next = node;
        self->tail = node;
    }
    else {
        pinel_list_node_t* next = node_at(self, index);
        pinel_list_node_t* prev = next->prev;

        node->prev = prev;
        node->next = next;

        prev->next = node;
        next->prev = node;
    }

    self->length++;

    return 0;
}

int pinel_list_pushF(pinel_list_t* self, void* elem) {
    return pinel_list_push(self, elem, 0);
}

int pinel_list_pushB(pinel_list_t* self, void* elem) {
    if (!self) {
        return 1;
    }

    return pinel_list_push(self, elem, self->length);
}

int pinel_list_pop(pinel_list_t* self, size_t index, void* elem) {
    if (!self) {
        return 1;
    }

    if (index >= self->length) {
        return 1;
    }

    pinel_list_node_t* node = node_at(self, index);

    if (node->prev) {
        node->prev->next = node->next;
    }
    else {
        self->head = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    }
    else {
        self->tail = node->prev;
    }

    if (elem) {
        memcpy(elem, node->elem, self->elem_size);
    }
    else if (self->dealloc) {
        self->dealloc(node->elem);
    }

    free(node->elem);
    free(node);

    self->length--;

    return 0;
}

int pinel_list_popF(pinel_list_t* self, void* elem) {
    return pinel_list_pop(self, 0, elem);
}

int pinel_list_popB(pinel_list_t* self, void* elem) {
    if (!self) {
        return 1;
    }

    return pinel_list_pop(self, self->length - 1, elem);
}

void pinel_list_clear(pinel_list_t* self) {
    if (!self) {
        return;
    }

    pinel_list_node_t* node = self->head;
    while (node) {
        pinel_list_node_t* next = node->next;

        if (self->dealloc) {
            self->dealloc(node->elem);
        }

        free(node->elem);
        free(node);

        node = next;
    }

    self->head = NULL;
    self->tail = NULL;

    self->length = 0;
}

void pinel_list_destroy(pinel_list_t** self) {
    if (!self) {
        return;
    }

    if (!*self) {
        return;
    }

    pinel_list_clear(*self);
    free(*self);

    *self = NULL;
}

size_t pinel_list_length(pinel_list_t* self) {
    if (!self) {
        return 0;
    }

    return self->length;
}

size_t pinel_list_capacity(pinel_list_t* self) {
    if (!self) {
        return 0;
    }

    return self->length;
}

void* pinel_list_at(pinel_list_t* self, size_t index) {
    if (!self) {
        return NULL;
    }

    if (index >= self->length) {
        return NULL;
    }

    return node_at(self, index)->elem;
}
