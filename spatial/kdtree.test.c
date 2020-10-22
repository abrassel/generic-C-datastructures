#include "../testing/test.h"
#include "../util/destroy_helpers.h"
#include "kdtree.h"

static void empty_assertions(KDTree *tree) {
    double point[] = {.5, .5};
    assert_true(kdtree_is_empty(tree), NULL);
    assert_equal_int(kdtree_size(tree), 0, NULL);
    assert_null(kdtree_search(tree, point), NULL);
}

static void test_empty() {
    KDTree *tree = kdtree_init(2, NULL, NULL);
    empty_assertions(tree);
    kdtree_destroy(tree);
}

static void test_insert1() {
    printf("test_insert1\n");
    KDTree *tree = kdtree_init(2, NULL, NULL);
    //test alternating dimensions in strictly increasing order
    double root[] = {5,5};
    double child[] = {10, 5};
    double gchild[] = {8, 8};

    kdtree_insert(tree, root, (char *) "hello");

    assert_equal("hello", kdtree_search(tree, root), &str_eq_ctx, NULL);
    assert_null(kdtree_search(tree, child), NULL);
    
    kdtree_insert(tree, child, (char *) "veronika");
    assert_equal("hello", kdtree_search(tree, root), &str_eq_ctx, NULL);
    assert_equal("veronika", kdtree_search(tree, child), &str_eq_ctx, NULL);
    assert_null(kdtree_search(tree, gchild), NULL);
    kdtree_insert(tree, gchild, (char *) "alex");
    assert_equal("hello", kdtree_search(tree, root), &str_eq_ctx, NULL);
    assert_equal("veronika", kdtree_search(tree, child), &str_eq_ctx, NULL);
    assert_equal("alex", kdtree_search(tree, gchild), &str_eq_ctx, NULL);
    
    kdtree_print(tree, print_string);
    kdtree_destroy(tree);
}

static void test_insert2() {
    printf("test_insert2\n");
    KDTree *tree = kdtree_init(2, NULL, NULL);

    double p1[] = {5,-5};
    double p2[] = {2,-3};
    double p3[] = {7, 1};
    double p4[] = {2, 5};
    double p5[] = {8, -10};

    double *points[] = {p1, p2, p3, p4, p5};
    const char * const names[] = {"alex", "griffin", "aubrey", "nola", "veronika"};

    for (size_t i = 0; i < 5; i++) {
        kdtree_insert(tree, points[i], (char *) names[i]);
        assert_equal(kdtree_search(tree, points[i]), names[i], &str_eq_ctx, NULL);
    }
    assert_equal_int(5, kdtree_size(tree), NULL);

    kdtree_print(tree, print_string);
    kdtree_destroy(tree);
}

static void test_replace() {
    printf("test replace\n");

    double p1[] = {5,-5};
    double p2[] = {2,-3};
    double p3[] = {7, 1};
    double p4[] = {2, 5};
    double p5[] = {8, -10};

    double dummy_point[] = {2.000001, -3.000001};

    KDTree *tree = kdtree_init(2, NULL, NULL);

    double *points[] = {p1, p2, p3, p4, p5};
    const char * const names[] = {"alex", "griffin", "aubrey", "nola", "veronika"};

    for (size_t i = 0; i < 5; i++) {
        kdtree_insert(tree, points[i], (char *) names[i]);
        assert_equal(kdtree_search(tree, points[i]), names[i], &str_eq_ctx, NULL);
    }
    assert_equal_int(5, kdtree_size(tree), NULL);

    kdtree_insert(tree, dummy_point, (char *) "new_guy");
    kdtree_print(tree, print_string);
    assert_equal_int(5, kdtree_size(tree), NULL);
    assert_equal("new_guy", kdtree_search(tree, p2), &str_eq_ctx, NULL);
    
    kdtree_destroy(tree);
}

static void test_remove_singleton() {
    KDTree *tree = kdtree_init(2, &naive_destroy_ctx, &naive_destroy_ctx);
    char *test = malloc(sizeof(char) * 6);
    strcpy(test, "hello");
    double *p0 = malloc(sizeof(double) * 2);
    p0[0] = 5;
    p0[1] = -5;

    
    kdtree_insert(tree, p0, test);
    assert_equal(kdtree_remove(tree, p0), "hello", &str_eq_ctx, NULL);
    empty_assertions(tree);
    free(test);
    kdtree_destroy(tree);
}

