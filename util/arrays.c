#include "arrays.h"

size_t sorted_array_median_ind(size_t n) {
    return n / 2;
}

size_t split_sorted_array_on_median(void *arr1, size_t nmemb, size_t item_size, void *arr2, void *key) {
    if (nmemb == 0) {
        return 0;
    }
    size_t median = sorted_array_median_ind(nmemb);
    memcpy(key, (char *) arr1 + median * item_size, item_size);

    if (nmemb > 1) {
        memcpy(arr2, (char *) arr1 + (median + 1) * item_size, item_size * nmemb - (median + 1));
    }

    return median;
}

void *median_of_medians(void *arr, size_t nmemb, size_t item_size, size_t i, const comp_ctx *comp_ctx) {
    //catch base case of arr1 length <= 5
    if (nmemb <= COL_SIZE) {
        bubble_sort(arr, nmemb, item_size, comp_ctx);
        return (i < nmemb) ? (char *) arr + i *item_size : NULL;
    }
    
    //sort across the five columns by gathering arrays of references and passing to modified bubble sort
    void *col[COL_SIZE];
    //round up to account for remainder
    size_t ngroups = (size_t) ceil((double) nmemb / COL_SIZE);
    for (size_t i = 0; i < ngroups; i++) {
        int j;
        //terminate early for remainder group
        for (j = 0; j < COL_SIZE && i + j * ngroups < nmemb; j++) {
            col[j] = (char *) arr + (i + j * ngroups) * item_size;
        }
        //once group is in buffer, sort the underlying elements
        //bubble sort is chosen because it does well for small elements that are likely to be already sorted
        bubble_sort_by_ref(col, j, item_size, comp_ctx);
    }

    //now that columns are sorted, the median is a contiguous subarray
    //select the 3rd "row" (at index 2)
    void *pivot = median_of_medians((char *) arr + (COL_SIZE / 2 * ngroups) * item_size, ngroups, item_size, ngroups / 2, comp_ctx);

    //partition around the pivot
    size_t pivot_ind = partition(arr, nmemb, item_size, pivot, comp_ctx);

    //decide which side the pivot should go on
    return (pivot_ind < i) ?
        median_of_medians((char *) arr + (pivot_ind + 1) * item_size, (nmemb - pivot_ind -1), item_size, i - pivot_ind - 1, comp_ctx)
        : median_of_medians(arr, pivot_ind, item_size, i, comp_ctx);
}

size_t partition(void *arr, size_t nmemb, size_t item_size, void *pivot, const comp_ctx *comp_ctx) {
    size_t l = 0, r = nmemb - 1;
    
    while (l < r) {
        while (comp_ctx->comp((char *) arr + l * item_size, pivot, comp_ctx->ctx) < 0) {
            l++;
        }

        while (comp_ctx->comp((char *) arr + r * item_size, pivot, comp_ctx->ctx) > 0) {
            r--;
        }
        if (l < r) {
            swap((char *) arr + l * item_size, (char*) arr + r * item_size, item_size);
        }
    }

    return l;
}
