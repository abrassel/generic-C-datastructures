#include "../testing/test.h"
#include "adjacency_list.h"
#include "../spatial/point.h"
#include "traversal.h"
#include "math.h"

static Adjacency_List *create_tree() {
    Adjacency_List *l = adjacency_list_init(false);

    for (int i = 0; i < 9; i++) {
        adjacency_list_add_vertex(l);
    }

    adjacency_list_add_edge(l, 0, 1, 1);
    adjacency_list_add_edge(l, 0, 2, 1);
    adjacency_list_add_edge(l, 0, 3, 1);
    adjacency_list_add_edge(l, 1, 4, 1);
    adjacency_list_add_edge(l, 4, 7, 1);
    adjacency_list_add_edge(l, 3, 5, 1);
    adjacency_list_add_edge(l, 3, 6, 1);

    return l;
}

static void test_bfs_empty() {
    Adjacency_List *l = adjacency_list_init(false);
    assert_null(BFS(l, 3, 5), NULL);
    adjacency_list_destroy(l);
}

static void test_bfs_tree() {
    Adjacency_List *l = create_tree();

    struct path *closest = BFS(l, 0, 7);

    assert_equal_int(closest->label, 7, NULL);
    assert_equal_int(closest->prev->label, 4, NULL);
    assert_equal_int(closest->prev->prev->label, 1, NULL);
    assert_equal_int(closest->prev->prev->prev->label, 0, NULL);
    assert_null(closest->prev->prev->prev->prev, NULL);

    path_destroy(closest);

    closest = BFS(l, 0, 2);

    assert_equal_int(closest->label, 2, NULL);
    assert_equal_int(closest->prev->label, 0, NULL);
    assert_null(closest->prev->prev, NULL);

    path_destroy(closest);

    closest = BFS(l, 4, 3);

    assert_equal_int(closest->label, 3, NULL);
    assert_equal_int(closest->prev->label, 0, NULL);
    assert_equal_int(closest->prev->prev->label, 1, NULL);
    assert_equal_int(closest->prev->prev->prev->label, 4, NULL);
    assert_null(closest->prev->prev->prev->prev, NULL);

    path_destroy(closest);
    
    adjacency_list_destroy(l);
}
static void test_bfs_loops() {
    Adjacency_List *l = create_tree();

    //add a loop between 0 and 6 and 5 and 6
    adjacency_list_add_edge(l, 5, 6, 1);
    adjacency_list_add_edge(l, 0, 6, 1);

    //BFS will still find shortest path
    struct path *path = BFS(l, 0, 6);
    assert_equal_int(path->label, 6, NULL);
    assert_equal_int(path->prev->label, 0, NULL);
    assert_null(path->prev->prev, NULL);

    path_destroy(path);
    adjacency_list_destroy(l);
}

static void test_bfs_unreachable() {
    Adjacency_List *l = create_tree();
    assert_null(BFS(l, 4, 8), NULL);
    adjacency_list_destroy(l);
}

static void test_dfs_empty() {
    Adjacency_List *l = adjacency_list_init(false);
    assert_null(DFS(l, 3, 5), NULL);
    adjacency_list_destroy(l);
}
static void test_dfs_tree() {
    Adjacency_List *l = create_tree();

    struct path *closest = DFS(l, 0, 7);

    assert_equal_int(closest->label, 7, NULL);
    assert_equal_int(closest->prev->label, 4, NULL);
    assert_equal_int(closest->prev->prev->label, 1, NULL);
    assert_equal_int(closest->prev->prev->prev->label, 0, NULL);
    assert_null(closest->prev->prev->prev->prev, NULL);

    path_destroy(closest);

    closest = DFS(l, 0, 2);

    assert_equal_int(closest->label, 2, NULL);
    assert_equal_int(closest->prev->label, 0, NULL);
    assert_null(closest->prev->prev, NULL);

    path_destroy(closest);

    closest = DFS(l, 4, 3);

    assert_equal_int(closest->label, 3, NULL);
    assert_equal_int(closest->prev->label, 0, NULL);
    assert_equal_int(closest->prev->prev->label, 1, NULL);
    assert_equal_int(closest->prev->prev->prev->label, 4, NULL);
    assert_null(closest->prev->prev->prev->prev, NULL);

    path_destroy(closest);
    
    adjacency_list_destroy(l);
}
static void test_dfs_loops() {
    Adjacency_List *l = create_tree();

    //add a loop between 0 and 6 and 5 and 6
    adjacency_list_add_edge(l, 5, 6, 1);
    adjacency_list_remove_edge(l, 0, 3);
    //put 6 before 3
    adjacency_list_add_edge(l, 0, 6, 1);
    adjacency_list_add_edge(l, 0, 3, 1);

    //DFS will go down the 3 path first
    struct path *path = DFS(l, 0, 6);
    assert_equal_int(path->label, 6, NULL);
    assert_equal_int(path->prev->label, 3, NULL);
    assert_equal_int(path->prev->prev->label, 0, NULL);
    assert_null(path->prev->prev->prev, NULL);

    path_destroy(path);
    adjacency_list_destroy(l);
}

