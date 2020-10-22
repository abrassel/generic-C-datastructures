#include "hashtable.h"

#define START_SIZE 13
#define REMOVED_VALUE -1

typedef struct {
    void *value;
    bool removed;
} Cell;

typedef struct {
    const hash_ctx *hash_ctx;
    collision_resolution resolve;
    const eq_ctx *eq_ctx;
    const destroy_ctx *destroy;
} Hash_Cbs;

struct Hashtable_Iter_ {
    const Hashtable *table;
    size_t cur, found;
};

struct Hashtable_ {
    Cell *data;
    Hash_Cbs cbs;
    size_t length, size;
    double load_factor;
};

size_t linear_probing(size_t start, size_t count, size_t table_len) {
    return (start + count) % table_len;
}

size_t identity_hash(const void *value, size_t table_len, const void *ctx) {
    (void) ctx;
    return * (int *) value % table_len;
}

size_t raw_identity_hash(const void *value, size_t table_len, const void *ctx) {
    (void) ctx;
    return (uintptr_t) value % table_len;
}


hash_ctx identity_hash_ctx = {identity_hash, NULL};
hash_ctx raw_identity_hash_ctx = {
    raw_identity_hash,
    NULL
};

static bool empty(Cell c) { return c.removed || !c.value; }

static void resize(Hashtable *table) {
    //copy all elements out
    void **copies = malloc(table->size * sizeof(*table->data));
    
    Hashtable_Iter *iter = hashtable_iter_init(table);
    for (size_t i = 0; hashtable_iter_has_next(iter); i++) {
        copies[i] = hashtable_iter_next(iter);
    }
    hashtable_iter_destroy(iter);

    //now, free the old data and re-insert
    free(table->data);
    size_t old_table_size = table->size;

    //double length
    table->data = calloc(table->length = next_prime(table->length * 2), sizeof(Cell));
    table->size = 0;
    
    //reinsert
    for (size_t i = 0; i < old_table_size; i++) {
        hashtable_add(table, copies[i]);
    }

    free(copies);
}

Hashtable *hashtable_init(const hash_ctx *hash_ctx,
                          collision_resolution resolve, const eq_ctx *eq_ctx,
                          const destroy_ctx *destroy, double load_factor) {
    Hashtable *table = malloc(sizeof(*table));

    table->data = calloc((table->length = START_SIZE), sizeof(Cell));
    table->cbs.resolve = resolve;
    table->cbs.hash_ctx = hash_ctx;
    table->cbs.destroy = destroy;
    table->cbs.eq_ctx = eq_ctx;
    table->size = 0;
    table->load_factor = load_factor;
    return table;
}

void hashtable_destroy(Hashtable *table) {
    if (table->cbs.destroy) {
        for (size_t i = 0; i < table->length; i++) {
            if (!empty(table->data[i])) {
                table->cbs.destroy->destroy(table->data[i].value, table->cbs.destroy->ctx);
            }
        }
    }

    free(table->data);
    free(table);
}

bool hashtable_add(Hashtable *table, void *value) {
    if (table->size >= table->length * table->load_factor) {
        resize(table);
    }

    size_t start = table->cbs.hash_ctx->hash(value, table->length, table->cbs.hash_ctx->ctx), cur = start;

    for (size_t count = 0; !empty(table->data[cur]); count++) {
        if (table->cbs.eq_ctx->eq(table->data[cur].value, value, table->cbs.eq_ctx->ctx)) {
            return false;
        }
        
        cur = table->cbs.resolve(start, count, table->length);
    }
    
    table->data[cur].removed = false;
    table->data[cur].value = value;
    table->size++;

    return true;
}

void *hashtable_remove(Hashtable *table, void *value) {
    size_t start = table->cbs.hash_ctx->hash(value, table->length, table->cbs.hash_ctx->ctx);

    for (size_t count = 0, cur = start; !empty(table->data[cur]); count++) {
        if (table->cbs.eq_ctx->eq(table->data[cur].value, value, table->cbs.eq_ctx->ctx)) {
            //should removing destroy?
            void *v = table->data[cur].value;
            table->data[cur].value = NULL;
            table->data[cur].removed = true;
            table->size--;
            return v;
        }

        cur = table->cbs.resolve(start, count, table->length);
    }

    return NULL;
}

