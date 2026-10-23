#ifndef PINEL_TEST_H
#define PINEL_TEST_H

#include <stdio.h>
#include <stdlib.h>

#define CHECK(cond)                                                          \
    do {                                                                     \
        if (!(cond)) {                                                       \
            fprintf(stderr, "%s:%d: check failed: %s\n",                     \
                    __FILE__, __LINE__, #cond);                              \
            exit(EXIT_FAILURE);                                              \
        }                                                                    \
    } while (0)

#endif
