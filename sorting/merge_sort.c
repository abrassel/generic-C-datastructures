#include "merge_sort.h"

static void copy_to(void *dest, void *src, size_t i, size_t j, size_t item_size) {
    memcpy((char *) dest + i * item_size, (char *) src + j * item_size, item_size);
}

void merge(void *left, size_t l_n, void *right, size_t r_n, void *arr, const comp_ctx *comp_ctx, size_t item_size) {
    size_t l = 0, r = 0;

    int i;
    for (i = 0; l < l_n && r < r_n; i++) {
        void *l_val = (char *) left + l * item_size;
        void *r_val = (char *) right + r * item_size;

        void *target;
        if (comp_ctx->comp(l_val, r_val, comp_ctx->ctx) > 0) {
            target = r_val;
            r++;
        }
        else {
            target = l_val;
            l++;
        }

        copy_to(arr, target, i, 0, item_size);
    }

    for (size_t j = r; j < r_n; j++, i++) {
        copy_to(arr, right, i, j, item_size);
    }

    for (size_t j = l; j < l_n; j++, i++) {
        copy_to(arr, left, i, j, item_size);
    }
}

void merge_sort(void *arr, size_t nmemb, size_t item_size, const comp_ctx *comp_ctx) {
    if (nmemb <= 1) {
        return;
    }

    size_t m = nmemb / 2;
    void *right = (char *)arr + m * item_size;
    
    //sort left
    merge_sort(arr, m, item_size, comp_ctx);
    merge_sort(right, nmemb - m, item_size, comp_ctx);

    //create buffer to hold result
    void *buf = malloc(nmemb * item_size);
    merge(arr, m, right, nmemb - m, buf, comp_ctx, item_size);
    memcpy(arr, buf, nmemb * item_size);
    free(buf);
}
