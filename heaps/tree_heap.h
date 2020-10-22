#ifndef TREE_HEAP_H
#define TREE_HEAP_H

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "../stack/stack.h"
#include "../util/comparisons.h"
#include "../util/destroy.h"

typedef struct Tree_Heap_ Tree_Heap;

Tree_Heap *tree_heap_init(const comp_ctx *comp_ctx, const destroy_ctx *destroy_key, const destroy_ctx *destroy_value);
void tree_heap_destroy(Tree_Heap *heap);
void tree_heap_insert(Tree_Heap *heap, void *key, void *element);
bool tree_heap_is_empty(const Tree_Heap *heap);
void *tree_heap_pull(Tree_Heap *heap);
void *tree_heap_peek(const Tree_Heap *heap);

size_t tree_heap_size(const Tree_Heap *heap);

#endif
