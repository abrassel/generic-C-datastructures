#include "bubble_sort.h"

void bubble_sort(void *arr, size_t nmemb, size_t item_size, const comp_ctx *comp_ctx) {
    for (int i = 0; i < (int) nmemb - 1; i++) {
        //starting at each index
        bool swapped = false;
        for (size_t j = 0; j < nmemb - 1 - i; j++) {
            //iterate up until either the end is reached or the swap is no longer useful
            void *cur = (char *) arr + j * item_size;
            void *next = (char *) cur + item_size;
            if (comp_ctx->comp(cur, next, comp_ctx->ctx) > 0) {
                swap(cur, next, item_size);
                swapped = true;
            }
        }

        if (!swapped) {
            break;
        }
    }
}

void bubble_sort_by_ref(void *arr_t, size_t nmemb, size_t item_size, const comp_ctx *comp_ctx) {
    void **arr = (void **) arr_t;
    for (int i = 0; i < (int) nmemb - 1; i++) {
        //starting at each index
        bool swapped = false;
        for (size_t j = 0; j < nmemb - 1 - i; j++) {
            //iterate up until either the end is reached or the swap is no longer useful
            void *cur = arr[j];
            void *next = arr[j+1];
            if (comp_ctx->comp(cur, next, comp_ctx->ctx) > 0) {
                swap(cur, next, item_size);
                swapped = true;
            }
        }

        if (!swapped) {
            break;
        }
    }
}