void *hashtable_get(const Hashtable *table, const void *value) {
    size_t start = table->cbs.hash_ctx->hash(value, table->length, table->cbs.hash_ctx->ctx), cur = start;

    for (size_t count = 0; table->data[cur].removed || table->data[cur].value; count++) {
        if (!table->data[cur].removed && table->cbs.eq_ctx->eq(table->data[cur].value, value, table->cbs.eq_ctx->ctx)) {
            return table->data[cur].value;
        }

        cur = table->cbs.resolve(start, count, table->length);
    }

    return NULL;
}

bool hashtable_contains(const Hashtable *table, const void *value) {
    return hashtable_get(table, value) != NULL;
}

void hashtable_addall(Hashtable *table, const Hashtable *other) {
    size_t final_size = table->size + other->size;
    while (table->length * table->load_factor < final_size) {
        resize(table);
    }

    Hashtable_Iter *iter = hashtable_iter_init(other);
    while (hashtable_iter_has_next(iter)) {
        hashtable_add(table, hashtable_iter_next(iter));
    }
    hashtable_iter_destroy(iter);
}

bool hashtable_containsall(Hashtable *table, const Hashtable *other) {
    Hashtable_Iter *iter = hashtable_iter_init(other);
    while (hashtable_iter_has_next(iter)) {
        if (!hashtable_contains(table, hashtable_iter_next(iter))) {
            hashtable_iter_destroy(iter);
            return false;
        }
    }

    hashtable_iter_destroy(iter);
    return true;
}

void hashtable_removeall(Hashtable *table, const Hashtable *other) {
    Hashtable_Iter *iter = hashtable_iter_init(other);
    while (hashtable_iter_has_next(iter)) {
        hashtable_remove(table, hashtable_iter_next(iter));
    }

    hashtable_iter_destroy(iter);
}

void hashtable_retainall(Hashtable *table, const Hashtable *other) {
    Hashtable_Iter *iter = hashtable_iter_init(table);
    while (hashtable_iter_has_next(iter)) {
        if (!hashtable_contains(other, hashtable_iter_next(iter))) {
            hashtable_iter_remove(iter);
        }
    }

    hashtable_iter_destroy(iter);
}

size_t hashtable_size(const Hashtable *table) {
    return table->size;
}
bool hashtable_is_empty(const Hashtable *table) { return table->size == 0; }

void hashtable_clear(Hashtable *table) {
    if (table->cbs.destroy) {
        for (size_t i = 0; i < table->length; i++) {
            if (!empty(table->data[i])) {
                table->cbs.destroy->destroy(table->data[i].value, table->cbs.destroy->ctx);
            }
        }
    }
    memset(table->data, 0, sizeof(*table->data));
    table->size = 0;
}

Hashtable_Iter *hashtable_iter_init(const Hashtable *table) {
    Hashtable_Iter *i = calloc(1, sizeof(*i));
    i->table = table;
    return i;
}

void *hashtable_iter_next(Hashtable_Iter *iter) {
    while(empty(iter->table->data[iter->cur])) {
        iter->cur = (iter->cur + 1) % iter->table->length;
    }

    void *value = iter->table->data[iter->cur].value;
    
    iter->found++;
    iter->cur = (iter->cur + 1) % iter->table->length;
    
    return value;
}

bool hashtable_iter_has_next(Hashtable_Iter *iter) {
    return iter->found < iter->table->size;
}

void hashtable_iter_destroy(Hashtable_Iter *iter) {
    free(iter);
}

void hashtable_iter_remove(Hashtable_Iter *iter) {
    size_t last_value = (iter->cur - 1) % iter->table->length;
    if (iter->table->cbs.destroy) {
        iter->table->cbs.destroy->destroy(iter->table->data[last_value].value, iter->table->cbs.destroy->ctx);
    }
    iter->table->data[last_value].value = NULL;
    iter->table->data[last_value].removed = true;
}
