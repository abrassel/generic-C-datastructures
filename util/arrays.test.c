#include "../testing/test.h"
#include "arrays.h"
#include "printing.h"

static void test_partition() {
    printf("test_partition\n");
    int arr[] = {5, 3, 8, 11, 2, 20};
    int eleven = 11;
    assert_equal_int(
        partition(arr, 6, sizeof(int), &eleven, &int_comp_ctx),
        4,
        NULL);
    print_int_array(arr, 6);
    printf("\n");
}

static void test_partition_backwards() {
    printf("test_partition_backwards\n");
    int arr[] = {20, 11, 8, 5, 6, 3};
    int five = 5;
    int res = partition(arr, 6, sizeof(int), &five, &int_comp_ctx);
    print_int_array(arr, 6);
    printf("\n");
    assert_equal_int(
        res,
        1,
        NULL);
}

static void test_median_of_medians() {
    printf("test_median_of_medians\n");
    int arr[] = {20, 11, 8, 5, 6, 3};
    median_of_medians(arr, 6, sizeof(int), 3, &int_comp_ctx);
    
    print_int_array(arr, 6); printf("\n");

    int arr2[] = {
        35, 5, 15, 6, 7, 105,
        20, 25, 100, 11, 14, 12,
        80, 90, 0, -10, -12, -11,
        10, 8, 3, 13, -13, 81,
        7, 9, 27, 83
    };
    int arr3[] = {
        35, 5, 15, 6, 7, 105,
        20, 25, 100, 11, 14, 12,
        80, 90, 0, -10, -12, -11,
        10, 8, 3, 13, -13, 81,
        7, 9, 27, 83
    };
    bubble_sort(arr3, 28, sizeof(int), &int_comp_ctx);

    median_of_medians(arr2, 28, sizeof(int), 20, &int_comp_ctx);
    print_int_array(arr2, 28);
    printf("\n");
    assert_equal_int(arr2[20], arr3[20], NULL);
}

int main() {
    test_partition();
    test_partition_backwards();
    test_median_of_medians();
}
