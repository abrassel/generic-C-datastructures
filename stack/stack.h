#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stdlib.h>

#include "../util/destroy.h"

typedef struct Stack_ Stack;

Stack *stack_init(size_t nmemb, const destroy_ctx *destroy_ctx);
void stack_destroy(Stack *s);

bool stack_push(Stack *s, void *datum);
bool stack_is_empty(const Stack *s);
bool stack_is_full(const Stack *s);
void *stack_pop(Stack *s);
void *stack_peek(const Stack *s);
size_t stack_size(const Stack *s);
void **stack_data(const Stack *s);

void stack_clear(Stack *s);

#endif
