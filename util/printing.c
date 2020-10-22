#include "printing.h"

void print_double_array(const double *arr, size_t len) {
    printf("[");
    for (size_t i = 0; i < len; i++) {
        printf("%.3lf, ", arr[i]);
    }
    printf("]");
}

void print_int_array(const int *arr, size_t len) {
    printf("[");
    for (size_t i = 0; i < len; i++) {
        printf("%d, ", arr[i]);
    }
    printf("]");
}
