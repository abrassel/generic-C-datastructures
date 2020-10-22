#ifndef ARR_HEAP_H
#define ARR_HEAP_H

#include <stdbool.h>
#include <stdlib.h>

#include "../util/comparisons.h"
#include "../util/destroy.h"
#include "../util/kvpair.h"

typedef struct Array_Heap_ Array_Heap;

Array_Heap *array_heap_init(size_t nmemb, const comp_ctx *comp_ctx, const destroy_ctx *destroy_key, const destroy_ctx *destroy_value);
void array_heap_destroy(Array_Heap *heap);
void array_heap_insert(Array_Heap *heap, void *key, void *element);
bool array_heap_is_empty(const Array_Heap *heap);
bool array_heap_is_full(const Array_Heap *heap);
KVPair *array_heap_pull(Array_Heap *heap);
void *array_heap_pull_v(Array_Heap *heap);
KVPair *array_heap_peek(const Array_Heap *heap);
void *array_heap_peek_v(const Array_Heap *heap);

size_t array_heap_size(const Array_Heap *heap);
Array_Heap *array_heap_heapify(void *arr, size_t nmemb, size_t item_size,
                               const comp_ctx *comp_ctx);


// MIN _ QUEUE

typedef Array_Heap Bounded_Min_Queue;

//n is capacity
Bounded_Min_Queue *bounded_min_queue_init(size_t n, const comp_ctx *comp_ctx, const destroy_ctx *destroy_key, const destroy_ctx *destroy_value);
void bounded_min_queue_destroy(Bounded_Min_Queue *q);

void bounded_min_queue_insert(Bounded_Min_Queue *q, void *key, void *value);

KVPair *bounded_min_queue_pop_unordered(Bounded_Min_Queue *q);

// This method empties and destroys the queue.
// Must free the returned array afterwards.
KVPair **bounded_min_queue_getall_ordered(Bounded_Min_Queue *q);

size_t bounded_min_queue_size(const Bounded_Min_Queue *q);
bool bounded_min_queue_is_empty(const Bounded_Min_Queue *q);
bool bounded_min_queue_is_full(const Bounded_Min_Queue *q);

#endif
