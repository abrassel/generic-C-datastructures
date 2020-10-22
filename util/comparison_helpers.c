#include "comparison_helpers.h"

#include "comparisons.h"

int int_comp(const void *x, const void *y, const void *ctx) {
    (void) ctx;
    return * (int *) x - * (int *) y;
}

int raw_int_comp(const void *x_raw, const void *y_raw, const void *ctx) {
    (void) ctx;
    int x = (int) (uintptr_t) x_raw,
	y = (int) (uintptr_t) y_raw;

    return x - y;
}

int raw_size_t_comp(const void *x_raw, const void *y_raw, const void *ctx) {
    (void) ctx;
    size_t x = (size_t) (uintptr_t) x_raw,
	y = (size_t) (uintptr_t) y_raw;

    return x - y;
}

int str_comp(const void *x, const void *y, const void *ctx) {
    (void) ctx;
    return strcmp((char *) x, (char *) y);
}

int naive_comp(const void *x, const void *y, const void *ctx) {
    (void) ctx;
    return (uintptr_t) y - (uintptr_t) x;
}

int double_comp(const void *x, const void *y, const void *ctx) {
    (void) ctx;
    const double *d1 = (const double *) x,
        *d2 = (const double *) y;

    return eq_d(*d1, *d2) ? 0 : *d2 - *d1 > 0 ? -1 : 1;
}

int raw_double_comp(const void *x, const void *y, const void *ctx) {
    (void) ctx;
    double d1 = (double) (uintptr_t) x,
        d2 = (double) (uintptr_t) y;
    return eq_d(d1, d2) ? 0 : d2 - d1 > 0 ? -1 : 1;
}

int inv_comp(const void *x, const void *y, const void *ctx) {
    const comp_ctx *backing_comp_ctx = (const comp_ctx *) ctx;
    return - backing_comp_ctx->comp(x, y, backing_comp_ctx->ctx);
}

bool eq_from_comp(const void *v1, const void *v2, const void *ctx) {
    const comp_ctx *backing_comp_ctx = (const comp_ctx *) ctx;
    return backing_comp_ctx->comp(v1, v2, backing_comp_ctx->ctx) == 0;
}

comp_ctx int_comp_ctx = {int_comp, NULL};
comp_ctx raw_int_comp_ctx = {raw_int_comp, NULL};
comp_ctx raw_size_t_comp_ctx = {raw_size_t_comp, NULL};
comp_ctx str_comp_ctx = {str_comp, NULL};
comp_ctx naive_comp_ctx = {naive_comp, NULL};
comp_ctx double_comp_ctx = {double_comp, NULL};
comp_ctx raw_double_comp_ctx = {raw_double_comp, NULL};

eq_ctx int_eq_ctx = {eq_from_comp, &int_comp_ctx};
eq_ctx raw_int_eq_ctx = {eq_from_comp, &raw_int_comp_ctx};
eq_ctx raw_size_t_eq_ctx = {eq_from_comp, &raw_size_t_comp_ctx};
eq_ctx str_eq_ctx = {eq_from_comp, &str_comp_ctx};
eq_ctx naive_eq_ctx = {eq_from_comp, &naive_comp_ctx};
eq_ctx double_eq_ctx = {eq_from_comp, &double_comp_ctx};
eq_ctx raw_double_eq_ctx = {eq_from_comp, &raw_double_comp_ctx};