static void test_dfs_unreachable() {
    Adjacency_List *l = create_tree();
    assert_null(DFS(l, 4, 8), NULL);
    adjacency_list_destroy(l);
}

static Adjacency_List *make_dijsktras() {
    Adjacency_List *l = adjacency_list_init(false);

    for (int i = 0; i < 10; i++) {
        adjacency_list_add_vertex(l);            
    }

    adjacency_list_add_edge(l, 0, 1, 4);
    adjacency_list_add_edge(l, 0, 7, 6);
    adjacency_list_add_edge(l, 1, 2, 10);
    adjacency_list_add_edge(l, 1, 7, 11);
    adjacency_list_add_edge(l, 7, 8, 7);
    adjacency_list_add_edge(l, 7, 6, 1);
    adjacency_list_add_edge(l, 2, 8, 2);
    adjacency_list_add_edge(l, 8, 6, 6);
    adjacency_list_add_edge(l, 2, 3, 7);
    adjacency_list_add_edge(l, 2, 5, 4);
    adjacency_list_add_edge(l, 6, 5, 2);
    adjacency_list_add_edge(l, 3, 5, 14);
    adjacency_list_add_edge(l, 3, 4, 9);
    adjacency_list_add_edge(l, 4, 5, 100);

    return l;
}

static void test_dijsktra_all() {
    Adjacency_List *l = make_dijsktras();

    struct prev_step *paths = dijsktras_all(l, 0);

    struct prev_step correct[] = {
        {0, {false}},
        {4, {.option = {true, 0}}},
        {13, {.option = {true, 5}}},
        {20, {.option = {true, 2}}},
        {29, {.option = {true, 3}}},
        {9, {.option = {true, 6}}},
        {7, {.option = {true, 7}}},
        {6, {.option = {true, 0}}},
        {13,{.option = {true, 7}}},
        {DBL_MAX, {false}}
    };

    for (size_t i = 0; i < 10; i++) {
        assert_true(
            eq_d(
                correct[i].dist_from_src,
                paths[i].dist_from_src
                ),
            NULL
            );
        assert_true(
            correct[i].prev.has_last == paths[i].prev.has_last,
            NULL
            );
        if (correct[i].prev.has_last) {
            assert_equal_int(
                correct[i].prev.option.val,
                paths[i].prev.option.val,
                NULL
                );
        }
    }

    free(paths);
    adjacency_list_destroy(l);
}

static void test_dijsktra_single() {
    Adjacency_List *l = make_dijsktras();

    struct prev_step *paths = dijsktras_single(l, 0, 2);

    struct prev_step correct[] = {
        {0, .prev = {false}},
        {4, .prev = {.option = {true, 0}}},
        {13, .prev = {.option = {true, 5}}},
        {20, .prev = {.option = {true, 2}}},
        {29, .prev = {.option = {true, 3}}},
        {9, .prev = {.option = {true, 6}}},
        {7, .prev = {.option = {true, 7}}},
        {6, .prev = {.option = {true, 0}}},
        {13, .prev = {.option = {true, 8}}},
        {DBL_MAX, .prev = {false}}
    };

    size_t path[] = {
        0, 7, 6, 5, 2
    };

    for (size_t j = 0; j < 5; j++) {
        size_t i = path[j];
        assert_true(
            eq_d(
                correct[i].dist_from_src,
                paths[i].dist_from_src
                ),
            NULL
            );
        assert_true(
            correct[i].prev.has_last == paths[i].prev.has_last,
            NULL
            );
        if (correct[i].prev.has_last) {
            assert_equal_int(
                correct[i].prev.option.val,
                paths[i].prev.option.val,
                NULL
                );
        }
    }

    free(paths);
    adjacency_list_destroy(l);
}

