#include "traversal.h"

static void free_path(void *v, const void *ctx) {
    (void) ctx;
    struct path *cur = (struct path *) v;
    path_destroy(cur);
}

//publically visible API
void path_destroy(struct path *cur) {
    //iterate up the chain.
    //if came from the only remaining reference, free the path
    //otherwise, decrement the reference and stop

    //this prevents multiple freeing attempts
    //or premature frees for the used path
    while(cur) {
        if (cur->ref_count != 0) {
            break;
        }

        //freeing and decrementing next count
        if (cur->prev) {
            cur->prev->ref_count--;
        }

        struct path *temp = cur;
        cur = cur->prev;
        free(temp);
    }
}

static destroy_ctx destroy_path_ctx = {free_path, NULL};

static struct path *create_path_entry(struct path *prev, size_t next) {
    struct path *cur_step = malloc(sizeof(*cur_step));

    cur_step->label = next;
    cur_step->prev = prev;
    cur_step->ref_count = 0;
    if (prev) {
        prev->ref_count++;
    }

    return cur_step;
}

struct path *generic_search(Adjacency_List *graph, size_t source, size_t dest, bool is_queue) {
    size_t IVI = adjacency_list_size(graph);

    //need |E| = |V|^2 space because in fully connected
    //queue removes one at each step, enqueues n-1
    //each time, one fewer vertex can be enqueued
    //but guass's sum still runs in n^2
    //this is not a tight upper bound
    union {
        Circ_Queue *q;
        Stack *s;
    } frontier;

    if (is_queue) {
      frontier.q = circ_queue_init(IVI * IVI + 1, &destroy_path_ctx);
    }
    else {
      frontier.s = stack_init(IVI * IVI + 1, &destroy_path_ctx);
    }


    //track all of the so far seen vertices
    //adding copies of labels in case of premature frees from paths.  therefore, must have cleanup in hashtable
    Hashtable *explored = hashtable_init(&identity_hash_ctx, linear_probing, &int_eq_ctx, &naive_destroy_ctx, (double) 2 / 3);

    //seed the queue with the root path
    if (is_queue) {
        circ_queue_insert(frontier.q, (void *) create_path_entry(NULL, source));
    }
    else {
        stack_push(frontier.s, (void *) create_path_entry(NULL, source));
    }

    bool success = false;

    struct path *cur;
    while (!((is_queue && circ_queue_is_empty(frontier.q)) || (!is_queue && stack_is_empty(frontier.s)))) {
        cur = (struct path *) (is_queue ? circ_queue_pop(frontier.q) : stack_pop(frontier.s));

        //terminate if the goal condition is met
        if (cur->label == dest) {
            success = true;
            break;
        }

        //check to make sure that the current node is unvisited
        if (hashtable_contains(explored, (void *) cur)) {
            goto cleanup_and_continue;
        }
        //if it is, mark as visited
        else {
            int *found_copy = malloc(sizeof(*found_copy));
            *found_copy = cur->label;
            hashtable_add(explored, (void *) found_copy);
        }

        Singly_Linked_List *neighbors = adjacency_list_neighbors(graph, cur->label);

        if (!neighbors) {
            goto cleanup_and_continue;
        }

        Singly_Linked_List_Iter *neighbor_iter =
            singly_linked_list_iter_init(neighbors);

        //explore each neighbor
        while (singly_linked_list_iter_has_next(neighbor_iter)) {
            Edge *neighbor = (Edge *) singly_linked_list_iter_next(neighbor_iter);
            if (!hashtable_contains(explored, (void *) neighbor)) {
                if (is_queue) {
                    circ_queue_insert(frontier.q, (void *) create_path_entry(cur, neighbor->label));
                }
                else {
                    stack_push(frontier.s, (void *) create_path_entry(cur, neighbor->label));
                }
            }
        }

        singly_linked_list_iter_destroy(neighbor_iter);

    cleanup_and_continue:
        //only frees if there are no remaining references
        //or, equivalently, no neighbors
        free_path(cur, NULL);

    }
    if (is_queue) {
        circ_queue_destroy(frontier.q);
    }
    else {
        stack_destroy(frontier.s);
    }
    hashtable_destroy(explored);
    return success ? cur : NULL;
}

