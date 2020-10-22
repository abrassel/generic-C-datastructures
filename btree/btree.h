#ifndef BTREE_H
#define BTREE_H

#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "../util/interval_search.h"
#include "../util/kvpair.h"
#include "../util/comparisons.h"
#include "../util/arrays.h"
#include "../stack/stack.h"
#include "../util/destroy.h"
#include "../util/destroy_helpers.h"

typedef struct Btree_ Btree;

/* setup and teardown **/

Btree *btree_init(size_t p, const comp_ctx *comp, const destroy_ctx *destroy_key, const destroy_ctx *destroy_value);
void btree_destroy(Btree *tree);

/* insertion search and deletion **/

void btree_insert(Btree *tree, void *key, void *value);
void *btree_search(const Btree *tree, const void *key);
int btree_size(const Btree *tree);
bool btree_is_empty(const Btree *tree);

void btree_print(const Btree *tree, void (*print_key)(const void *key));

#endif
