#ifndef SORTED_ARRAYS_H
#define SORTED_ARRAYS_H

#include "../sorting/bubble_sort.h"
#include "comparisons.h"

struct array_eq {
    size_t nmemb, item_size;
    const eq_ctx *item_eq;
};

bool array_contains(void *arr, size_t nmemb, size_t item_size, const void *key,
                    const eq_ctx *eq_ctx);

int array_ind(void *arr, size_t nmemb, size_t item_size, const void *key,
              const eq_ctx *eq_ctx);

bool eq_arrs(const void *arr1, const void *arr2, const void *ctx);

#endif