static double zero_fun(size_t source, size_t dest, const Adjacency_List *graph, const void *ctx) {
    (void) source;
    (void) dest;
    (void) graph;
    (void) ctx;

    return 0;
}

static void test_astar_trivial() {
    Adjacency_List *l = make_dijsktras();

    heuristic_ctx trivial = {
        zero_fun,
        NULL
    };
    
    struct prev_step *paths = astar(l, 0, 2, &trivial);

    struct prev_step correct[] = {
        {0, .prev = {false}},
        {4, .prev = {.option = {true, 0}}},
        {13, .prev = {.option = {true, 5}}},
        {20, .prev = {.option = {true, 2}}},
        {29, .prev = {.option = {true, 3}}},
        {9, .prev = {.option = {true, 6}}},
        {7, .prev = {.option = {true, 7}}},
        {6, .prev = {.option = {true, 0}}},
        {13, .prev = {.option = {true, 8}}},
        {DBL_MAX, .prev = {false}}
    };

    size_t path[] = {
        0, 7, 6, 5, 2
    };

    for (size_t j = 0; j < 5; j++) {
        size_t i = path[j];
        assert_true(
            eq_d(
                correct[i].dist_from_src,
                paths[i].dist_from_src
                ),
            NULL
            );
        assert_true(
            correct[i].prev.has_last == paths[i].prev.has_last,
            NULL
            );
        if (correct[i].prev.has_last) {
            assert_equal_int(
                correct[i].prev.option.val,
                paths[i].prev.option.val,
                NULL
                );
        }
    }

    free(paths);
    adjacency_list_destroy(l);  
}

static double dist(size_t source, size_t dest, const Adjacency_List *graph, const void *ctx) {
    (void) graph;
    Point2D *points = (Point2D *) ctx;

    return sqrt(squared_distance(points[source], points[dest], 2));
}

static void test_astar_aggressive() {
    Adjacency_List *l = adjacency_list_init(false);
    for (size_t i = 0; i < 6; i++) {
        adjacency_list_add_vertex(l);
    }

    adjacency_list_add_edge(l, 0, 1, sqrt(10));
    adjacency_list_add_edge(l, 0, 2, sqrt(5));
    adjacency_list_add_edge(l, 1, 3, sqrt(13));
    adjacency_list_add_edge(l, 0, 4, 4*sqrt(2));
    adjacency_list_add_edge(l, 4, 5, sqrt(2));

    Point2D points[6] = {
        {0, 0},
        {1, 3},
        {1, 2},
        {4, 1},
        {4, 4},
        {5, 5}
    };
    
    heuristic_ctx euclidean_distance_ctx = {
        dist,
        (void *) points
    };

    struct prev_step *path = astar(l, 0, 5, &euclidean_distance_ctx);

    assert_true(path[5].prev.has_last, NULL);
    assert_equal((void *) (uintptr_t) path[5].dist_from_src, (void *) (uintptr_t) (5 * sqrt(2)), &raw_double_eq_ctx, NULL);
    assert_equal_int(path[5].prev.option.val, 4, NULL);

    assert_true(path[4].prev.has_last, NULL);
    assert_equal((void *) (uintptr_t) path[4].dist_from_src, (void *) (uintptr_t) (4 * sqrt(2)), &raw_double_eq_ctx, NULL);
    assert_equal_int(path[4].prev.option.val, 0, NULL);

    free(path);
    adjacency_list_destroy(l);
}

int main() {
    test_bfs_empty();
    test_bfs_tree();
    test_dfs_empty();
    test_dfs_tree();
    test_bfs_loops();
    test_dfs_loops();
    test_bfs_unreachable();
    test_dfs_unreachable();

    test_dijsktra_all();
    test_dijsktra_single();

    test_astar_trivial();
    test_astar_aggressive();
}
