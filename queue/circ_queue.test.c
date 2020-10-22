#include "../testing/test.h"
#include "circ_queue.h"

static void test_empty() {
    Circ_Queue *q = circ_queue_init(10, NULL);
    assert_true(circ_queue_is_empty(q), "empty");
    assert_false(circ_queue_is_full(q), "full");
    assert_null(circ_queue_peek(q), "pop");
    assert_equal_int(circ_queue_size(q), 0, "size");
    circ_queue_destroy(q);
}

static void test_insertion_to_full() {
    int three = 3;
    int five = 5;
    int seven = 7;
    Circ_Queue *q = circ_queue_init(3, NULL);

    assert_true(circ_queue_insert(q, &three), "insertion success");
    assert_equal(&three, circ_queue_peek(q), NULL, "peek");
    assert_true(circ_queue_insert(q, &five), "insert 1");
    assert_true(circ_queue_insert(q, &seven), "insert 2");
    assert_false(circ_queue_insert(q, &three), "insert 3");
    assert_true(circ_queue_is_full(q), "full");
    
    circ_queue_destroy(q);
}

static void test_insertion_wraparound() {
    int three = 3;
    int five = 5;
    int seven = 7;
    Circ_Queue *q = circ_queue_init(3, NULL);

    assert_true(circ_queue_insert(q, &three), "insertion success");
    assert_equal(&three, circ_queue_peek(q), NULL, "peek");
    assert_true(circ_queue_insert(q, &five), "insert 1");
    assert_equal(&three, circ_queue_pop(q), NULL, "pop three");
    assert_equal(&five, circ_queue_pop(q), NULL, "pop five");
    assert_null(circ_queue_pop(q), "null");
    assert_true(circ_queue_insert(q, &seven), "insert 2");
    assert_true(circ_queue_insert(q, &three), "insert 3");
    assert_equal(&seven, circ_queue_pop(q), NULL, "remove");
    assert_false(circ_queue_is_full(q), "full");
    assert_equal(&three, circ_queue_pop(q), NULL, "remove");
    assert_true(circ_queue_is_empty(q), NULL);
    circ_queue_destroy(q);
}

static void test_strings() {
    char *veronika = (char *) "Veronika";
    char *alex = (char *) "Alex";

    Circ_Queue *q = circ_queue_init(2, NULL);
    circ_queue_insert(q, veronika);
    circ_queue_insert(q, alex);
    assert_equal(veronika, circ_queue_pop(q), &str_eq_ctx, NULL);
    circ_queue_insert(q, veronika);
    assert_equal(alex, circ_queue_pop(q), &str_eq_ctx, NULL);
    circ_queue_destroy(q);
}

int main() {
    test_empty();
    test_insertion_to_full();
    test_insertion_wraparound();
    test_strings();
}
