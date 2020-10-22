#include "../testing/test.h"
#include "bubble_sort.h"

static void test_sort1() {
    int arr[] = {5, 1, 8, 11, 3, 4, 15};

    bubble_sort(arr, 7, sizeof(int), &int_comp_ctx);
    assert_sorted_int(arr, 7, NULL);
}

static void test_sort2() {
    int arr[] = {1, 8, 4, 10, 20, 3, 5, 4, -5};

    bubble_sort(arr, 9, sizeof(int), &int_comp_ctx);
    assert_sorted_int(arr, 9, NULL);
}

static void test_sort_by_ref() {
    int arr[] = {1, 8, 4, 10, 20, 3, 5, 4, -5};
    int *arr_p[] = {arr + 1, arr + 5, arr + 8};

    bubble_sort_by_ref(arr_p, 3, sizeof(int), &int_comp_ctx);
    assert_sorted(arr_p, 3, sizeof(int *), &int_comp_ctx, NULL);
}

int main() {
    test_sort1();
    test_sort2();
    test_sort_by_ref();
}
