#include "circ_queue.h"

struct Circ_Queue_ {
    size_t loc, cur_size, max_size;
    void **data;
    const destroy_ctx *destroy_ctx;
};

Circ_Queue *circ_queue_init(size_t nmemb, const destroy_ctx *destroy_ctx) {
    Circ_Queue *q = malloc(sizeof(*q));
    q->cur_size = 0;
    q->loc = 0;
    q->max_size = nmemb;
    q->data = malloc(nmemb * sizeof(*q->data));
    q->destroy_ctx = destroy_ctx;
    return q;
}

void circ_queue_destroy(Circ_Queue *q) {
    if (q->destroy_ctx) {
        for (size_t i = 0; i < q->cur_size; i++) {
            q->destroy_ctx->destroy(
                q->data[(q->loc + i) % q->max_size],
                q->destroy_ctx->ctx
                );
        }
    }
    free(q->data);
    free(q);
}

bool circ_queue_insert(Circ_Queue *q, void *datum) {
    if (circ_queue_is_full(q)) {
        return false;
    }

    q->data[(q->loc + q->cur_size++) % q->max_size] = datum;
    
    return true;
}

size_t circ_queue_size(Circ_Queue *q) { return q->cur_size; }

bool circ_queue_is_empty(Circ_Queue *q) { return q->cur_size == 0; }

void *circ_queue_pop(Circ_Queue *q) {
    if (circ_queue_is_empty(q)) {
        return NULL;
    }

    void *datum = q->data[q->loc];

    q->loc = (q->loc + 1) % q->max_size;
    q->cur_size--;

    return datum;
}

void *circ_queue_peek(Circ_Queue *q) {
    if (circ_queue_is_empty(q)) {
        return NULL;
    }

    return q->data[q->loc];
}

bool circ_queue_is_full(Circ_Queue *q) {
    return q->cur_size == q->max_size;
}
