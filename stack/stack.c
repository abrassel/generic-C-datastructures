#include "stack.h"

struct Stack_ {
    size_t size;
    void **data;
    size_t cur_size;
    const destroy_ctx *destroy_ctx;
};

Stack *stack_init(size_t nmemb, const destroy_ctx *destroy_ctx) {
    Stack *s = malloc(sizeof(*s));

    if (!s) {
        return NULL;
    }
    
    s->size = nmemb;
    s->cur_size = 0;
    s->data = malloc(nmemb * sizeof(*s->data));
    s->destroy_ctx = destroy_ctx;

    return s;
}

void stack_destroy(Stack *s) {
    if (!s) {
        return;
    }
    
    stack_clear(s);
    
    free(s->data);
    free(s);
}

bool stack_push(Stack *s, void *datum) {
    if (stack_is_full(s)) {
        return false;
    }

    s->data[s->cur_size++] = datum;
    return true;
}

bool stack_is_empty(const Stack *s) { return s->cur_size == 0; }

bool stack_is_full(const Stack *s) { return s->cur_size == s->size; }

void *stack_pop(Stack *s) {
    if (stack_is_empty(s)) {
        return NULL;
    }

    return s->data[--s->cur_size];
}

void *stack_peek(const Stack *s) {
    if (stack_is_empty(s)) {
        return NULL;
    }

    return s->data[s->cur_size - 1];
}

size_t stack_size(const Stack *s) {
    return s->size;
}

void **stack_data(const Stack *s) { return s->data; }

void stack_clear(Stack *s) {
    for (size_t i = 0; i < s->cur_size; i++) {
        if (s->destroy_ctx) {
            s->destroy_ctx->destroy(s->data[i], s->destroy_ctx->ctx);
        }
    }
    s->cur_size = 0;
}
