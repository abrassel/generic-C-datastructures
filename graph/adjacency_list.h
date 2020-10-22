#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../list/arraylist.h"
#include "../util/comparisons.h"
#include "../list/singly_linked_list.h"
#include "../util/double_comp.h"
#include "../util/comparison_helpers.h"

typedef struct Adjacency_List_ Adjacency_List;
typedef struct {
    size_t label;
    double weight;
} Edge;

Adjacency_List *adjacency_list_init(bool is_directed);
void adjacency_list_destroy(Adjacency_List *graph);

bool adjacency_list_adjacent(const Adjacency_List *graph, size_t v1, size_t v2);
Singly_Linked_List *adjacency_list_neighbors(const Adjacency_List *graph, size_t label);

void adjacency_list_add_vertex(Adjacency_List *graph);

bool adjacency_list_add_edge(Adjacency_List *graph, size_t x, size_t y, double weight);
void adjacency_list_remove_edge(Adjacency_List *graph, size_t x, size_t y);
double adjacency_list_get_edge_value(const Adjacency_List *graph, size_t x, size_t y);
void adjacency_list_set_edge_value(Adjacency_List *graph, size_t x, size_t y,
                                   double weight);

size_t adjacency_list_size(const Adjacency_List *graph);

#endif
