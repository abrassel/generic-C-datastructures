#ifndef KVPAIR_H
#define KVPAIR_H

#include <stdlib.h>

#include "destroy.h"

typedef struct KVPair_ {
    void *key;
    void *value;
} KVPair;

typedef struct {
    const destroy_ctx *destroy_key;
    const destroy_ctx *destroy_value;
} destroy_kvpair_ctx;

void kvpair_destroy(void *v, const void *ctx);
destroy_ctx *create_destroy_pair(const destroy_ctx *destroy_key,
                                 const destroy_ctx *destroy_value);
KVPair *create_kvpair(void *key, void *value);
#endif
