#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <stdlib.h>

#include "../util/comparisons.h"
#include "../util/destroy.h"
#include "../util/kvpair.h"
#include "../set/hashtable.h"

typedef Hashtable_Iter Hashmap_Iter;
typedef struct Hashmap_ Hashmap;

Hashmap *hashmap_init(const hash_ctx *hash, const eq_ctx *key_eq, const destroy_ctx *destroy_key, const destroy_ctx *destroy_value);
void hashmap_destroy(Hashmap *map);

bool hashmap_add(Hashmap *map, void *key, void *value);
void *hashmap_remove(Hashmap *map, void *key);
void *hashmap_get(const Hashmap *map, const void *key);
bool hashmap_contains(const Hashmap *map, const void *key);

void hashmap_addall(Hashmap *map, const Hashmap *other);
bool hashmap_containsall(Hashmap *map, const Hashmap *other);
void hashmap_removeall(Hashmap *map, const Hashmap *other);
void hashmap_retainall(Hashmap *map, const Hashmap *other);

size_t hashmap_size(const Hashmap *map);
bool hashmap_is_empty(const Hashmap *map);
void hashmap_clear(Hashmap *map);


Hashmap_Iter *hashmap_iter_init(const Hashmap *map);
KVPair *hashmap_iter_next(Hashmap_Iter *iter);
bool hashmap_iter_has_next(Hashmap_Iter *iter);
void hashmap_iter_destroy(Hashmap_Iter *iter);
void hashmap_iter_remove(Hashmap_Iter *iter);

#endif
