#ifndef PR_QUADTREE_H
#define PR_QUADTREE_H

#include <stdlib.h>
#include <stdbool.h>
#include "../stack/stack.h"
#include "point.h"
#include "../util/array_lookup.h"

typedef struct PRQuadtree_ PRQuadtree;

PRQuadtree *prquadtree_init(Rectangle boundary);
void prquadtree_destroy(PRQuadtree *tree);

void prquadtree_add(PRQuadtree *tree, Point2D point);
bool prquadtree_search(const PRQuadtree *tree, const Point2D point);
void prquadtree_remove(PRQuadtree *tree, const Point2D point);

// nn
// knn

// range query

size_t prquadtree_size(const PRQuadtree *tree);
bool prquadtree_is_empty(const PRQuadtree *tree);

size_t prquadtree_height(const PRQuadtree *tree);

#endif
