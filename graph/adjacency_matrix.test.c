#include "../testing/test.h"
#include "adjacency_matrix.h"

static void test_adding_edges_undirected() {
    Adjacency_Matrix *m = adjacency_matrix_init(5, false);

    for (size_t i = 0; i < 6; i++) {
        for (size_t j = 0; j < 6; j++) {
            assert_false(adjacency_matrix_adjacent(m, i, j), NULL);
        }
    }

    assert_true(adjacency_matrix_add_edge(m, 0, 2, 5.3), NULL);
    assert_true(adjacency_matrix_add_edge(m, 2, 4, 2), NULL);
    assert_false(adjacency_matrix_add_edge(m, 6, 2, 5), NULL);
    assert_true(adjacency_matrix_add_edge(m, 1, 0, 2), NULL);

    assert_true(adjacency_matrix_adjacent(m, 2, 0), NULL);
    assert_true(adjacency_matrix_adjacent(m, 2, 4), NULL);
    assert_false(adjacency_matrix_adjacent(m, 2, 6), NULL);

    double *neighbors = adjacency_matrix_neighbors(m, 2);
    double correct[] = {5.3, 0, 0, 0, 2, 0};


    for (size_t i = 0; i < 5; i++) {
        assert_equal_int(correct[i], neighbors[i], NULL);
    }

    assert_true(adjacency_matrix_remove_edge(m, 2, 4), NULL);
    assert_false(adjacency_matrix_adjacent(m, 2, 4), NULL);

    adjacency_matrix_destroy(m);
}

static void test_adding_and_removing_nodes() {
    Adjacency_Matrix *m = adjacency_matrix_init(5, false);

    //get a small graph going.

    assert_true(adjacency_matrix_add_edge(m, 0, 2, 5.3), NULL);
    assert_true(adjacency_matrix_add_edge(m, 2, 4, 2), NULL);
    assert_false(adjacency_matrix_add_edge(m, 6, 2, 5), NULL);
    assert_true(adjacency_matrix_add_edge(m, 1, 0, 2), NULL);

    //add at the edge
    adjacency_matrix_add_vertex(m, 5);
    assert_true(adjacency_matrix_add_edge(m, 0, 5, 3), NULL);

    //check old invariants
    assert_true(adjacency_matrix_adjacent(m, 2, 0), NULL);
    assert_true(adjacency_matrix_adjacent(m, 2, 4), NULL);
    assert_false(adjacency_matrix_adjacent(m, 2, 6), NULL);

    double *neighbors = adjacency_matrix_neighbors(m, 2);
    double correct[] = {5.3, 0, 0, 0, 2, 0, 0};


    for (size_t i = 0; i < 6; i++) {
        assert_equal_int(correct[i], neighbors[i], NULL);
    }

    //now try inserting in the middle
    adjacency_matrix_add_vertex(m, 3);
    
    //shifts 3+ over 1
    //check old invariants
    assert_true(adjacency_matrix_adjacent(m, 2, 0), NULL);
    assert_true(adjacency_matrix_adjacent(m, 2, 5), NULL);

    neighbors = adjacency_matrix_neighbors(m, 2);
    double correct2[] = {5.3, 0, 0, 0, 0, 2, 0};


    for (size_t i = 0; i < 6; i++) {
        assert_equal_int(correct2[i], neighbors[i], NULL);
    }

    //now remove 3 and should go back to old
    adjacency_matrix_remove_vertex(m, 3);
    assert_true(adjacency_matrix_adjacent(m, 2, 0), NULL);
    assert_true(adjacency_matrix_adjacent(m, 2, 4), NULL);
    assert_false(adjacency_matrix_adjacent(m, 2, 6), NULL);

    neighbors = adjacency_matrix_neighbors(m, 2);


    for (size_t i = 0; i < 6; i++) {
        assert_equal_int(correct[i], neighbors[i], NULL);
    }
    
    adjacency_matrix_destroy(m);
}


int main() {
    test_adding_edges_undirected();
    test_adding_and_removing_nodes();
}
