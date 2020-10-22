#include "../testing/test.h"
#include "point.h"
#include "pr_quadtree.h"

static Point2D origin = {0, 0};
static Point2D test_point2 = {5, 5};
static Point2D test_point3 = {-5, 5};
static Point2D test_point4 = {-5, -5};
static Point2D test_point5 = {5, -5};
static Rectangle plane;


static void test_empty_quadtree() {
    PRQuadtree *q = prquadtree_init(plane);

    assert_true(prquadtree_is_empty(q), NULL);
    assert_equal_int(0, prquadtree_size(q), NULL);
    assert_false(prquadtree_search(q, origin), NULL);
    
    prquadtree_destroy(q);
}

static void test_singleton_quadtree() {
    PRQuadtree *q = prquadtree_init(plane);

    //insert up to three and check invariants at each step
    assert_equal_int(-1, prquadtree_height(q), NULL);
    prquadtree_add(q, origin);
    assert_equal_int(0, prquadtree_height(q), NULL);
    assert_equal_int(1, prquadtree_size(q), NULL);
    prquadtree_add(q, test_point2);
    assert_equal_int(0, prquadtree_height(q), NULL);
    assert_equal_int(2, prquadtree_size(q), NULL);
    prquadtree_add(q, test_point3);
    assert_equal_int(0, prquadtree_height(q), NULL);
    assert_equal_int(3, prquadtree_size(q), NULL);
    prquadtree_add(q, test_point4);
    assert_equal_int(0, prquadtree_height(q), NULL);
    assert_equal_int(4, prquadtree_size(q), NULL);

    assert_true(prquadtree_search(q, origin), NULL);
    assert_true(prquadtree_search(q, test_point2), NULL);
    assert_true(prquadtree_search(q, test_point3), NULL);
    assert_true(prquadtree_search(q, test_point4), NULL);
    assert_false(prquadtree_search(q, test_point5), NULL);
    
    
    
    prquadtree_destroy(q);
}

static void test_single_split_quadtree() {
    PRQuadtree *q = prquadtree_init(plane);

    prquadtree_add(q, test_point2);
    prquadtree_add(q, test_point3);
    prquadtree_add(q, test_point4);
    prquadtree_add(q, test_point5);
    prquadtree_add(q, origin);

    assert_true(prquadtree_search(q, origin), NULL);
    assert_true(prquadtree_search(q, test_point2), NULL);
    assert_true(prquadtree_search(q, test_point3), NULL);
    assert_true(prquadtree_search(q, test_point4), NULL);
    assert_true(prquadtree_search(q, test_point5), NULL);

    assert_equal_int(1, prquadtree_height(q), NULL);
    assert_equal_int(5, prquadtree_size(q), NULL);

    prquadtree_destroy(q);
}

static void test_repeated_split() {
    Rectangle smaller = {
        {-10, -10},
        {10, 10}
    };
    PRQuadtree *q = prquadtree_init(smaller);

    Point2D q1_points[5] = {
        {1, 1},
        {2, 2},
        {3, 3},
        {4, 4},
        {6, 6},
    };
    
    for (size_t i = 0; i < 5; i++) {
        prquadtree_add(q, q1_points[i]);
    }

    for (size_t i = 0; i < 5; i++) {
        assert_true(prquadtree_search(q, q1_points[i]), NULL);
    }

    assert_equal_int(2, prquadtree_height(q), NULL);

    assert_equal_int(prquadtree_size(q), 5, NULL);
    
    prquadtree_destroy(q);
}

static void test_removal() {
    Rectangle smaller = {
        {-10, -10},
        {10, 10}
    };
    PRQuadtree *q = prquadtree_init(smaller);

    Point2D q1_points[6] = {
        {1, 1},
        {2, 2},
        {3, 3},
        {4, 4},
        {6, 6},
        {2, 3},
    };
    
    for (size_t i = 0; i < 6; i++) {
        prquadtree_add(q, q1_points[i]);
    }
    assert_equal_int(prquadtree_height(q), 3, NULL);

    prquadtree_remove(q, q1_points[5]);
    assert_equal_int(prquadtree_size(q), 5, NULL);
    assert_equal_int(prquadtree_height(q), 3, NULL);
    assert_false(prquadtree_search(q, q1_points[5]), NULL);

    for (int i = 0; i < 5; i++) {
        assert_true(prquadtree_search(q, q1_points[i]), NULL);
        prquadtree_remove(q, q1_points[i]);
        assert_equal_int(prquadtree_size(q), 4-i, NULL);

        if (i == 3) {
            assert_equal_int(prquadtree_height(q), 2, NULL);
        }
    }

    prquadtree_destroy(q);
}

int main() {
    infinite_rectangle(plane);
    test_empty_quadtree();
    test_singleton_quadtree();
    test_single_split_quadtree();
    test_repeated_split();
    test_removal();
}
