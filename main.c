#include <pinel/array.h>

#include <stdio.h>

int main() {
    pinel_array_t* arr = pinel_array_create(sizeof(int), NULL, NULL);

    for (size_t i = 0; i < 5; i++) {
        pinel_array_pushB(arr, &i);
    }
    
    printf("[ ");
    for (size_t i = 0; i < pinel_array_length(arr); i++) {
        printf("%d, ", *(int*)pinel_array_at(arr, i));
    }
    printf("]\n");

    pinel_array_destroy(&arr);

    return 0;
}