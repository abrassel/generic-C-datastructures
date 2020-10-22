#ifndef DESTROY_HELPERS_H
#define DESTROY_HELPERS_H

#include <stdlib.h>
#include "destroy.h"

void naive_destroy(void *val, const void *ctx);

extern destroy_ctx naive_destroy_ctx;

#endif
