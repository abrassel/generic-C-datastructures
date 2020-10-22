#include "../testing/test.h"
#include "arr_heap.h"
#include "../util/comparisons.h"
#include "../util/kvpair.h"
#include "../util/destroy_helpers.h"

static void empty_assertions(Array_Heap *heap) {
    assert_true(array_heap_is_empty(heap), NULL);
    assert_null(array_heap_pull(heap), NULL);
    assert_null(array_heap_peek(heap), NULL);
    assert_equal_int(array_heap_size(heap), 0, NULL);
}

static void test_empty() {
    Array_Heap *heap = array_heap_init(0, &str_comp_ctx, &naive_destroy_ctx, &naive_destroy_ctx);
    empty_assertions(heap);
    array_heap_destroy(heap);
}

static void test_singleton() {
    Array_Heap *heap = array_heap_init(1, &str_comp_ctx, NULL, NULL);

    array_heap_insert(heap, (char *) "abc", (char *) "veronika");
    assert_false(array_heap_is_empty(heap), NULL);
    assert_equal_int(array_heap_size(heap), 1, NULL);

    assert_equal(array_heap_peek_v(heap), (char *) "veronika", &str_eq_ctx, NULL);
    KVPair *result = array_heap_pull(heap);
    assert_equal(result->value, (char *) "veronika", &str_eq_ctx, NULL);
    free(result);

    empty_assertions(heap);
    
    array_heap_destroy(heap);
}

static void test_insert() {
    Array_Heap *heap = array_heap_init(10, &str_comp_ctx, NULL, NULL);
    array_heap_insert(heap, (char *)"e", (char *) "e");
    array_heap_insert(heap, (char *)"d", (char *)"d");
    array_heap_insert(heap, (char *) "c", (char *) "c");
    array_heap_insert(heap, (char *) "b", (char *) "b");
    array_heap_insert(heap, (char *) "a", (char *) "a");

    assert_equal_int(array_heap_size(heap), 5, NULL);

    assert_equal(array_heap_pull_v(heap), (char *) "e", &str_eq_ctx, NULL);
    assert_equal(array_heap_pull_v(heap), (char *) "d", &str_eq_ctx, NULL);
    assert_equal(array_heap_pull_v(heap), (char *) "c", &str_eq_ctx, NULL);
    assert_equal(array_heap_pull_v(heap), (char *) "b", &str_eq_ctx, NULL);
    assert_equal(array_heap_pull_v(heap), (char *) "a", &str_eq_ctx, NULL);

    empty_assertions(heap);

    array_heap_insert(heap, (char *) "a", (char *) "a");
    array_heap_insert(heap, (char *) "b", (char *) "b");
    array_heap_insert(heap, (char *) "c", (char *) "c");
    array_heap_insert(heap, (char *) "d", (char *) "d");
    array_heap_insert(heap, (char *) "f", (char *) "f");

    assert_equal(array_heap_pull_v(heap), (char *) "f", &str_eq_ctx, NULL);
    assert_equal_int(array_heap_size(heap), 4, NULL);
    array_heap_insert(heap, (char* )"e", (char *)"e");
    assert_equal_int(array_heap_size(heap), 5, NULL);

    assert_equal(array_heap_pull_v(heap), (char *) "e", &str_eq_ctx, NULL);
    assert_equal(array_heap_pull_v(heap), (char *) "d", &str_eq_ctx, NULL);
    assert_equal(array_heap_pull_v(heap), (char *) "c", &str_eq_ctx, NULL);
    assert_equal(array_heap_pull_v(heap), (char *) "b", &str_eq_ctx, NULL);
    assert_equal(array_heap_pull_v(heap), (char *) "a", &str_eq_ctx, NULL);

    empty_assertions(heap);
    
    array_heap_destroy(heap);
}

