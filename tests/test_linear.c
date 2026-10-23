#include <pinel/linear.h>

#include "test.h"

static void test_backend(pinel_lin_kind_t kind) {
    pinel_lin_t* lin = pinel_lin_create(kind, sizeof(int), NULL, NULL);

    CHECK(lin != NULL);
    CHECK(pinel_lin_kind(lin) == kind);
    CHECK(pinel_lin_length(lin) == 0);
    CHECK(pinel_lin_capacity(lin) == 0);
    CHECK(pinel_lin_at(lin, 0) == NULL);

    for (int i = 0; i < 5; i++) {
        CHECK(pinel_lin_pushB(lin, &i) == 0);
    }

    int front = 40;
    CHECK(pinel_lin_pushF(lin, &front) == 0);

    int middle = 25;
    CHECK(pinel_lin_push(lin, &middle, 3) == 0);

    const int expected[] = {40, 0, 1, 25, 2, 3, 4};

    CHECK(pinel_lin_length(lin) == 7);
    CHECK(pinel_lin_at(lin, 7) == NULL);

    for (size_t i = 0; i < 7; i++) {
        CHECK(*(int*)pinel_lin_at(lin, i) == expected[i]);
    }

    int out = -1;

    CHECK(pinel_lin_pop(lin, 3, &out) == 0);
    CHECK(out == 25);

    CHECK(pinel_lin_popF(lin, &out) == 0);
    CHECK(out == 40);

    CHECK(pinel_lin_popB(lin, &out) == 0);
    CHECK(out == 4);

    const int since_pop[] = {0, 1, 2, 3};
    for (size_t i = 0; i < 4; i++) {
        CHECK(*(int*)pinel_lin_at(lin, i) == since_pop[i]);
    }

    CHECK(pinel_lin_pop(lin, 4, &out) == 1);

    pinel_lin_clear(lin);
    CHECK(pinel_lin_length(lin) == 0);
    CHECK(pinel_lin_capacity(lin) == 0);
    CHECK(pinel_lin_at(lin, 0) == NULL);

    
    CHECK(pinel_lin_pushB(lin, &front) == 0);
    CHECK(*(int*)pinel_lin_at(lin, 0) == 40);

    pinel_lin_destroy(&lin);
    CHECK(lin == NULL);
}

static void test_null_and_invalid(void) {
    CHECK(pinel_lin_create((pinel_lin_kind_t)99, sizeof(int), NULL, NULL) == NULL);

    CHECK(pinel_lin_length(NULL) == 0);
    CHECK(pinel_lin_capacity(NULL) == 0);
    CHECK(pinel_lin_at(NULL, 0) == NULL);
    CHECK(pinel_lin_push(NULL, NULL, 0) == 1);
    CHECK(pinel_lin_pushF(NULL, NULL) == 1);
    CHECK(pinel_lin_pushB(NULL, NULL) == 1);
    CHECK(pinel_lin_pop(NULL, 0, NULL) == 1);
    CHECK(pinel_lin_popF(NULL, NULL) == 1);
    CHECK(pinel_lin_popB(NULL, NULL) == 1);

    pinel_lin_clear(NULL);
    pinel_lin_destroy(NULL);
}

int main(void) {
    test_backend(PINEL_LIN_ARRAY);
    test_backend(PINEL_LIN_LIST);
    test_null_and_invalid();

    printf("linear tests passed\n");
    return EXIT_SUCCESS;
}