struct path *BFS(Adjacency_List *graph, size_t source, size_t dest) {
    return generic_search(graph, source, dest, true);
}
struct path *DFS(Adjacency_List *graph, size_t source, size_t dest) {
    return generic_search(graph, source, dest, false);
}

static size_t uintptr_t_hash(const void *value, size_t len, const void *ctx) {
    (void) ctx;
    return (uintptr_t) value % len;
}

static hash_ctx uintptr_t_hash_ctx = {uintptr_t_hash, NULL};

static struct prev_step *dijsktras_internal(const Adjacency_List *graph, size_t source, size_t *dest) {
    //its possible that architecture pointer and double size are not equal.  complain if so
    _Static_assert (sizeof(double) == sizeof(uintptr_t), "double and pointer architecture size misaligned");

    size_t IVI = adjacency_list_size(graph);
    if (IVI == 0) {
        return NULL;
    }
  /*
   * visited set: hashtable
   * best paths: array of vertex label -> {distance, path_pointer}
   * frontier: pqueue of {distance, path_pointer}
   *
   * Pick the best from the frontier.
   * 1) if the distance is worse than the current listed best, skip this loop
   * 2) set best path so far for this vertex
   * 3) calculate new min distance for each neighbor that has not yet been
visited
   * 4) add new neighbor to priority queue if it is better than current best
   */

    //different approach than above - push path so far
    Hashtable *visited = hashtable_init(&uintptr_t_hash_ctx, linear_probing, &naive_eq_ctx, NULL, (double) 2 / 3);

    //could use a hashmap if using non-integer labels.  array will do in this case.
    struct prev_step *dist = malloc(sizeof(*dist) * IVI);
    for (size_t i = 0; i < IVI; i++) {
        dist[i].prev.has_last = false;
        dist[i].dist_from_src = DBL_MAX;
    }

    dist[source].dist_from_src = 0;

    //the heap is a max heap.  will need to compose an inverted comparison function
    comp_ctx inv_raw_double_comp_ctx = {
        inv_comp,
        &raw_double_comp_ctx
    };

    //use pointer to direct edge weights in adjacency list representation
    //track the path so far (including self)
    Array_Heap *frontier = array_heap_init(IVI * IVI, &inv_raw_double_comp_ctx, NULL, NULL);

    //seed the frontier with the source
    array_heap_insert(frontier, (void *) (uintptr_t) 0, (void *) (uintptr_t) source);

    bool answer_found = false;
    while (!answer_found && !array_heap_is_empty(frontier)) {
        size_t cur_vertex = (size_t) (uintptr_t) array_heap_pull_v(frontier);

        //possible to reach again with the same distance
        //this is ok though

        if (dest && *dest == cur_vertex) {
            answer_found = true;
            continue;
        }

        hashtable_add(visited, (void *) (uintptr_t) cur_vertex);

        //update neighbor best
        Singly_Linked_List *neighbors = adjacency_list_neighbors(graph, cur_vertex);
        if (!neighbors) {
            continue;
        }

        Singly_Linked_List_Iter *neighbors_iter = singly_linked_list_iter_init(neighbors);

        while (singly_linked_list_iter_has_next(neighbors_iter)) {
            Edge *e = singly_linked_list_iter_next(neighbors_iter);

            //calculate distance guess for the neighbor.
            double new_guess = dist[cur_vertex].dist_from_src + e->weight;

            //if distance guess is better, update frontier and best guess
            if (new_guess < dist[e->label].dist_from_src && !hashtable_contains(visited, (void *) (uintptr_t) e->label)) {
                dist[e->label].prev.option.has_last = true;
                dist[e->label].prev.option.val = cur_vertex;
                dist[e->label].dist_from_src = new_guess;
                array_heap_insert(frontier, (void *) (uintptr_t) new_guess, (void *) (uintptr_t) e->label);
            }

        }

        singly_linked_list_iter_destroy(neighbors_iter);
    }

