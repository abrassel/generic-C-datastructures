#include "counting_sort.h"

void counting_sort(int *arr, size_t n, int min, int max) {
    int *counts = calloc(max - min + 1, sizeof(int));

    //count up each number in counts
    for (size_t i = 0; i < n; i++) {
        counts[arr[i] - min]++;
    }

    //dump counts back into arr
    for (int i = 0, offset = 0; i <= max - min; i++) {
        for (int j = 0; j < counts[i]; j++, offset++) {
            arr[offset] = i + min;
        }
    }

    free(counts);
}