static void test_insert2() {
    comp_ctx inv_raw_double_comp_ctx = {
        inv_comp,
        &raw_double_comp_ctx
    };
    Array_Heap *h = array_heap_init(100, &inv_raw_double_comp_ctx, NULL, NULL);

    array_heap_insert(h, (void *) (uintptr_t) 0, (void *) (uintptr_t) 0);
    assert_equal_int((uintptr_t) array_heap_pull_v(h), 0, NULL);
    array_heap_insert(h, (void *) (uintptr_t) 4, (void *) (uintptr_t) 1);
    array_heap_insert(h, (void *) (uintptr_t) 6, (void *) (uintptr_t) 7);
    assert_equal_int((uintptr_t) array_heap_pull_v(h), 1, NULL);
    array_heap_insert(h, (void *) (uintptr_t) 14, (void *) (uintptr_t) 2);
    array_heap_insert(h, (void *) (uintptr_t) 15, (void *) (uintptr_t) 6);
    assert_equal_int((uintptr_t) array_heap_pull_v(h), 7, NULL);
    array_heap_insert(h, (void *) (uintptr_t) 7, (void *) (uintptr_t) 1);
    array_heap_insert(h, (void *) (uintptr_t) 13, (void *) (uintptr_t) 8);
    assert_equal_int((uintptr_t) array_heap_pull_v(h), 1, NULL);

    array_heap_destroy(h);
}

static void test_bounded_min_queue() {
    Bounded_Min_Queue *min_queue = bounded_min_queue_init(5, &int_comp_ctx, NULL, NULL);
    int values[] = {5, 3, 10, 2, 8, 13, 4};
    const char *words[] = {"alex", "veronika", "shon", "nola", "griffin", "aubrey", "olivia"};
    // implementation details yield opposite order, which can be leveraged for testing

    // at first try a partial fill
    for (int i = 0 ; i < 4; i++) {
        bounded_min_queue_insert(min_queue, values + i, (char *) words[i]);
    }
    //no assertions to test because wrapper around heap
    assert_equal_int(bounded_min_queue_size(min_queue), 4, NULL);

    //try inserting the rest
    for (int i = 4; i < 7; i++) {
        bounded_min_queue_insert(min_queue, values + i, (char *) words[i]);
        assert_equal_int(bounded_min_queue_size(min_queue), 5, NULL);
    }

    int smallest_keys[5] = {
        3, 1, 6, 0, 4
    };

    for (int i = 4; i >= 0; i--) {
        KVPair *res = bounded_min_queue_pop_unordered(min_queue);
        assert_equal(words[smallest_keys[i]], res->value, &str_eq_ctx, NULL);
        free(res);
    }
    
    bounded_min_queue_destroy(min_queue);
}

static void test_bounded_min_queue_sorted() {
    Bounded_Min_Queue *min_queue = bounded_min_queue_init(5, &str_comp_ctx, NULL, NULL);

    // this time, a sorted array is created which is predictable
    int values[] = {5, 3, 10, 2, 8, 13, 4};
    const char *words[] = {"alex", "veronika", "shon", "nola", "griffin", "aubrey", "olivia"};
    // implementation details yield opposite order, which can be leveraged for testing

    // at first try a partial fill
    for (int i = 0 ; i < 4; i++) {
        bounded_min_queue_insert(min_queue, values + i, (char *) words[i]);
    }
    //no assertions to test because wrapper around heap
    assert_equal_int(bounded_min_queue_size(min_queue), 4, NULL);

    //try inserting the rest
    for (int i = 4; i < 7; i++) {
        bounded_min_queue_insert(min_queue, values + i, (char *) words[i]);
        assert_equal_int(bounded_min_queue_size(min_queue), 5, NULL);
    }

    int smallest_keys[5] = {
        3, 1, 6, 0, 4
    };

    KVPair **all_points = bounded_min_queue_getall_ordered(min_queue);

    for (int i = 0; i < 5; i++) {
        assert_equal(words[smallest_keys[i]], all_points[i]->value, &str_eq_ctx, NULL);
        free(all_points[i]);
    }

    free(all_points);
}

int main() {
    test_empty();
    test_singleton();
    test_insert();
    test_insert2();
    test_bounded_min_queue();
    test_bounded_min_queue_sorted();
}
