#include <pinel/array.h>

#include "test.h"

static int deallocs = 0;

static void count_dealloc(void* elem) {
    (void)elem;
    deallocs++;
}

static void test_create_and_access(void) {
    pinel_array_t* array = pinel_array_create(sizeof(int), NULL, NULL);

    CHECK(array != NULL);
    CHECK(pinel_array_length(array) == 0);
    CHECK(pinel_array_capacity(array) == 0);
    CHECK(pinel_array_at(array, 0) == NULL);

    int value = 42;
    CHECK(pinel_array_pushB(array, &value) == 0);

    CHECK(pinel_array_length(array) == 1);
    CHECK(pinel_array_at(array, 0) != NULL);
    CHECK(*(int*)pinel_array_at(array, 0) == 42);

    
    value = 7;
    CHECK(*(int*)pinel_array_at(array, 0) == 42);

    
    CHECK(pinel_array_at(array, 1) == NULL);
    CHECK(pinel_array_at(array, 100) == NULL);

    pinel_array_destroy(&array);
    CHECK(array == NULL);
}

static void test_push(void) {
    pinel_array_t* array = pinel_array_create(sizeof(int), NULL, NULL);
    CHECK(array != NULL);

    for (int i = 0; i < 5; i++) {
        CHECK(pinel_array_pushB(array, &i) == 0);
    }

    int front = 40;
    CHECK(pinel_array_pushF(array, &front) == 0);

    int middle = 25;
    CHECK(pinel_array_push(array, &middle, 3) == 0);

    const int expected[] = {40, 0, 1, 25, 2, 3, 4};

    CHECK(pinel_array_length(array) == 7);
    CHECK(pinel_array_capacity(array) >= 7);

    for (size_t i = 0; i < 7; i++) {
        CHECK(*(int*)pinel_array_at(array, i) == expected[i]);
    }

    
    int value = 99;
    CHECK(pinel_array_push(array, &value, 8) == 1);
    CHECK(pinel_array_length(array) == 7);

    
    CHECK(pinel_array_push(NULL, &value, 0) == 1);
    CHECK(pinel_array_pushF(NULL, &value) == 1);
    CHECK(pinel_array_pushB(NULL, &value) == 1);

    pinel_array_destroy(&array);
}

static void test_pop(void) {
    pinel_array_t* array = pinel_array_create(sizeof(int), NULL, NULL);
    CHECK(array != NULL);

    for (int i = 0; i < 5; i++) {
        CHECK(pinel_array_pushB(array, &i) == 0);
    }

    int out = -1;

    CHECK(pinel_array_pop(array, 2, &out) == 0);
    CHECK(out == 2);
    CHECK(pinel_array_length(array) == 4);

    CHECK(pinel_array_popF(array, &out) == 0);
    CHECK(out == 0);

    CHECK(pinel_array_popB(array, &out) == 0);
    CHECK(out == 4);

    const int expected[] = {1, 3};
    for (size_t i = 0; i < 2; i++) {
        CHECK(*(int*)pinel_array_at(array, i) == expected[i]);
    }

    CHECK(pinel_array_pop(array, 2, &out) == 1);
    CHECK(pinel_array_pop(NULL, 0, &out) == 1);
    CHECK(pinel_array_popF(NULL, &out) == 1);
    CHECK(pinel_array_popB(NULL, &out) == 1);

    pinel_array_t* empty = pinel_array_create(sizeof(int), NULL, NULL);
    CHECK(empty != NULL);
    CHECK(pinel_array_popB(empty, &out) == 1);
    pinel_array_destroy(&empty);

    pinel_array_destroy(&array);
}

static void test_dealloc_and_clear(void) {
    deallocs = 0;

    pinel_array_t* array = pinel_array_create(sizeof(int), count_dealloc, NULL);
    CHECK(array != NULL);

    int value = 1;
    CHECK(pinel_array_pushB(array, &value) == 0);

    
    CHECK(pinel_array_pop(array, 0, NULL) == 0);
    CHECK(deallocs == 1);
    CHECK(pinel_array_length(array) == 0);

    CHECK(pinel_array_pushB(array, &value) == 0);
    CHECK(pinel_array_pushB(array, &value) == 0);

    pinel_array_clear(array);
    CHECK(deallocs == 3);
    CHECK(pinel_array_length(array) == 0);
    CHECK(pinel_array_capacity(array) == 0);

    
    CHECK(pinel_array_pushB(array, &value) == 0);
    CHECK(*(int*)pinel_array_at(array, 0) == 1);

    pinel_array_destroy(&array);

    
    pinel_array_clear(NULL);
    pinel_array_destroy(NULL);
    CHECK(pinel_array_length(NULL) == 0);
    CHECK(pinel_array_capacity(NULL) == 0);
    CHECK(pinel_array_at(NULL, 0) == NULL);
}

static void test_growth(void) {
    pinel_array_t* array = pinel_array_create(sizeof(int), NULL, NULL);
    CHECK(array != NULL);

    for (int i = 0; i < 1000; i++) {
        CHECK(pinel_array_pushB(array, &i) == 0);
    }

    CHECK(pinel_array_length(array) == 1000);
    CHECK(pinel_array_capacity(array) >= 1000);

    for (int i = 0; i < 1000; i++) {
        CHECK(*(int*)pinel_array_at(array, (size_t)i) == i);
    }

    pinel_array_destroy(&array);
}

int main(void) {
    test_create_and_access();
    test_push();
    test_pop();
    test_dealloc_and_clear();
    test_growth();

    printf("array tests passed\n");
    return EXIT_SUCCESS;
}