    hashtable_destroy(visited);
    array_heap_destroy(frontier);

    return dist;
}


struct prev_step *dijsktras_all(const Adjacency_List *graph, size_t source) {
    return dijsktras_internal(graph, source, NULL);
}
struct prev_step *dijsktras_single(const Adjacency_List *graph, size_t source, size_t dest) {
    return dijsktras_internal(graph, source, &dest);
}


//for educational reasons, code is copy and pasted instead of modified.
struct prev_step *astar(const Adjacency_List *graph, size_t source, size_t dest, const heuristic_ctx *h_ctx) {
    //its possible that architecture pointer and double size are not equal.  complain if so
    _Static_assert (sizeof(double) == sizeof(uintptr_t), "double and pointer architecture size misaligned");

    size_t IVI = adjacency_list_size(graph);
    if (IVI == 0) {
        return NULL;
    }
  /*
   * visited set: hashtable
   * best paths: array of vertex label -> {distance, path_pointer}
   * frontier: pqueue of {distance, path_pointer}
   *
   * Pick the best from the frontier.
   * 1) if the distance is worse than the current listed best, skip this loop
   * 2) set best path so far for this vertex
   * 3) calculate new min distance for each neighbor that has not yet been
visited
   * 4) add new neighbor to priority queue if it is better than current best
   */

    //different approach than above - push path so far
    Hashtable *visited = hashtable_init(&uintptr_t_hash_ctx, linear_probing, &naive_eq_ctx, NULL, (double) 2 / 3);

    //could use a hashmap if using non-integer labels.  array will do in this case.
    struct prev_step *dist = malloc(sizeof(*dist) * IVI);
    for (size_t i = 0; i < IVI; i++) {
        dist[i].prev.has_last = false;
        dist[i].dist_from_src = DBL_MAX;
    }

    dist[source].dist_from_src = 0;

    //the heap is a max heap.  will need to compose an inverted comparison function
    comp_ctx inv_raw_double_comp_ctx = {
        inv_comp,
        &raw_double_comp_ctx
    };

    //use pointer to direct edge weights in adjacency list representation
    //track the path so far (including self)
    Array_Heap *frontier = array_heap_init(IVI * IVI, &inv_raw_double_comp_ctx, NULL, NULL);

    //seed the frontier with the source
    array_heap_insert(frontier, (void *) (uintptr_t) 0, (void *) (uintptr_t) source);

    bool answer_found = false;
    while (!answer_found && !array_heap_is_empty(frontier)) {
        size_t cur_vertex = (size_t) (uintptr_t) array_heap_pull_v(frontier);

        if (dest == cur_vertex) {
            answer_found = true;
            continue;
        }

        hashtable_add(visited, (void *) (uintptr_t) cur_vertex);

        //update neighbor best
        Singly_Linked_List *neighbors = adjacency_list_neighbors(graph, cur_vertex);
        if (!neighbors) {
            continue;
        }

        Singly_Linked_List_Iter *neighbors_iter = singly_linked_list_iter_init(neighbors);

        while (singly_linked_list_iter_has_next(neighbors_iter)) {
            Edge *e = singly_linked_list_iter_next(neighbors_iter);

            //calculate distance guess for the neighbor.
            double g = dist[cur_vertex].dist_from_src + e->weight;
            double h = h_ctx->h(e->label, dest, graph, h_ctx->ctx);
            double new_guess = g + h;

            //if distance guess is better, update frontier and best guess
            if (new_guess < dist[e->label].dist_from_src && !hashtable_contains(visited, (void *) (uintptr_t) e->label)) {
                dist[e->label].prev.option.has_last = true;
                dist[e->label].prev.option.val = cur_vertex;
                dist[e->label].dist_from_src = g;
                array_heap_insert(frontier,  * (void **) & new_guess, (void *) (uintptr_t) e->label);
            }

        }

        singly_linked_list_iter_destroy(neighbors_iter);

    }

    hashtable_destroy(visited);
    array_heap_destroy(frontier);

    return dist;
}
