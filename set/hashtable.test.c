#include "../testing/test.h"
#include "hashtable.h"
#include "../util/comparisons.h"

static int vals[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

static void empty_assertions(Hashtable *h) {
    assert_true(hashtable_is_empty(h), NULL);
    assert_false(hashtable_remove(h, vals + 0), NULL);
    assert_false(hashtable_contains(h, vals + 0), NULL);
    assert_null(hashtable_get(h, vals + 0), NULL);
    hashtable_clear(h);

    Hashtable_Iter *iter = hashtable_iter_init(h);
    assert_false(hashtable_iter_has_next(iter), NULL);
    hashtable_iter_destroy(iter);
}
static void test_empty() {
    Hashtable *h = hashtable_init(&identity_hash_ctx, linear_probing, &int_eq_ctx, NULL, (double) 2 / 3);

    empty_assertions(h);
    
    hashtable_destroy(h);
}
static void test_insert_no_collision() {
    Hashtable *h = hashtable_init(&identity_hash_ctx, linear_probing, &int_eq_ctx, NULL, (double) 2 / 3);
    int multed[15];
    for (int i = 0; i < 15; i++) {
        multed[i] = 13 * vals[i];
    }

    for (int i = 0; i < 8; i++) {
        assert_true(hashtable_add(h, multed + i), NULL);
        for (int j=0; j <= i; j++) {
            assert_true(hashtable_contains(h, multed + j), NULL);
            assert_equal(hashtable_get(h, multed + j), multed + j, &int_eq_ctx, NULL);
        }
        assert_equal_int(i + 1, hashtable_size(h), NULL);
        for (int j = i+1; j < 8; j++) {
            assert_false(hashtable_contains(h, multed + j), NULL);
        }
    }

    Hashtable_Iter *iter = hashtable_iter_init(h);

    for (int i = 0; i < 8; i++) {
        assert_true(hashtable_iter_has_next(iter), NULL);
        assert_equal(hashtable_iter_next(iter), multed + i, &int_eq_ctx, NULL);
    }
    assert_false(hashtable_iter_has_next(iter), NULL);
    
    hashtable_iter_destroy(iter);
    
    hashtable_destroy(h);  
}
static void test_insert_collision() {
    Hashtable *h = hashtable_init(&identity_hash_ctx, linear_probing, &int_eq_ctx, NULL, (double) 2 / 3);

    int vals[] = {5, 5 + 13, 5 + 2 * 13};

    assert_true(hashtable_add(h, vals), NULL);
    assert_true(hashtable_add(h, vals + 1), NULL);
    assert_true(hashtable_add(h, vals + 2), NULL);

    assert_equal_int(3, hashtable_size(h), NULL);
    assert_equal(hashtable_get(h, vals), vals, &int_eq_ctx, NULL);
    assert_equal(hashtable_get(h, vals + 1), vals + 1, &int_eq_ctx, NULL);
    assert_equal(hashtable_get(h, vals + 2), vals + 2, &int_eq_ctx, NULL);

    Hashtable_Iter *iter = hashtable_iter_init(h);

    for (int i = 0; i < 3; i++) {
        assert_true(hashtable_iter_has_next(iter), NULL);
        assert_equal(hashtable_iter_next(iter), vals + i, &int_eq_ctx, NULL);
    }
    assert_false(hashtable_iter_has_next(iter), NULL);
   
    
    hashtable_iter_destroy(iter);
    
    hashtable_destroy(h);  
}
static void test_resize() {
    Hashtable *h = hashtable_init(&identity_hash_ctx, linear_probing, &int_eq_ctx, NULL, (double) 2 / 3);

    for (int i = 0; i < 15; i++) {
        assert_true(hashtable_add(h, vals + i), NULL);
        for (int j=0; j <= i; j++) {
            assert_true(hashtable_contains(h, vals + j), NULL);
            assert_equal(hashtable_get(h, vals + j), vals + j, &int_eq_ctx, NULL);
        }
        assert_equal_int(i + 1, hashtable_size(h), NULL);
        for (int j = i+1; j < 15; j++) {
            assert_false(hashtable_contains(h, vals + j), NULL);
        }
    }

    Hashtable_Iter *iter = hashtable_iter_init(h);

    for (int i = 0; i < 15; i++) {
        assert_true(hashtable_iter_has_next(iter), NULL);
        assert_equal(hashtable_iter_next(iter), vals + i, &int_eq_ctx, NULL);
    }
    assert_false(hashtable_iter_has_next(iter), NULL);
    
    hashtable_iter_destroy(iter);

    hashtable_destroy(h);  
}
static void test_remove() {
    Hashtable *h = hashtable_init(&identity_hash_ctx, linear_probing, &int_eq_ctx, NULL, (double) 2 / 3);

    for (int i = 0; i < 15; i++) {
        assert_true(hashtable_add(h, vals + i), NULL);
        for (int j=0; j <= i; j++) {
            assert_true(hashtable_contains(h, vals + j), NULL);
            assert_equal(hashtable_get(h, vals + j), vals + j, &int_eq_ctx, NULL);
        }
        assert_equal_int(i + 1, hashtable_size(h), NULL);
        for (int j = i+1; j < 15; j++) {
            assert_false(hashtable_contains(h, vals + j), NULL);
        }
    }

    for (int k = 14; k >= 0; k--) {
        assert_true(hashtable_remove(h, vals + k), NULL);
        for (int j=0; j < k; j++) {
            assert_true(hashtable_contains(h, vals + j), NULL);
            assert_equal(hashtable_get(h, vals + j), vals + j, &int_eq_ctx, NULL);
        }
        assert_equal_int(k, hashtable_size(h), NULL);
        for (int j = k; j < 15; j++) {
            assert_false(hashtable_contains(h, vals + j), NULL);
        }
        
        Hashtable_Iter *iter = hashtable_iter_init(h);

        for (int i = 0; i < k; i++) {
            assert_true(hashtable_iter_has_next(iter), NULL);
            assert_equal(hashtable_iter_next(iter), vals + i, &int_eq_ctx, NULL);
        }
        assert_false(hashtable_iter_has_next(iter), NULL);
    
        hashtable_iter_destroy(iter);  
    }

    empty_assertions(h);

    hashtable_destroy(h);  
}
static void test_remove_with_collision() {
    Hashtable *h = hashtable_init(&identity_hash_ctx, linear_probing, &int_eq_ctx, NULL, (double) 2 / 3);

    int vals[] = {5, 5 + 13, 5 + 2 * 13};

    assert_true(hashtable_add(h, vals), NULL);
    assert_true(hashtable_add(h, vals + 1), NULL);
    assert_true(hashtable_add(h, vals + 2), NULL);

    assert_true(hashtable_remove(h, vals + 1), NULL);
    assert_false(hashtable_remove(h, vals + 1), NULL);

    assert_true(hashtable_contains(h, vals + 2), NULL);
    assert_false(hashtable_contains(h, vals + 1), NULL);

    assert_equal_int(2, hashtable_size(h), NULL);
    
    hashtable_destroy(h);  
}

static void test_addall() {
    Hashtable *h = hashtable_init(&identity_hash_ctx, linear_probing, &int_eq_ctx, NULL, (double) 2 / 3);

    hashtable_destroy(h);  
}
static void test_containsall() {
    Hashtable *h = hashtable_init(&identity_hash_ctx, linear_probing, &int_eq_ctx, NULL, (double) 2 / 3);

    hashtable_destroy(h);  
}
static void test_removall() {
    Hashtable *h = hashtable_init(&identity_hash_ctx, linear_probing, &int_eq_ctx, NULL, (double) 2 / 3);

    hashtable_destroy(h);  
}
static void test_retainall() {
    Hashtable *h = hashtable_init(&identity_hash_ctx, linear_probing, &int_eq_ctx, NULL, (double) 2 / 3);

    hashtable_destroy(h);  
}

int main() {
    test_empty();
    test_insert_no_collision();
    test_insert_collision();
    test_resize();
    test_remove();
    test_remove_with_collision();
    test_addall();
    test_containsall();
    test_removall();
    test_retainall();
}
