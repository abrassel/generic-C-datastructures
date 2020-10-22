#ifndef COMPARISONS_H
#define COMPARISONS_H

#include <stdbool.h>

typedef int (*comp_fn)(const void *key1, const void *key2, const void *ctx);
typedef bool (*eq_fn)(const void *v1, const void *v2, const void *ctx);

typedef struct {
    eq_fn eq;
    const void *ctx;
} eq_ctx;

typedef struct {
    comp_fn comp;
    const void *ctx;
} comp_ctx;

#endif
