#include "../testing/test.h"
#include "arraylist.h"
#include "../util/comparisons.h"

static void test_push() {
    Array_List *a = array_list_init(NULL, &str_eq_ctx);

    assert_true(array_list_is_empty(a), NULL);
    assert_equal_int(0, array_list_size(a), NULL);

    const char *strings[] = {
        "hello",
        "world",
        "this",
        "is",
        "a",
        "good",
        "day",
        "to",
        "be",
        "alive",
        "now"
    };

    for (size_t i = 0; i < 11; i++) {
        array_list_push(a, (char *) strings[i]);
        for (size_t j = 0; j < i; j++) {
            assert_true(array_list_contains(a, strings[j]), NULL);
            assert_equal(array_list_get(a, j), strings[j], &str_eq_ctx, NULL);
        }
        assert_equal_int(array_list_size(a), i + 1, NULL);
    }

    for (int i = 10; i >= 0; i--) {
        array_list_set(a, 10-i, (char *) strings[i]);
    }
    for (int i = 0; i < 11; i++) {
        assert_equal(array_list_get(a, i), strings[10-i], &str_eq_ctx, NULL);
    }
    
    array_list_destroy(a);
}

static void test_replace() {
    Array_List *a = array_list_init(NULL, &str_eq_ctx);
    const char *strings[] = {
        "hello",
        "world",
        "this",
        "is",
        "a",
        "good",
        "day",
        "to",
        "be",
        "alive",
        "now"
    };

    for (size_t i = 0; i < 11; i++) {
        array_list_push(a, (char *) strings[i]);
    }

    array_list_set(a, 3, (char *) strings[4]);
    assert_equal(strings[4], array_list_get(a, 3), &str_eq_ctx, NULL);
    array_list_remove(a, 3);
    assert_equal_int(10, array_list_size(a), NULL);
    for (size_t i = 3; i < 10; i++) {
        assert_equal(strings[i + 1], array_list_get(a, i), &str_eq_ctx, NULL);
    }

    array_list_remove(a, 9);
    assert_equal_int(9, array_list_size(a), NULL);
    
    array_list_destroy(a);
}

int main() {
    test_push();
    test_replace();
}
