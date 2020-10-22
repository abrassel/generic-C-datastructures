#include "adjacency_list.h"

typedef struct {
    size_t label;
    Singly_Linked_List *neighbors;
} Vertex;

struct Adjacency_List_ {
    bool is_directed;
    Array_List *vertices;
};

static void free_fun(void *value, const void *ctx) {
    (void) ctx;
    free(value);
}

static void free_vertex_fn(void *value, const void *ctx) {
  (void)ctx;
  Vertex *v = (Vertex *)value;
  singly_linked_list_destroy(v->neighbors);
  free(v);
}

static destroy_ctx destroy_linked_list_ctx = {free_vertex_fn, NULL};
static destroy_ctx free_ctx = {free_fun, NULL};

Adjacency_List *adjacency_list_init(bool is_directed) {
    Adjacency_List *a = malloc(sizeof(*a));

    a->vertices = array_list_init(&destroy_linked_list_ctx, &int_eq_ctx);
    a->is_directed = is_directed;
    return a;
}

void adjacency_list_destroy(Adjacency_List *graph) {
    array_list_destroy(graph->vertices);
    free(graph);
}

bool adjacency_list_adjacent(const Adjacency_List *graph, size_t v1, size_t v2) {
    Vertex *v = array_list_get(graph->vertices, v1);
    return v && singly_linked_list_contains(v->neighbors, &v2);
}

Singly_Linked_List *adjacency_list_neighbors(const Adjacency_List *graph, size_t label) {
    Vertex *v = array_list_get(graph->vertices, label);
    return v ? v->neighbors : NULL;
}

void adjacency_list_add_vertex(Adjacency_List *graph) {
    Vertex *v = malloc(sizeof(*v));
    v->label = array_list_size(graph->vertices);
    v->neighbors = singly_linked_list_init(&int_eq_ctx, &free_ctx);

    array_list_push(graph->vertices, v);
}

bool adjacency_list_add_edge(Adjacency_List *graph, size_t x, size_t y, double weight) {
    Vertex *v = array_list_get(graph->vertices, x);
    Vertex *v2 = array_list_get(graph->vertices, y);
    if (!v || !v2) {
        return false;
    }

    Edge *e = malloc(sizeof(*e));
    e->label = y;
    e->weight = weight;

    singly_linked_list_push(v->neighbors, e);

    if (!graph->is_directed) {
        e = malloc(sizeof(*e));
        e->label = x;
        e->weight = weight;
    }

    singly_linked_list_push(v2->neighbors, e);

    return true;
}

void adjacency_list_remove_edge(Adjacency_List *graph, size_t x, size_t y) {
    Vertex *v = array_list_get(graph->vertices, x);
    singly_linked_list_remove_elm(v->neighbors, &y);

    if (!graph->is_directed) {
        v = array_list_get(graph->vertices, y);
        singly_linked_list_remove_elm(v->neighbors, &x);
    }
}

double adjacency_list_get_edge_value(const Adjacency_List *graph, size_t x, size_t y) {
    Vertex *v = array_list_get(graph->vertices, x);
    return ((Edge *)singly_linked_list_get_elm(v->neighbors, &y))->weight;
}

void adjacency_list_set_edge_value(Adjacency_List *graph, size_t x, size_t y, double weight) {
    Vertex *v = array_list_get(graph->vertices, x);
    Edge *e = singly_linked_list_get_elm(v->neighbors, &y);
    if (e) {
        e->weight = weight;
    }
}

size_t adjacency_list_size(const Adjacency_List *graph) {
    return array_list_size(graph->vertices);
}
