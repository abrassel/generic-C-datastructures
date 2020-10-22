#include "../testing/test.h"
#include "adjacency_list.h"

#include <stdbool.h>

static void empty_assertions(Adjacency_List *l) {
    assert_false(adjacency_list_adjacent(l, 0, 2), NULL);
    assert_null(adjacency_list_neighbors(l, 0), NULL);

    assert_false(adjacency_list_add_edge(l, 2, 3, 5), NULL);
}

/*     6
 * 1 ------2
 *   \     |
 * -1 \    |-5
 *     \   |
 *    5 --\|    2.3
 * 0-------3-------4
 **/
static Adjacency_List *before_each() {
    Adjacency_List *l = adjacency_list_init(false);
    empty_assertions(l);

    adjacency_list_add_vertex(l);
    adjacency_list_add_vertex(l);
    adjacency_list_add_vertex(l);
    adjacency_list_add_vertex(l);
    adjacency_list_add_vertex(l);

    assert_true(adjacency_list_add_edge(l, 0, 3, 5), NULL);
    assert_true(adjacency_list_add_edge(l, 3, 4, 2.3), NULL);
    assert_true(adjacency_list_add_edge(l, 3, 2, -5), NULL);
    assert_true(adjacency_list_add_edge(l, 1, 3, -1), NULL);
    assert_true(adjacency_list_add_edge(l, 1, 2, 5), NULL);
    return l;
}

static void test_adjacencies() {
    Adjacency_List *l = before_each();

    assert_true(adjacency_list_adjacent(l, 0, 3), NULL);
    assert_true(adjacency_list_adjacent(l, 3, 0), NULL);
    assert_true(adjacency_list_adjacent(l, 3, 4), NULL);
    assert_true(adjacency_list_adjacent(l, 4, 3), NULL);
    assert_true(adjacency_list_adjacent(l, 3, 2), NULL);
    assert_true(adjacency_list_adjacent(l, 2, 3), NULL);
    assert_true(adjacency_list_adjacent(l, 1, 2), NULL);
    assert_true(adjacency_list_adjacent(l, 2, 1), NULL);
    assert_true(adjacency_list_adjacent(l, 1, 3), NULL);
    assert_true(adjacency_list_adjacent(l, 3, 1), NULL);
    assert_false(adjacency_list_adjacent(l, 0, 1), NULL);
    assert_false(adjacency_list_adjacent(l, 1, 0), NULL);
    assert_false(adjacency_list_adjacent(l, 2, 4), NULL);
    assert_false(adjacency_list_adjacent(l, 4, 2), NULL);

    adjacency_list_destroy(l);
}

static void test_remove_edge() {
    Adjacency_List *l = before_each();

    adjacency_list_remove_edge(l, 1, 3);

    assert_false(adjacency_list_adjacent(l, 1, 3), NULL);
    assert_false(adjacency_list_adjacent(l, 3, 1), NULL);

    adjacency_list_remove_edge(l, 3, 4);

    assert_true(
        singly_linked_list_is_empty(
            adjacency_list_neighbors(l, 4)
            ),
        NULL
        );

    adjacency_list_destroy(l);
}

static void test_get_set_edge_value() {
    Adjacency_List *l = before_each();

    assert_equal_int(
        adjacency_list_get_edge_value(l, 0, 3),
        adjacency_list_get_edge_value(l, 1, 2),
        NULL);

    adjacency_list_set_edge_value(l, 0, 3, -1);
    assert_equal_int(
        adjacency_list_get_edge_value(l, 1, 3),
        adjacency_list_get_edge_value(l, 0, 3),
        NULL);
    assert_nequal_int(
        adjacency_list_get_edge_value(l, 0, 3),
        adjacency_list_get_edge_value(l, 1, 2),
        NULL);

    adjacency_list_destroy(l);
}

int main() {
    test_adjacencies();
    test_get_set_edge_value();
    test_remove_edge();
}
