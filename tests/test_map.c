#include <pinel/map.h>

#include <string.h>

#include "test.h"

static size_t hash_zero(const void* key) {
    (void)key;
    return 0;
}

static size_t hash_int(const void* key) {
    return (size_t)*(const int*)key;
}

static int cmp_int(const void* a, const void* b) {
    int x = *(const int*)a;
    int y = *(const int*)b;
    return (x > y) - (x < y);
}

static int deallocs = 0;

static void count_dealloc(void* elem) {
    (void)elem;
    deallocs++;
}

static void test_backend(pinel_lin_kind_t kind) {
    pinel_map_t* map = pinel_map_create(sizeof(int), sizeof(int),
                                        cmp_int, hash_int,
                                        NULL, NULL, kind);

    CHECK(map != NULL);
    CHECK(pinel_map_length(map) == 0);
    CHECK(pinel_map_capacity(map) == 0);

    int missing = 5000;
    CHECK(pinel_map_get(map, &missing) == NULL);
    CHECK(!pinel_map_contains(map, &missing));

    for (int i = 0; i < 1000; i++) {
        int value = i * 10;
        CHECK(pinel_map_put(map, &i, &value) == 0);
        CHECK(pinel_map_length(map) == (size_t)(i + 1));
    }

    CHECK(pinel_map_capacity(map) >= 1000);

    for (int i = 0; i < 1000; i++) {
        int* value = pinel_map_get(map, &i);
        CHECK(value != NULL);
        CHECK(*value == i * 10);
        CHECK(pinel_map_contains(map, &i));
    }

    CHECK(pinel_map_get(map, &missing) == NULL);
    CHECK(!pinel_map_contains(map, &missing));

    
    int key = 10;
    int replacement = -1;
    CHECK(pinel_map_put(map, &key, &replacement) == 0);
    CHECK(pinel_map_length(map) == 1000);
    CHECK(*(int*)pinel_map_get(map, &key) == -1);

    
    for (int i = 0; i < 1000; i += 2) {
        CHECK(pinel_map_remove(map, &i) == 0);
    }

    CHECK(pinel_map_length(map) == 500);
    CHECK(pinel_map_remove(map, &key) == 1);

    for (int i = 0; i < 1000; i++) {
        int* value = pinel_map_get(map, &i);

        if (i % 2 == 0) {
            CHECK(value == NULL);
        }
        else {
            CHECK(value != NULL);
            CHECK(*value == i * 10);
        }
    }

    pinel_map_clear(map);
    CHECK(pinel_map_length(map) == 0);
    CHECK(pinel_map_capacity(map) == 0);

    
    int a = 1;
    int b = 2;
    CHECK(pinel_map_put(map, &a, &b) == 0);
    CHECK(*(int*)pinel_map_get(map, &a) == 2);

    pinel_map_destroy(&map);
    CHECK(map == NULL);
}

static void test_forced_collisions(pinel_lin_kind_t kind) {
    pinel_map_t* map = pinel_map_create(sizeof(int), sizeof(int),
                                        cmp_int, hash_zero,
                                        NULL, NULL, kind);
    CHECK(map != NULL);

    for (int i = 0; i < 100; i++) {
        int value = i;
        CHECK(pinel_map_put(map, &i, &value) == 0);
    }

    CHECK(pinel_map_length(map) == 100);

    for (int i = 0; i < 100; i++) {
        CHECK(*(int*)pinel_map_get(map, &i) == i);
    }

    for (int i = 0; i < 100; i += 3) {
        CHECK(pinel_map_remove(map, &i) == 0);
    }

    CHECK(pinel_map_length(map) == 66);

    for (int i = 0; i < 100; i++) {
        int* value = pinel_map_get(map, &i);

        if (i % 3 == 0) {
            CHECK(value == NULL);
        }
        else {
            CHECK(value != NULL);
            CHECK(*value == i);
        }
    }

    pinel_map_destroy(&map);
}

