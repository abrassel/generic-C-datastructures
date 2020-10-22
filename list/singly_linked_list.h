#ifndef SINGLE_LINKED_LIST_H
#define SINGLE_LINKED_LIST_H

#include <stdlib.h>

#include "../util/comparisons.h"
#include "../util/destroy.h"

typedef struct Singly_Linked_List_ Singly_Linked_List;
typedef struct Singly_Linked_List_Iter_ Singly_Linked_List_Iter;

Singly_Linked_List *singly_linked_list_init(const eq_ctx *eq_ctx,
                                            const destroy_ctx *destroy_ctx);
void singly_linked_list_destroy(Singly_Linked_List *list);

void *singly_linked_list_get(const Singly_Linked_List *a, size_t i);
void *singly_linked_list_get_elm(const Singly_Linked_List *a, void *elm);
void singly_linked_list_set(Singly_Linked_List *a, size_t i, void *new_val);
void singly_linked_list_remove(Singly_Linked_List *a, size_t i);
void singly_linked_list_remove_elm(Singly_Linked_List *a, void *elm);
bool singly_linked_list_insert(Singly_Linked_List *a, size_t i, void *item);
bool singly_linked_list_contains(const Singly_Linked_List *a, const void *item);
size_t singly_linked_list_size(const Singly_Linked_List *a);
bool singly_linked_list_is_empty(const Singly_Linked_List *a);

void singly_linked_list_push(Singly_Linked_List *a, void *item);

Singly_Linked_List_Iter *
singly_linked_list_iter_init(const Singly_Linked_List *l);
void singly_linked_list_iter_destroy(Singly_Linked_List_Iter *iter);
void *singly_linked_list_iter_next(Singly_Linked_List_Iter *iter);
bool singly_linked_list_iter_has_next(const Singly_Linked_List_Iter *iter);


#endif
