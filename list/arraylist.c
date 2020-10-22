#include "arraylist.h"
#include <linux/limits.h>

struct Array_List_ {
    size_t cur_size, capacity;
    const destroy_ctx *destroy;
    const eq_ctx *eq;
    void **data;
};

static bool resize(Array_List *a) {
    if (SIZE_MAX / 2 <= a->capacity) {
        return false;
    }
    a->data = realloc(a->data, (a->capacity *= 2) * sizeof(*a->data));
    return true;
}

Array_List *array_list_init(const destroy_ctx *destroy, const eq_ctx *eq) {
    Array_List *a = malloc(sizeof(*a));

    a->cur_size = 0;
    a->destroy = destroy;
    a->eq = eq;
    a->data = malloc(sizeof(*a->data) * (a->capacity = START));
    
    return a;
}

void array_list_destroy(Array_List *a) {
    if (a->destroy) {
        for (size_t i = 0; i < a->cur_size; i++) {
            a->destroy->destroy(a->data[i], a->destroy->ctx);
        }
    }

    free(a->data);
    free(a);
}

void *array_list_get(const Array_List *a, size_t i) {
    return i < a->cur_size ? a->data[i] : NULL;
}

void array_list_set(Array_List *a, size_t i, void *new_val) {
    if (i >= a->cur_size) {
        return;
    }

    if (a->destroy) {
        a->destroy->destroy(a->data[i], a->destroy->ctx);
    }

    a->data[i] = new_val;
}

void array_list_remove(Array_List *a, size_t i) {
    if (i >= a->cur_size) {
        return;
    }

    if (a->destroy) {
        a->destroy->destroy(a->data[i], a->destroy->ctx);
    }

    memmove(a->data + i, a->data + i + 1, (--a->cur_size - i) * sizeof(*a->data));
}

bool array_list_insert(Array_List *a, size_t i, void *item) {
    if (i > a->cur_size) {
        return false;
    }

    if (a->cur_size == a->capacity) {
        if (!resize(a)) {
            return false;
        }
    }

    memmove(a->data + i + 1, a->data + i, (a->cur_size - i) * sizeof(*a->data));

    a->data[i] = item;
    a->cur_size++;

    return true;
}

bool array_list_contains(const Array_List *a, const void *item) {
    if (!a->eq) {
        return false;
    }
    
    for (size_t i = 0; i < a->cur_size; i++) {
        if (a->eq->eq(a->data[i], item, a->eq->ctx)) {
            return true;
        }
    }

    return false;
}

size_t array_list_size(const Array_List *a) {
    return a->cur_size;
}

bool array_list_is_empty(const Array_List *a) {
    return a->cur_size == 0;
}

void array_list_push(Array_List *a, void *item) {
    array_list_insert(a, a->cur_size, item);
}
