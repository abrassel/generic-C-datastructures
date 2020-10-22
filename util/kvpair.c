#include "kvpair.h"

KVPair *create_kvpair(void *key, void *value) {
    KVPair *p = malloc(sizeof(*p));
    p->key = key;
    p->value = value;

    return p;
}

void kvpair_destroy(void *v, const void *ctx) {
    destroy_kvpair_ctx *kvpair_ctx = (destroy_kvpair_ctx *) ctx;
    KVPair *kvpair = (KVPair *) v;

    if (kvpair_ctx->destroy_key) {
        kvpair_ctx->destroy_key->destroy(kvpair->key, kvpair_ctx->destroy_key->ctx);
    }
    if (kvpair_ctx->destroy_value) {
        kvpair_ctx->destroy_value->destroy(kvpair->value, kvpair_ctx->destroy_value->ctx);
    }
    free(kvpair);
}

destroy_ctx *create_destroy_pair(const destroy_ctx *destroy_key, const destroy_ctx *destroy_value) {
    destroy_ctx *overall_ctx = malloc(sizeof(*overall_ctx));
    destroy_kvpair_ctx *kvpair_ctx = malloc(sizeof(*kvpair_ctx));
    kvpair_ctx->destroy_key = destroy_key;
    kvpair_ctx->destroy_value = destroy_value;

    overall_ctx->destroy = kvpair_destroy;
    overall_ctx->ctx = kvpair_ctx;

    return overall_ctx;
}
