#include <pinel/list.h>

#include "test.h"

static int deallocs = 0;

static void count_dealloc(void* elem) {
    (void)elem;
    deallocs++;
}

static void test_create_and_access(void) {
    pinel_list_t* list = pinel_list_create(sizeof(int), NULL, NULL);

    CHECK(list != NULL);
    CHECK(pinel_list_length(list) == 0);
    CHECK(pinel_list_capacity(list) == 0);
    CHECK(pinel_list_at(list, 0) == NULL);

    int value = 42;
    CHECK(pinel_list_pushB(list, &value) == 0);

    CHECK(pinel_list_length(list) == 1);
    CHECK(*(int*)pinel_list_at(list, 0) == 42);

    
    value = 7;
    CHECK(*(int*)pinel_list_at(list, 0) == 42);

    CHECK(pinel_list_at(list, 1) == NULL);
    CHECK(pinel_list_at(list, 100) == NULL);

    pinel_list_destroy(&list);
    CHECK(list == NULL);
}

static void test_push(void) {
    pinel_list_t* list = pinel_list_create(sizeof(int), NULL, NULL);
    CHECK(list != NULL);

    for (int i = 0; i < 5; i++) {
        CHECK(pinel_list_pushB(list, &i) == 0);
    }

    int front = 40;
    CHECK(pinel_list_pushF(list, &front) == 0);

    int middle = 25;
    CHECK(pinel_list_push(list, &middle, 3) == 0);

    const int expected[] = {40, 0, 1, 25, 2, 3, 4};

    CHECK(pinel_list_length(list) == 7);
    CHECK(pinel_list_capacity(list) == 7);

    for (size_t i = 0; i < 7; i++) {
        CHECK(*(int*)pinel_list_at(list, i) == expected[i]);
    }

    
    int value = 99;
    CHECK(pinel_list_push(list, &value, 8) == 1);
    CHECK(pinel_list_length(list) == 7);

    CHECK(pinel_list_push(NULL, &value, 0) == 1);
    CHECK(pinel_list_pushF(NULL, &value) == 1);
    CHECK(pinel_list_pushB(NULL, &value) == 1);

    pinel_list_destroy(&list);
}

static void test_pop(void) {
    pinel_list_t* list = pinel_list_create(sizeof(int), NULL, NULL);
    CHECK(list != NULL);

    for (int i = 0; i < 5; i++) {
        CHECK(pinel_list_pushB(list, &i) == 0);
    }

    int out = -1;

    CHECK(pinel_list_pop(list, 2, &out) == 0);
    CHECK(out == 2);
    CHECK(pinel_list_length(list) == 4);

    CHECK(pinel_list_popF(list, &out) == 0);
    CHECK(out == 0);

    CHECK(pinel_list_popB(list, &out) == 0);
    CHECK(out == 4);

    const int expected[] = {1, 3};
    for (size_t i = 0; i < 2; i++) {
        CHECK(*(int*)pinel_list_at(list, i) == expected[i]);
    }

    CHECK(pinel_list_pop(list, 2, &out) == 1);
    CHECK(pinel_list_pop(NULL, 0, &out) == 1);
    CHECK(pinel_list_popF(NULL, &out) == 1);
    CHECK(pinel_list_popB(NULL, &out) == 1);

    pinel_list_t* empty = pinel_list_create(sizeof(int), NULL, NULL);
    CHECK(empty != NULL);
    CHECK(pinel_list_popB(empty, &out) == 1);
    pinel_list_destroy(&empty);

    pinel_list_destroy(&list);
}

static void test_dealloc_and_clear(void) {
    deallocs = 0;

    pinel_list_t* list = pinel_list_create(sizeof(int), count_dealloc, NULL);
    CHECK(list != NULL);

    int value = 1;
    CHECK(pinel_list_pushB(list, &value) == 0);

    CHECK(pinel_list_pop(list, 0, NULL) == 0);
    CHECK(deallocs == 1);
    CHECK(pinel_list_length(list) == 0);

    CHECK(pinel_list_pushB(list, &value) == 0);
    CHECK(pinel_list_pushB(list, &value) == 0);

    pinel_list_clear(list);
    CHECK(deallocs == 3);
    CHECK(pinel_list_length(list) == 0);
    CHECK(pinel_list_capacity(list) == 0);

    
    CHECK(pinel_list_pushB(list, &value) == 0);
    CHECK(*(int*)pinel_list_at(list, 0) == 1);

    pinel_list_destroy(&list);

    pinel_list_clear(NULL);
    pinel_list_destroy(NULL);
    CHECK(pinel_list_length(NULL) == 0);
    CHECK(pinel_list_capacity(NULL) == 0);
    CHECK(pinel_list_at(NULL, 0) == NULL);
}

static void test_growth(void) {
    pinel_list_t* list = pinel_list_create(sizeof(int), NULL, NULL);
    CHECK(list != NULL);

    for (int i = 0; i < 1000; i++) {
        CHECK(pinel_list_pushB(list, &i) == 0);
    }

    CHECK(pinel_list_length(list) == 1000);
    CHECK(pinel_list_capacity(list) == 1000);

    for (int i = 0; i < 1000; i++) {
        CHECK(*(int*)pinel_list_at(list, (size_t)i) == i);
    }

    pinel_list_destroy(&list);
}

int main(void) {
    test_create_and_access();
    test_push();
    test_pop();
    test_dealloc_and_clear();
    test_growth();

    printf("list tests passed\n");
    return EXIT_SUCCESS;
}
