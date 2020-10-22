#include "../testing/test.h"
#include "counting_sort.h"

static void test_empty() {
    int arr[2] = {0, 0};
    counting_sort(arr, 0, -5, 5);
    assert_sorted_int(arr, 0, NULL);
}

static void test_positive() {
    int arr[] = {7, 7, 5, 5, 0, 2, 10, 12};
    counting_sort(arr, 8, 0, 12);
    assert_sorted_int(arr, 8, NULL);
}

static void test_negative() {
    int arr[] = {7, -5, -5, 7, 3};
    counting_sort(arr, 5, -5, 7);
    assert_sorted_int(arr, 5, NULL);
}

int main() {
    test_empty();
    test_positive();
    test_negative();
}
