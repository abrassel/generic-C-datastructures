#include "../testing/test.h"
#include "hashmap.h"

#include "../util/destroy.h"
#include "../util/comparisons.h"
#include "../util/destroy_helpers.h"

#include <stdlib.h>

static int vals[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

static void empty_assertions(Hashmap *h) {
    assert_true(hashmap_is_empty(h), NULL);
    assert_false(hashmap_remove(h, vals + 0), NULL);
    assert_false(hashmap_contains(h, vals + 0), NULL);
    assert_null(hashmap_get(h, vals + 0), NULL);
    hashmap_clear(h);

    Hashmap_Iter *iter = hashmap_iter_init(h);
    assert_false(hashmap_iter_has_next(iter), NULL);
    hashmap_iter_destroy(iter);
}
static void test_empty() {
    Hashmap *h = hashmap_init(&identity_hash_ctx, &int_eq_ctx, NULL, &naive_destroy_ctx);

    empty_assertions(h);
    
    hashmap_destroy(h);
}

static void test_add_a_few() {
    //create a few keys and values
    int *key1 = malloc(sizeof(*key1));
    int *key2 = malloc(sizeof(*key2));
    int *key3 = malloc(sizeof(*key3));

    char *val1 = malloc(sizeof(*val1) * 2);
    char *val2 = malloc(sizeof(*val2) * 2);
    char *val3 = malloc(sizeof(*val3) * 2);

    *key1 = 5;
    *key2 = 6;
    *key3 = 7;

    strcpy(val1, "a");
    strcpy(val2, "b");
    strcpy(val3, "c");
    
    Hashmap *h = hashmap_init(&identity_hash_ctx, &int_eq_ctx, &naive_destroy_ctx, &naive_destroy_ctx);

    hashmap_add(h, key1, val1);
    hashmap_add(h, key2, val2);
    hashmap_add(h, key3, val3);

    void *test = hashmap_get(h, key2);

    assert_equal(hashmap_get(h, key1), val1, &str_eq_ctx, NULL);
    assert_equal(test, val2, &str_eq_ctx, NULL);
    assert_equal(hashmap_get(h, key3), val3, &str_eq_ctx, NULL);

    assert_true(hashmap_remove(h, key2), NULL);
    free(test);
    
    hashmap_destroy(h);
}



int main() {
    test_empty();
    test_add_a_few();
}
