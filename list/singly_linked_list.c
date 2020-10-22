#include "singly_linked_list.h"

typedef struct Node_ {
    void *value;
    struct Node_ *next;
} Node;

struct Singly_Linked_List_ {
    Node *root, *last;
    size_t cur_size;
    const eq_ctx *eq_ctx;
    const destroy_ctx *destroy_ctx;
};

struct Singly_Linked_List_Iter_ {
    Node *cur;
};

static Node *find_node(const Singly_Linked_List *list, size_t i, Node **prev) {
    if (prev) {
        *prev = NULL;
    }
    if (i > list->cur_size) {
        return NULL;
    }

    Node *cur;
    for (cur = list->root; i > 0 && cur; i--, cur = cur->next) {
        if (prev) {
            *prev = cur;
        }
    }

    return cur;
}

Singly_Linked_List *singly_linked_list_init(const eq_ctx *eq_ctx,
                                            const destroy_ctx *destroy_ctx) {
    Singly_Linked_List *list = malloc(sizeof(*list));

    list->root = list->last = NULL;
    list->cur_size = 0;
    list->eq_ctx = eq_ctx;
    list->destroy_ctx = destroy_ctx;
    
    return list;
}

void singly_linked_list_destroy(Singly_Linked_List *list) {
    Node *cur = list->root;
    while(cur) {
        Node *temp = cur;
        if (list->destroy_ctx) {
            list->destroy_ctx->destroy(cur->value, list->destroy_ctx->ctx);
        }
        cur = cur->next;
        free(temp);
    }

    free(list);
}

void *singly_linked_list_get(const Singly_Linked_List *list, size_t i) {
    if (i >= list->cur_size) {
        return NULL;
    }
    if (i == list->cur_size - 1) {
        return list->last ? list->last->value : NULL;
    }

    return find_node(list, i, NULL)->value;
}
void *singly_linked_list_get_elm(const Singly_Linked_List *a, void *elm) {
    for (Node *cur = a->root; cur; cur = cur->next) {
        if (a->eq_ctx->eq(cur->value, elm, a->eq_ctx->ctx)) {
            return cur->value;
        }
    }

    return NULL;
}

void singly_linked_list_set(Singly_Linked_List *list, size_t i, void *new_val) {
    if (list->cur_size == 0 || i >= list->cur_size) {
        return;
    }

    Node *n;
    if (i == list->cur_size - 1) {
        n = list->last;
    }
    else {
        n = find_node(list, i, NULL);
    }

    if (list->destroy_ctx) {
        list->destroy_ctx->destroy(n->value, list->destroy_ctx->ctx);
    }
    n->value = new_val;
}
static void singly_linked_list_remove_p(Singly_Linked_List *list, Node *cur, Node *prev) {
    if (!prev) {
        //singleton
        list->root = cur->next;
        if (!cur->next) {
            //this was tail
            //but no prev so empty list
            list->last = NULL;
        }
    }
    else {
        prev->next = cur->next;
        if (!cur->next) {
            list->last = prev;
        }
    }

    //free up node
    if (list->destroy_ctx) {
        list->destroy_ctx->destroy(cur->value, list->destroy_ctx->ctx);
    }
    list->cur_size--;
    free(cur);
}
void singly_linked_list_remove(Singly_Linked_List *list, size_t i) {
    if (list->cur_size == 0 || i >= list->cur_size) {
        return;
    }
    Node *prev, *n = find_node(list, i, &prev);

    singly_linked_list_remove_p(list, n, prev);
}
void singly_linked_list_remove_elm(Singly_Linked_List *a, void *elm) {
    Node *prev = NULL, *cur = a->root;
    while (cur) {
        if (a->eq_ctx->eq(cur->value, elm, a->eq_ctx->ctx)) {
            singly_linked_list_remove_p(a, cur, prev);
            break;
        }
        prev = cur;
        cur = cur->next;
    }
}

bool singly_linked_list_insert(Singly_Linked_List *list, size_t i, void *item) {
    if (i > list->cur_size) {
        return false;
    }

    Node *new_node = malloc(sizeof(*new_node));
    new_node->next = NULL;
    new_node->value = item;
    
    if (i == list->cur_size) {
        if (!list->last) {
            list->root = list->last = new_node;
        }
        else {
            list->last->next = new_node;
            list->last = new_node;
        }
    }
    else {
        Node *prev, *cur = find_node(list, i, &prev);
        if (!prev) {
            //root insertion case
            list->root = new_node;
        }
        else {
            prev->next = new_node;
        }
        new_node->next = cur;
    }
    list->cur_size++;
    return true;
}
bool singly_linked_list_contains(const Singly_Linked_List *list, const void *item) {
    for (Node *cur = list->root; cur; cur = cur->next) {
        if (list->eq_ctx->eq(cur->value, item, list->eq_ctx->ctx)) {
            return true;
        }
    }
    return false;
}

size_t singly_linked_list_size(const Singly_Linked_List *list) { return list->cur_size; }

bool singly_linked_list_is_empty(const Singly_Linked_List *list) {
    return list->cur_size == 0;
}

void singly_linked_list_push(Singly_Linked_List *list, void *item) {
    singly_linked_list_insert(list, list->cur_size, item);
}

Singly_Linked_List_Iter *
singly_linked_list_iter_init(const Singly_Linked_List *l) {
    Singly_Linked_List_Iter *iter = malloc(sizeof(*iter));
    iter->cur = l->root;

    return iter;
}
void singly_linked_list_iter_destroy(Singly_Linked_List_Iter *iter) {
    free(iter);
}
void *singly_linked_list_iter_next(Singly_Linked_List_Iter *iter) {
    void *value = iter->cur->value;
    iter->cur = iter->cur->next;
    return value;
}
bool singly_linked_list_iter_has_next(const Singly_Linked_List_Iter *iter) {
    return iter->cur != NULL;
}
