#include "../testing/test.h"
#include "heap_sort.h"

static void test_arr1() {
    int arr[] = {39, 5, 24, 2, 15, 0, 27, 1, 10, 45, 40, 32, 38, 17, 43, 23, 26, 22, 37};
    heap_sort(arr, 19, sizeof(int), &int_comp_ctx);
    assert_sorted_int(arr, 19, NULL);
}
static void test_arr2() {
    int arr[] = {42, 4, 31, 12};
    heap_sort(arr, 4, sizeof(int), &int_comp_ctx);
    assert_sorted_int(arr, 4, NULL);
}
static void test_arr3() {
    int arr[] = {48, 13, 46, 44, 19, 16, 36, 18, 49, 11, 9, 21, 33, 29, 14, 41, 30, 7, 6, 35, 20, 25, 8, 28, 34, 3, 47};
    heap_sort(arr, 27, sizeof(int), &int_comp_ctx);
    assert_sorted_int(arr, 27, NULL);
}
static void test_empty() {
    int arr[] = {0, 0};
    heap_sort(arr, 0, sizeof(int), &int_comp_ctx);
    assert_sorted_int(arr, 0, NULL);
}

int main() {
    test_empty();
    test_arr1();
    test_arr2();
    test_arr3();
}
