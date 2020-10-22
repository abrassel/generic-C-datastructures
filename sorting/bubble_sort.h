#ifndef Bubble_Sort_H
#define Bubble_Sort_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../util/swapping.h"
#include "../util/comparisons.h"

void bubble_sort(void *arr, size_t nmemb, size_t item_size,
                 const comp_ctx *comp_ctx);

//use this to sort underlying pointers in the array
void bubble_sort_by_ref(void *arr, size_t nmemb, size_t item_size, const comp_ctx *comp_ctx);

#endif
