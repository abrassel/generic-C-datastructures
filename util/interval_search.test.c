#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "interval_search.h"
#include "../testing/test.h"

static int *setup() {
    int *test = malloc(sizeof(int) * 4);
    for (int i = 0; i < 4; i++) {
        test[i] = (i + 1) * (i + 1) + 3 * (i + 1);
    }

    //test = [4, 10, 18, 28]
    return test;
}

static void destroy(int *arr) {
    free(arr);
}

static void test_lower(int *test) {
    int two = 2;
    assert_equal_int(
        0,
        interval_search(test, 4, sizeof(int), &two, &int_comp_ctx),
        "lower"
        );
}

static void test_upper(int *test) {
    int thirty = 30;
    assert_equal_int(
        4,
        interval_search(test, 4, sizeof(int), &thirty, &int_comp_ctx),
        "upper"
        );
}

static void test_mid1(int *test) {
    int sixteen = 16;
    assert_equal_int(
        2,
        interval_search(test, 4, sizeof(int), &sixteen, &int_comp_ctx),
        "mid1"
        );
}

static void test_mid2(int *test) {
    int twentytwo = 22;
    assert_equal_int(
        3,
        interval_search(test, 4, sizeof(int), &twentytwo, &int_comp_ctx),
        "mid2"
        );
}

static void test_exact1(int *test) {
    int ten = 10;
    assert_equal_int(
        -1,
        interval_search(test, 4, sizeof(int), &ten, &int_comp_ctx),
        "exact1"
        );
}

static void test_exact2(int *test) {
    int twentyeight = 28;
    assert_equal_int(
        -3,
        interval_search(test, 4, sizeof(int), &twentyeight, &int_comp_ctx),
        "exact2"
        );
}

static void test_exact3(int *test) {
    int four = 4;
    assert_equal_int(
        INT_MIN,
        interval_search(test, 4, sizeof(int), &four, &int_comp_ctx),
        "exact3"
        );
}

int main() {
    int *test = setup();
    test_lower(test);
    test_upper(test);
    test_mid1(test);
    test_mid2(test);
    test_exact1(test);
    test_exact2(test);
    test_exact3(test);
    destroy(test);
}