static void test_default_hash_and_cmp(void) {
    pinel_map_t* map = pinel_map_create(sizeof(int), sizeof(int),
                                        NULL, NULL, NULL, NULL,
                                        PINEL_LIN_ARRAY);
    CHECK(map != NULL);

    for (int i = 0; i < 100; i++) {
        int value = i * 2;
        CHECK(pinel_map_put(map, &i, &value) == 0);
    }

    for (int i = 0; i < 100; i++) {
        CHECK(*(int*)pinel_map_get(map, &i) == i * 2);
    }

    pinel_map_destroy(&map);
}

static void test_string_keys(void) {
    pinel_map_t* map = pinel_map_create(32, sizeof(int),
                                        NULL, NULL, NULL, NULL,
                                        PINEL_LIN_LIST);
    CHECK(map != NULL);

    char key[32] = {0};
    int value = 42;

    strcpy(key, "hello");
    CHECK(pinel_map_put(map, key, &value) == 0);

    memset(key, 0, sizeof(key));
    strcpy(key, "hello");
    CHECK(*(int*)pinel_map_get(map, key) == 42);

    memset(key, 0, sizeof(key));
    strcpy(key, "world");
    CHECK(pinel_map_get(map, key) == NULL);

    value = 7;
    CHECK(pinel_map_put(map, key, &value) == 0);
    CHECK(pinel_map_length(map) == 2);
    CHECK(*(int*)pinel_map_get(map, key) == 7);

    pinel_map_destroy(&map);
}

static void test_dealloc(void) {
    deallocs = 0;

    pinel_map_t* map = pinel_map_create(sizeof(int), sizeof(int),
                                        NULL, NULL,
                                        count_dealloc, count_dealloc,
                                        PINEL_LIN_ARRAY);
    CHECK(map != NULL);

    for (int i = 0; i < 10; i++) {
        CHECK(pinel_map_put(map, &i, &i) == 0);
    }

    CHECK(deallocs == 0);

    
    int key = 0;
    int value = 99;
    CHECK(pinel_map_put(map, &key, &value) == 0);
    CHECK(deallocs == 1);

    
    CHECK(pinel_map_remove(map, &key) == 0);
    CHECK(deallocs == 3);

    pinel_map_clear(map);
    CHECK(deallocs == 3 + 9 * 2);

    pinel_map_destroy(&map);
    CHECK(map == NULL);
}

static void test_invalid_args(void) {
    CHECK(pinel_map_create(0, sizeof(int), NULL, NULL, NULL, NULL,
                           PINEL_LIN_ARRAY) == NULL);
    CHECK(pinel_map_create(sizeof(int), 0, NULL, NULL, NULL, NULL,
                           PINEL_LIN_ARRAY) == NULL);
    CHECK(pinel_map_create(sizeof(int), sizeof(int), NULL, NULL, NULL, NULL,
                           (pinel_lin_kind_t)99) == NULL);

    CHECK(pinel_map_length(NULL) == 0);
    CHECK(pinel_map_capacity(NULL) == 0);
    CHECK(pinel_map_get(NULL, NULL) == NULL);
    CHECK(pinel_map_contains(NULL, NULL) == 0);
    CHECK(pinel_map_put(NULL, NULL, NULL) == 1);
    CHECK(pinel_map_remove(NULL, NULL) == 1);

    pinel_map_clear(NULL);
    pinel_map_destroy(NULL);
}

int main(void) {
    test_backend(PINEL_LIN_ARRAY);
    test_backend(PINEL_LIN_LIST);
    test_forced_collisions(PINEL_LIN_ARRAY);
    test_forced_collisions(PINEL_LIN_LIST);
    test_default_hash_and_cmp();
    test_string_keys();
    test_dealloc();
    test_invalid_args();

    printf("map tests passed\n");
    return EXIT_SUCCESS;
}
