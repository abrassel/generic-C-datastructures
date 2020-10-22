#include "../testing/test.h"
#include "tree_heap.h"
#include "../util/comparisons.h"
#include "../util/destroy_helpers.h"

static void empty_assertions(Tree_Heap *heap) {
    assert_true(tree_heap_is_empty(heap), NULL);
    assert_null(tree_heap_pull(heap), NULL);
    assert_null(tree_heap_peek(heap), NULL);
    assert_equal_int(tree_heap_size(heap), 0, NULL);
}

static void test_empty() {
    Tree_Heap *heap = tree_heap_init(&str_comp_ctx, &naive_destroy_ctx, &naive_destroy_ctx);
    empty_assertions(heap);
    tree_heap_destroy(heap);
}

static void test_singleton() {
    Tree_Heap *heap = tree_heap_init(&str_comp_ctx, NULL, NULL);

    tree_heap_insert(heap, (char *)  "abc", (char *) "veronika");
    assert_false(tree_heap_is_empty(heap), NULL);
    assert_equal_int(tree_heap_size(heap), 1, NULL);

    assert_equal(tree_heap_peek(heap), (char *) "veronika", &str_eq_ctx, NULL);
    char *singleton = tree_heap_pull(heap);
    assert_equal(singleton, (char *) "veronika", &str_eq_ctx, NULL);

    empty_assertions(heap);
    
    tree_heap_destroy(heap);
}

static void test_insert() {
    Tree_Heap *heap = tree_heap_init(&str_comp_ctx, NULL, NULL);
    tree_heap_insert(heap, (char *) "e", (char *) "e");
    tree_heap_insert(heap, (char *) "d", (char *) "d");
    tree_heap_insert(heap, (char *) "c", (char *) "c");
    tree_heap_insert(heap, (char *) "b", (char *) "b");
    tree_heap_insert(heap, (char *) "a", (char *) "a");

    assert_equal_int(tree_heap_size(heap), 5, NULL);

    assert_equal(tree_heap_pull(heap), (char *) "e", &str_eq_ctx, NULL);
    assert_equal(tree_heap_pull(heap), (char *) "d", &str_eq_ctx, NULL);
    assert_equal(tree_heap_pull(heap), (char *) "c", &str_eq_ctx, NULL);
    assert_equal(tree_heap_pull(heap), (char *) "b", &str_eq_ctx, NULL);
    assert_equal(tree_heap_pull(heap), (char *) "a", &str_eq_ctx, NULL);

    empty_assertions(heap);

    tree_heap_insert(heap, (char *) "a", (char *) "a");
    tree_heap_insert(heap, (char *) "b", (char *) "b");
    tree_heap_insert(heap, (char *) "c", (char *) "c");
    tree_heap_insert(heap, (char *) "d", (char *) "d");
    tree_heap_insert(heap, (char *) "f", (char *) "f");

    assert_equal(tree_heap_pull(heap), (char *) "f", &str_eq_ctx, NULL);
    assert_equal_int(tree_heap_size(heap), 4, NULL);
    tree_heap_insert(heap, (char *) "e", (char *) "e");
    assert_equal_int(tree_heap_size(heap), 5, NULL);

    assert_equal(tree_heap_pull(heap), (char *) "e", &str_eq_ctx, NULL);
    assert_equal(tree_heap_pull(heap), (char *) "d", &str_eq_ctx, NULL);
    assert_equal(tree_heap_pull(heap), (char *) "c", &str_eq_ctx, NULL);
    assert_equal(tree_heap_pull(heap), (char *) "b", &str_eq_ctx, NULL);
    assert_equal(tree_heap_pull(heap), (char *) "a", &str_eq_ctx, NULL);

    empty_assertions(heap);
    
    tree_heap_destroy(heap);
}

int main() {
    test_empty();
    test_singleton();
    test_insert();
}
