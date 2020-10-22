#include "destroy_helpers.h"

void naive_destroy(void *val, const void *ctx) {
    (void) ctx;
    free(val);
}

destroy_ctx naive_destroy_ctx = {
    naive_destroy,
    NULL
};
