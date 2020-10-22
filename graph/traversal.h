#ifndef GRAPH_TRAVERSAL_H
#define GRAPH_TRAVERSAL_H

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <float.h>

#include "adjacency_list.h"
#include "../queue/circ_queue.h"
#include "../stack/stack.h"
#include "../set/hashtable.h"
#include "../util/destroy_helpers.h"
#include "../heaps/arr_heap.h"

struct path {
    size_t label;
    struct path *prev;
    size_t ref_count;
};

struct prev_step {
    double dist_from_src;
    union {
        bool has_last;
        struct {
            bool has_last;
            size_t val;
        } option;
    } prev;
};

typedef double (*heuristic_fn)(size_t label, size_t dest, const Adjacency_List *graph, const void *ctx);

typedef struct {
    heuristic_fn h;
    void *ctx;
} heuristic_ctx;

void path_destroy(struct path *cur);

struct path *BFS(Adjacency_List *graph, size_t source, size_t dest);
struct path *DFS(Adjacency_List *graph, size_t source, size_t dest);
struct prev_step *dijsktras_all(const Adjacency_List *graph, size_t source);
struct prev_step *dijsktras_single(const Adjacency_List *graph, size_t source,
                                   size_t dest);

struct prev_step *astar(const Adjacency_List *graph, size_t source, size_t dest, const heuristic_ctx *h);

#endif
