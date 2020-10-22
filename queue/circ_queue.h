#ifndef CIRC_QUEUE_H
#define CIRC_QUEUE_H

#include <stdbool.h>
#include <stdlib.h>

#include "../util/destroy.h"

typedef struct Circ_Queue_ Circ_Queue;

Circ_Queue *circ_queue_init(size_t nmemb, const destroy_ctx *destroy_ctx);
void circ_queue_destroy(Circ_Queue *q);

bool circ_queue_insert(Circ_Queue *q, void *datum);
size_t circ_queue_size(Circ_Queue *q);
bool  circ_queue_is_empty(Circ_Queue *q);
void *circ_queue_pop(Circ_Queue *q);
void *circ_queue_peek(Circ_Queue *q);
bool circ_queue_is_full(Circ_Queue *q);

#endif
