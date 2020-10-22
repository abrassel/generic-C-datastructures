#ifndef HEAP_SORT_H
#define HEAP_SORT_H

#include <string.h>

#include "../heaps/arr_heap.h"
#include "../util/comparisons.h"

void heap_sort(void *arr, size_t nmemb, size_t item_size, const comp_ctx *comp_ctx);

#endif