static void test_remove_leaf() {
    KDTree *tree = kdtree_init(2, NULL, NULL);

    const char *words[] = {
        "alex",
        "had",
        "a",
        "little",
        "lamb",
        "test"
    };
    double p0[][2] = {
        {5, -5},
        {2, -3},
        {7, 1},
        {2, 5},
        {8, -10},
        {2, -2}
    };

    for (size_t i = 0; i < 5; i++) {
        kdtree_insert(tree, p0[i], (char *) words[i]);
    }

    assert_equal(kdtree_remove(tree, p0[3]), words[3], &str_eq_ctx, NULL);
    kdtree_print(tree, print_string);
    assert_equal_int(kdtree_size(tree), 4, NULL);

    kdtree_insert(tree, p0[5], (char*) words[5]);
    assert_equal_int(kdtree_size(tree), 5, NULL);
    assert_equal(kdtree_search(tree, p0[5]), words[5], NULL, NULL);
    

    kdtree_destroy(tree);
}

static void test_remove_root() {
    printf("\n\nTEST_REMOVE_ROOT\n\n");
    KDTree *tree = kdtree_init(2, NULL, NULL);

    const char *words[] = {
        "alex",
        "had",
        "a",
        "little",
        "lamb",
        "test"
    };
    double p0[][2] = {
        {5, -5},
        {2, -3},
        {7, 1},
        {2, 5},
        {8, -10},
        {2, -2}
    };

    for (size_t i = 0; i < 5; i++) {
        kdtree_insert(tree, p0[i], (char *) words[i]);
    }

    assert_equal(kdtree_remove(tree, p0[0]), words[0], &str_eq_ctx, NULL);
    kdtree_print(tree, print_string);
    assert_equal_int(kdtree_size(tree), 4, NULL);
    

    kdtree_destroy(tree);
}

static void test_removal_two() {
    KDTree *tree1 = kdtree_init(2, NULL, NULL), *tree2 = kdtree_init(2, NULL, NULL);

    double p0[][2] = {
        {30, 40},
        {5, 25},
        {70, 70},
        {10, 12},
        {50, 30},
        {35, 45}
    };

    const char *words[] = {
        "griffin",
        "alex",
        "tom",
        "liam",
        "aki",
        "trevor"
    };

    for (int i = 0; i < 6; i++) {
        kdtree_insert(tree1, p0[i], (char *) words[i]);
        kdtree_insert(tree2, p0[i], (char *) words[i]);
    }

    assert_equal(kdtree_remove(tree1, p0[0]), words[0], &str_eq_ctx, NULL);
    assert_equal(kdtree_remove(tree2, p0[2]), words[2], &str_eq_ctx, NULL);

    assert_equal_int(kdtree_size(tree1), 5, NULL);
    assert_equal_int(kdtree_size(tree2), 5, NULL);

    assert_null(kdtree_search(tree1, p0[0]), NULL);
    assert_null(kdtree_search(tree2, p0[2]), NULL);

    printf("PRINTING TREE 1\n\n");

    kdtree_print(tree1, print_string);

    printf("PRINTING TREE 2\n\n");

    kdtree_print(tree2, print_string);
    
    kdtree_destroy(tree1);
    kdtree_destroy(tree2);
}

static int neighbor_comp(const void *k1, const void *k2, const void *ctx) {
    const comp_ctx *value_comp = (const comp_ctx *) ctx;
    const struct KD_Point *n1 = k1, *n2 = k2;

    return value_comp->comp(n1->value, n2->value, value_comp->ctx);
}

static void test_nn() {
    KDTree *tree = kdtree_init(2, NULL, NULL);

    double points[][2] = {
        {51, 75},
        {25, 40},
        {70, 70},
        {10, 30},
        {35, 90},
        {55, 1},
        {60, 80},
        {1, 10},
        {50, 50},
    };

    const char *words[] = {
        "a",
        "b",
        "c",
        "d",
        "e",
        "f",
        "g",
        "h",
        "i"
    };

    struct KD_Point results[9];

    for (size_t i = 0; i < 9; i++) {
        kdtree_insert(tree, points[i], (char *) words[i]);
        results[i].point = points[i];
        results[i].value = (char *) words[i];
    }

    comp_ctx neighbor_comp_ctx = {
        neighbor_comp,
        &str_comp_ctx,
    };
    eq_ctx neighbor_eq_ctx = {
        eq_from_comp,
        &neighbor_comp_ctx,
    };

    //should be able to find itself
    for (size_t i = 0; i < 9; i++) {
        assert_equal(kdtree_nn(tree, points[i]), results + i, &neighbor_eq_ctx, NULL);
    }

    double anchor[] = {51, 90};

    assert_equal(kdtree_nn(tree, anchor), results + 6, &neighbor_eq_ctx, NULL);

    kdtree_destroy(tree);
}

int main() {
    test_empty();
    test_insert1();
    test_insert2();
    test_replace();
    test_remove_singleton();
    test_remove_leaf();
    test_remove_root();
    test_removal_two();
    test_nn();
}
