#ifndef INTERVAL_SEARCH_H
#define INTERVAL_SEARCH_H

#include <limits.h>
#include <stddef.h>

#include "comparisons.h"

/**
 * Return space between seperators.  0 = left of array, n = right.
 * If there is an exact match, -index is returned (or INT_MIN if 0)
 */
int interval_search(const void *arr, size_t n, size_t elm_size, const void *key, const comp_ctx *comp_ctx);

#endif
