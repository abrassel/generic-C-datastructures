#include "array_lookup.h"

bool array_contains(void *arr, size_t nmemb, size_t item_size, const void *key, const eq_ctx *eq_ctx) {
    return array_ind(arr, nmemb, item_size, key, eq_ctx) != -1;
}

int array_ind(void *arr, size_t nmemb, size_t item_size, const void *key,
              const eq_ctx *eq_ctx) {
    for (size_t i = 0; i < nmemb; i++) {
        if (eq_ctx->eq(key, (char *) arr + i * item_size, eq_ctx->ctx)) {
            return i;
        }
    }

    return -1;
}

bool eq_arrs(const void *arr1, const void *arr2, const void *ctx) {
    struct array_eq *arr_ctx = (struct array_eq *) ctx;

    for (size_t i = 0; i < arr_ctx->nmemb; i++) {
        if (!arr_ctx->item_eq->eq(
                (char *) arr1 + i * arr_ctx->item_size,
                (char *) arr2 + i * arr_ctx->item_size,
                arr_ctx->item_eq->ctx)) {
            return false;
        }
    }

    return true;
}
