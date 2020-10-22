#include "hashmap.h"

struct Hashmap_ {
    Hashtable *pairs;
    eq_ctx *key_eq;
    destroy_ctx *destroy_pair;
    eq_ctx eq_pair;
    hash_ctx hash_pair;
};

static bool key_eq_fn(const void *v1, const void *v2, const void *ctx) {
    const eq_ctx *key_ctx = (const eq_ctx *) ctx;

    const KVPair *pair1 = (const KVPair *) v1;
    const KVPair *pair2 = (const KVPair *) v2;
    return key_ctx->eq(pair1->key, pair2->key, key_ctx->ctx);
}

static size_t pair_hash_fn(const void *v_raw, size_t table_len, const void *ctx) {
    const KVPair *p = (const KVPair *) v_raw;
    const hash_ctx *key_hash_ctx = (const hash_ctx *) ctx;

    return key_hash_ctx->hash(p->key, table_len, key_hash_ctx->ctx);
}

Hashmap *hashmap_init(const hash_ctx *hash_key, const eq_ctx *key_eq, const destroy_ctx *destroy_key, const destroy_ctx *destroy_value) {
    Hashmap *map = malloc(sizeof(*map));
    
    map->destroy_pair = create_destroy_pair(destroy_key, destroy_value);
    map->eq_pair.eq = key_eq_fn;
    map->eq_pair.ctx = key_eq;
    map->hash_pair.hash = pair_hash_fn;
    map->hash_pair.ctx = hash_key;
    
    map->pairs = hashtable_init(&map->hash_pair, linear_probing, &map->eq_pair, map->destroy_pair, (double) 2 / 3);
    return map;
}
void hashmap_destroy(Hashmap *map) {
    hashtable_destroy(map->pairs);
    free((destroy_kvpair_ctx *) map->destroy_pair->ctx);
    free(map->destroy_pair);
    free(map);
}

bool hashmap_add(Hashmap *map, void *key, void *value) {
    return hashtable_add(map->pairs, create_kvpair(key, value));
}

void *hashmap_remove(Hashmap *map, void *key_raw) {
    KVPair search;
    search.key = key_raw;
    KVPair *res = hashtable_remove(map->pairs, &search);

    if (!res) {
        return NULL;
    }

    const destroy_kvpair_ctx *ctx = (const destroy_kvpair_ctx *) map->destroy_pair->ctx;
    if (ctx->destroy_key) {
        ctx->destroy_key->destroy(res->key, ctx->destroy_key->ctx);
    }
    void *value = res->value;
    free(res);

    return value;
}

void *hashmap_get(const Hashmap *map, const void *key_raw) {
    KVPair search;
    search.key = (void *) key_raw;
    KVPair *k = hashtable_get(map->pairs, &search);
    return k ? k->value : NULL;
}
bool hashmap_contains(const Hashmap *map, const void *key_raw) {
    KVPair search;
    search.key = (void *) key_raw;
    return hashtable_contains(map->pairs, &search);
}
void hashmap_addall(Hashmap *map, const Hashmap *other) {
    hashtable_addall(map->pairs, other->pairs);
}
bool hashmap_containsall(Hashmap *map, const Hashmap *other) {
    return hashtable_containsall(map->pairs, other->pairs);
}
void hashmap_removeall(Hashmap *map, const Hashmap *other) {
    hashtable_removeall(map->pairs, other->pairs);
}

void hashmap_retainall(Hashmap *map, const Hashmap *other) {
    hashtable_retainall(map->pairs, other->pairs);
}

size_t hashmap_size(const Hashmap *map) { return hashtable_size(map->pairs); }

bool hashmap_is_empty(const Hashmap *map) { return hashtable_is_empty(map->pairs); }
void hashmap_clear(Hashmap *map) { hashtable_clear(map->pairs); }


Hashmap_Iter *hashmap_iter_init(const Hashmap *table) {
    return hashtable_iter_init(table->pairs);
}
KVPair *hashmap_iter_next(Hashmap_Iter *iter) {
    return hashtable_iter_next(iter);
}
bool hashmap_iter_has_next(Hashmap_Iter *iter) {
    return hashtable_iter_has_next(iter);
}
void hashmap_iter_destroy(Hashmap_Iter *iter) {
    hashtable_iter_destroy(iter);
}
void hashmap_iter_remove(Hashmap_Iter *iter) {
    hashtable_iter_remove(iter);
}
