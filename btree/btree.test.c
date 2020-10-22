#include "../testing/test.h"
#include "../util/destroy_helpers.h"
#include "btree.h"

static void empty_assertions(Btree *tree) {
    assert_true(btree_is_empty(tree), NULL);
    assert_equal_int(0, btree_size(tree), NULL);
    assert_null(btree_search(tree, "hello"), NULL);
}

static void test_empty_btree() {
    Btree *tree = btree_init(5, &str_comp_ctx, NULL, NULL);

    empty_assertions(tree);
    
    btree_destroy(tree);
}

static void test_only_root_leaf_btree() {
    Btree *tree = btree_init(4, &str_comp_ctx, NULL, NULL);

    btree_insert(tree, (char *) "alex", (char *) "hello");
    assert_equal("hello", btree_search(tree, "alex"), &str_eq_ctx, NULL);
    assert_null(btree_search(tree, "veronika"), NULL);
    assert_null(btree_search(tree, "baby"), NULL);
    btree_insert(tree, (char *) "veronika", (char *) "world");
    assert_equal("hello", btree_search(tree, "alex"), &str_eq_ctx, NULL);
    assert_equal("world", btree_search(tree, "veronika"), &str_eq_ctx, NULL);
    assert_null(btree_search(tree, "baby"), NULL);
    btree_insert(tree, (char *) "baby", (char *) "goodbye");
    assert_equal("hello", btree_search(tree, "alex"), &str_eq_ctx, NULL);
    assert_equal("world", btree_search(tree, "veronika"), &str_eq_ctx, NULL);
    assert_equal("goodbye", btree_search(tree, "baby"), &str_eq_ctx, NULL);

    assert_false(btree_is_empty(tree), NULL);
    assert_equal_int(3, btree_size(tree), NULL);

    btree_destroy(tree);
}

static void test_resizes() {
    int **numbers = malloc(sizeof(int *) * 7);
    Btree *tree = btree_init(3, &int_comp_ctx, &naive_destroy_ctx, NULL);
    
    for (int i = 0; i < 7; i++) {
        numbers[i] = malloc(sizeof(int));
        *numbers[i] = i + 1;
        btree_insert(tree, numbers[i], numbers[i]);
        assert_equal_int(i + 1, btree_size(tree), NULL);
        
        for(int j = 0; j <= i; j++) {
            assert_equal(numbers[i], btree_search(tree, numbers[j]), NULL, NULL);
        }
        for (int j = i + 1; i < 7; i++) {
            int search = j + 1;
            assert_null(btree_search(tree, &search), NULL);
        }
    }
    free(numbers);
    btree_destroy(tree);
}

int main() {
    test_empty_btree();
    test_only_root_leaf_btree();
    test_resizes();
}
