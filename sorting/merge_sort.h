#ifndef MERGE_SORT_H
#define MERGE_SORT_H

#include <stdlib.h>
#include <string.h>

#include "../util/comparisons.h"

void merge(void *left, size_t l_n, void *right, size_t r_n, void *arr, const comp_ctx *comp_ctx, size_t item_size);

void merge_sort(void *arr, size_t nmemb, size_t item_size, const comp_ctx *comp_ctx);

#endif
