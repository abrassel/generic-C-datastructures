#ifndef SORTED_ARRAYS_H
#define SORTED_ARRAYS_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#include "../sorting/bubble_sort.h"
#include "comparisons.h"

#define COL_SIZE 5

size_t sorted_array_median_ind(size_t n);

size_t split_sorted_array_on_median(void *arr1, size_t nmemb, size_t item_size, void *arr2, void *key);

/*
 * Find the ith smallest element in an unsorted array
 **/
void *median_of_medians(void *arr, size_t nmemb, size_t item_size, size_t i,
                        const comp_ctx *comp_ctx);

size_t partition(void *arr, size_t nmemb, size_t item_size, void *pivot, const comp_ctx *comp_ctx);
#endif
