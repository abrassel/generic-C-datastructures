#ifndef COMPARISON_HELPERS_H
#define COMPARISON_HELPERS_H

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "comparisons.h"
#include "double_comp.h"

int int_comp(const void *x, const void *y, const void *ctx);
int str_comp(const void *x, const void *y, const void *ctx);
int naive_comp(const void *x, const void *y, const void *ctx);
int raw_double_comp(const void *x, const void *y, const void *ctx);
int raw_int_comp(const void *x, const void *y, const void *ctx);
int raw_size_t_comp(const void *x, const void *y, const void *ctx);

int double_comp(const void *x, const void *y, const void *ctx);

int inv_comp(const void *x, const void *y, const void *ctx);

bool eq_from_comp(const void *v1, const void *v2, const void *ctx);

extern comp_ctx int_comp_ctx;
extern comp_ctx raw_int_comp_ctx;
extern comp_ctx raw_size_t_comp_ctx;
extern comp_ctx str_comp_ctx;
extern comp_ctx naive_comp_ctx;
extern comp_ctx double_comp_ctx;
extern comp_ctx raw_double_comp_ctx;

extern eq_ctx int_eq_ctx;
extern eq_ctx raw_int_eq_ctx;
extern eq_ctx raw_size_t_eq_ctx;
extern eq_ctx str_eq_ctx;
extern eq_ctx naive_eq_ctx;
extern eq_ctx double_eq_ctx;
extern eq_ctx raw_double_eq_ctx;

#endif
