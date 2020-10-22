#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "../util/destroy.h"
#include "../util/comparisons.h"

#define START 10

typedef struct Array_List_ Array_List;

Array_List *array_list_init(const destroy_ctx *destroy, const eq_ctx *eq);
void array_list_destroy(Array_List *a);

void *array_list_get(const Array_List *a, size_t i);
void array_list_set(Array_List *a, size_t i, void *new_val);
void array_list_remove(Array_List *a, size_t i);
bool array_list_insert(Array_List *a, size_t i, void *item);
bool array_list_contains(const Array_List *a, const void *item);
size_t array_list_size(const Array_List *a);
bool array_list_is_empty(const Array_List *a);

void array_list_push(Array_List *a, void *item);

#endif
