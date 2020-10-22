#include "../testing/test.h"
#include "merge_sort.h"

static void test_small() {
    const char *arr[2] = {"veronika", "hello"};
    merge_sort(arr, 0, sizeof(char *), &str_comp_ctx);
    assert_sorted(arr, 2, sizeof(char *), &str_comp_ctx, NULL);
}

static void test_positive() {
    int arr[] = {7, 7, 5, 5, 0, 2, 10, 12};
    merge_sort(arr, 8, sizeof(int), &int_comp_ctx);
    assert_sorted_int(arr, 8, NULL);
}

static void test_negative() {
    int arr[] = {7, -5, -5, 7, 3};
    merge_sort(arr, 5, sizeof(int), &int_comp_ctx);
    assert_sorted_int(arr, 5, NULL);
}

int main() {
    test_small();
    test_positive();
    test_negative();
}
