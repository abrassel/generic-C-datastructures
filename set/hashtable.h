#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "../util/comparisons.h"
#include "../util/destroy.h"
#include "../problems/prime.h"

typedef struct Hashtable_ Hashtable;
typedef struct Hashtable_Iter_ Hashtable_Iter;
typedef size_t (*hash_function)(const void *value, size_t table_len, const void *ctx);
typedef size_t (*collision_resolution)(size_t start, size_t count, size_t table_len);

typedef struct {
    hash_function hash;
    const void *ctx;
} hash_ctx;

extern hash_ctx identity_hash_ctx;
extern hash_ctx raw_identity_hash_ctx;

size_t linear_probing(size_t start, size_t count, size_t table_len);
size_t identity_hash(const void *value, size_t table_len, const void *ctx);
size_t raw_identity_hash(const void *value, size_t table_len, const void *ctx);

Hashtable *hashtable_init(
    const hash_ctx *hash, collision_resolution resolve, const eq_ctx *eq, const destroy_ctx *destroy, double load_factor);
void hashtable_destroy(Hashtable *table);

bool hashtable_add(Hashtable *table, void *value);
void *hashtable_remove(Hashtable *table, void *value);
bool hashtable_contains(const Hashtable *table, const void *value);
void *hashtable_get(const Hashtable *table, const void *value);

void hashtable_addall(Hashtable *table, const Hashtable *other);
bool hashtable_containsall(Hashtable *table, const Hashtable *other);
void hashtable_removeall(Hashtable *table, const Hashtable *other);
void hashtable_retainall(Hashtable *table, const Hashtable *other);

size_t hashtable_size(const Hashtable *table);
bool hashtable_is_empty(const Hashtable *table);
void hashtable_clear(Hashtable *table);

Hashtable_Iter *hashtable_iter_init(const Hashtable *table);
void *hashtable_iter_next(Hashtable_Iter *iter);
bool hashtable_iter_has_next(Hashtable_Iter *iter);
void hashtable_iter_destroy(Hashtable_Iter *iter);
void hashtable_iter_remove(Hashtable_Iter *iter);

#endif
