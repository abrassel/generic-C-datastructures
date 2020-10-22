#ifndef KDTREE_H
#define KDTREE_H

#include <stdlib.h>
#include <stdbool.h>
#include <float.h>

#include "../stack/stack.h"
#include "../util/printing.h"
#include "../util/comparisons.h"
#include "../util/destroy.h"
#include "../util/destroy_helpers.h"
#include "point.h"

typedef struct KDTree_ KDTree;

//used for queries where point and value are of interest
struct KD_Point {
    double *point;
    void *value;
};

KDTree *kdtree_init(size_t k, const destroy_ctx *destroy_key, const destroy_ctx *destroy_value);
void kdtree_destroy(KDTree *tree);

void *kdtree_search(const KDTree *tree, const double *point);
void kdtree_insert(KDTree *tree, double *point, void *val);
void *kdtree_remove(KDTree *tree, const double *point);

struct KD_Point *kdtree_nn(const KDTree *tree, const double *anchor);

bool kdtree_is_empty(const KDTree *tree);
size_t kdtree_size(const KDTree *tree);

void kdtree_print(const KDTree *tree, void (*print_value)(const void *val));

#endif
