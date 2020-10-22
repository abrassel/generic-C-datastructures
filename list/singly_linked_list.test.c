#include "../testing/test.h"
#include "singly_linked_list.h"
#include "../util/comparisons.h"

static void empty_assertions(Singly_Linked_List *list) {
    assert_true(singly_linked_list_is_empty(list), NULL);
    assert_equal_int(singly_linked_list_size(list), 0, NULL);
    assert_false(singly_linked_list_contains(list, (char *) "hello"), NULL);
    assert_null(singly_linked_list_get(list, 0), NULL);
    assert_null(singly_linked_list_get(list, 1), NULL);
}

static void test_empty() {
    Singly_Linked_List *list = singly_linked_list_init(&str_eq_ctx, NULL);

    empty_assertions(list);
    
    singly_linked_list_destroy(list);
}

static void test_singleton() {
    Singly_Linked_List *list = singly_linked_list_init(&str_eq_ctx, NULL);

    assert_true(singly_linked_list_insert(list, 0, (char *) "hello"), NULL);
    assert_equal_int(singly_linked_list_size(list), 1, NULL);
    assert_true(singly_linked_list_contains(list, (char *) "hello"), NULL);
    assert_equal(singly_linked_list_get(list, 0), "hello", &str_eq_ctx, NULL);

    singly_linked_list_remove(list, 0);
    
    empty_assertions(list);
    
    singly_linked_list_destroy(list);  
}

static void test_add_front() {
    Singly_Linked_List *list = singly_linked_list_init(&str_eq_ctx, NULL);

    singly_linked_list_insert(list, 0, (char *) "hello");
    singly_linked_list_insert(list, 0, (char *) "world");

    assert_equal_int(2, singly_linked_list_size(list), NULL);
    assert_equal(singly_linked_list_get(list, 0), (char *) "world", &str_eq_ctx, NULL);
    assert_equal(singly_linked_list_get(list, 1), (char *) "hello", &str_eq_ctx, NULL);

    singly_linked_list_destroy(list);  
}

static void test_add_back() {
    Singly_Linked_List *list = singly_linked_list_init(&str_eq_ctx, NULL);
    singly_linked_list_push(list, (char *) "hello");
    singly_linked_list_push(list, (char *) "world");

    assert_equal_int(2, singly_linked_list_size(list), NULL);
    assert_equal(singly_linked_list_get(list, 0), (char *) "hello", &str_eq_ctx, NULL);
    assert_equal(singly_linked_list_get(list, 1), (char *) "world", &str_eq_ctx, NULL);


    singly_linked_list_destroy(list);  
}

static void test_add_middle() {
    Singly_Linked_List *list = singly_linked_list_init(&str_eq_ctx, NULL);

    singly_linked_list_push(list, (char *) "hello");
    singly_linked_list_push(list, (char *) "world");

    singly_linked_list_insert(list, 1, (char *) "tom");
    assert_equal_int(3, singly_linked_list_size(list), NULL);
    assert_equal(singly_linked_list_get(list, 0), (char *) "hello", &str_eq_ctx, NULL);
    assert_equal(singly_linked_list_get(list, 1), (char *) "tom", &str_eq_ctx, NULL);    
    assert_equal(singly_linked_list_get(list, 2), (char *) "world", &str_eq_ctx, NULL);    

    singly_linked_list_destroy(list);  
}

static void test_remove_front() {
    Singly_Linked_List *list = singly_linked_list_init(&str_eq_ctx, NULL);
    
    singly_linked_list_insert(list, 0, (char *) "hello");
    singly_linked_list_insert(list, 0, (char *) "world");

    singly_linked_list_remove(list, 0);
    assert_equal_int(1, singly_linked_list_size(list), NULL);
    assert_equal(singly_linked_list_get(list, 0), (char *) "hello", &str_eq_ctx, NULL);
    
    singly_linked_list_destroy(list);  
}

static void test_remove_middle() {
    Singly_Linked_List *list = singly_linked_list_init(&str_eq_ctx, NULL);

    singly_linked_list_push(list, (char *) "hello");
    singly_linked_list_push(list, (char *) "world");

    singly_linked_list_insert(list, 1, (char *) "tom");

    singly_linked_list_remove(list, 1);

    assert_equal_int(2, singly_linked_list_size(list), NULL);
    assert_equal(singly_linked_list_get(list, 0), (char *) "hello", &str_eq_ctx, NULL);
    assert_equal(singly_linked_list_get(list, 1), (char *) "world", &str_eq_ctx, NULL);    

    singly_linked_list_destroy(list);  
}

static void test_remove_end() {
    Singly_Linked_List *list = singly_linked_list_init(&str_eq_ctx, NULL);
    singly_linked_list_push(list, (char *) "hello");
    singly_linked_list_push(list, (char *) "world");
    singly_linked_list_push(list, (char *) "again");

    singly_linked_list_remove(list, 2);

    assert_equal_int(2, singly_linked_list_size(list), NULL);

    assert_equal(singly_linked_list_get(list, 0), (char *) "hello", &str_eq_ctx, NULL);
    assert_equal(singly_linked_list_get(list, 1), (char *) "world", &str_eq_ctx, NULL);   
    
    singly_linked_list_push(list, (char *) "once more");
    assert_equal_int(3, singly_linked_list_size(list), NULL);

    assert_equal(singly_linked_list_get(list, 2), (char *) "once more", &str_eq_ctx, NULL);
    
    singly_linked_list_destroy(list);
}

static void test_iter() {
    Singly_Linked_List *l = singly_linked_list_init(NULL, NULL);

    const char *words[] = {"hello", "world", "goodbye"};

    for (int i = 0; i < 3; i++) {
        singly_linked_list_push(l, (char *) words[i]);
    }

    Singly_Linked_List_Iter *iter = singly_linked_list_iter_init(l);

    for (int i = 0; i < 3; i++) {
        assert_true(singly_linked_list_iter_has_next(iter), NULL);
        assert_equal(singly_linked_list_iter_next(iter), words[i], &str_eq_ctx, NULL);
    }

    assert_false(singly_linked_list_iter_has_next(iter), NULL);
    singly_linked_list_iter_destroy(iter);
    
    singly_linked_list_destroy(l);
}

int main() {
    test_empty();
    test_singleton();
    test_add_front();
    test_add_back();
    test_add_middle();

    test_remove_front();
    test_remove_middle();
    test_remove_end();
    test_iter();
}
