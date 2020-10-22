#ifndef ADJACENCY_MATRIX_H
#define ADJACENCY_MATRIX_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../util/double_comp.h"

typedef struct Adjacency_Matrix_ Adjacency_Matrix;

Adjacency_Matrix *adjacency_matrix_init(size_t n, bool is_directed);
void adjacency_matrix_destroy(Adjacency_Matrix *m);

bool adjacency_matrix_adjacent(const Adjacency_Matrix *graph, size_t v1, size_t v2);
double *adjacency_matrix_neighbors(const Adjacency_Matrix *graph, size_t label);

void adjacency_matrix_add_vertex(Adjacency_Matrix *graph, size_t label);
void adjacency_matrix_remove_vertex(Adjacency_Matrix *graph, size_t label);

bool adjacency_matrix_add_edge(Adjacency_Matrix *graph, size_t x, size_t y, double weight);
bool adjacency_matrix_remove_edge(Adjacency_Matrix *graph, size_t x, size_t y);
double adjacency_matrix_get_edge_value(const Adjacency_Matrix *graph, size_t x, size_t y);
void adjacency_matrix_set_edge_value(Adjacency_Matrix *graph, size_t x, size_t y, double weight);

#endif
