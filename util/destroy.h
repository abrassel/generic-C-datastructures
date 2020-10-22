#ifndef DESTROY_H
#define DESTROY_H

#include <stdlib.h>
typedef void (*destroy_fn)(void *v, const void *ctx);
typedef struct {
    destroy_fn destroy;
    const void *ctx;
} destroy_ctx;

#endif
