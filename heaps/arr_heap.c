#include "arr_heap.h"

struct Array_Heap_ {
    size_t size;
    size_t cur_size;
    KVPair **data;
    const comp_ctx *comp_ctx;
    const destroy_ctx *destroy_key_ctx;
    const destroy_ctx *destroy_value_ctx;
} ;

static KVPair *create_entry(void *key, void *value) {
    KVPair *e = malloc(sizeof(KVPair));
    e->key = key;
    e->value = value;
    return e;
}

static void swap(Array_Heap *heap, size_t ind1, size_t ind2) {
    KVPair *temp = heap->data[ind1];
    heap->data[ind1] = heap->data[ind2];
    heap->data[ind2] = temp;
}

static size_t left_child(size_t ind) { return 2 * ind + 1; }

static size_t right_child(size_t ind) { return 2 * ind + 2; }

static int parent(size_t ind) { return (ind - 1) / 2; }

static void bubble_down(Array_Heap *heap, size_t ind) {
    size_t parent_ind = ind;

    // terminate when parent is no longer internal
    while (parent_ind < heap->cur_size / 2) {
        // pick max of children
        size_t left_child_ind = left_child(parent_ind);
        size_t right_child_ind = right_child(parent_ind);

        int max_child_ind = -1;
        if (left_child_ind >= heap->cur_size) {
            max_child_ind = right_child_ind;
        } else if (right_child_ind >= heap->cur_size) {
            max_child_ind = left_child_ind;
        } else if (heap->comp_ctx->comp(heap->data[left_child_ind]->key,
                                        heap->data[right_child_ind]->key, heap->comp_ctx->ctx) > 0) {
            max_child_ind = left_child_ind;
        } else {
            max_child_ind = right_child_ind;
        }

        if (max_child_ind == -1 || heap->comp_ctx->comp(heap->data[max_child_ind]->key, heap->data[parent_ind]->key, heap->comp_ctx->ctx) <= 0) {
            break;
        }

        swap(heap, parent_ind, max_child_ind);
        parent_ind = max_child_ind;
    }
}

static void bubble_up(Array_Heap *heap) {
    // start at newly inserted element and work way up
    int cur_ind = heap->cur_size - 1;
    int parent_ind = parent(cur_ind);

    while (parent_ind >= 0) {
        if (heap->comp_ctx->comp(heap->data[parent_ind]->key, heap->data[cur_ind]->key, heap->comp_ctx->ctx) >= 0) {
            break;
        }

        swap(heap, cur_ind, parent_ind);
        cur_ind = parent_ind;
        parent_ind = parent(cur_ind);
    }
}

Array_Heap *array_heap_init(size_t nmemb,
                            const comp_ctx *comp_ctx, const destroy_ctx *destroy_key, const destroy_ctx *destroy_value) {
    Array_Heap *heap = malloc(sizeof(*heap));
    heap->size = nmemb;
    heap->cur_size = 0;
    heap->destroy_key_ctx = destroy_key;
    heap->destroy_value_ctx = destroy_value;
    heap->data = malloc(nmemb * sizeof(KVPair *));
    heap->comp_ctx = comp_ctx;
    return heap;
}
void array_heap_destroy(Array_Heap *heap) {
    for (size_t i = 0; i < heap->cur_size; i++) {
      if (heap->destroy_key_ctx) {
          heap->destroy_key_ctx->destroy(heap->data[i]->key, heap->destroy_key_ctx->ctx);
      }
      if (heap->destroy_value_ctx) {

          heap->destroy_value_ctx->destroy(heap->data[i]->value, heap->destroy_value_ctx->ctx);
      }
        free(heap->data[i]);
    }
    free(heap->data);
    free(heap);
}

void array_heap_insert(Array_Heap *heap, void *key, void *element) {
    if (array_heap_is_full(heap)) {
        return;
    }

    heap->data[heap->cur_size++] = create_entry(key, element);
    bubble_up(heap);
}
bool array_heap_is_empty(const Array_Heap *heap) { return heap->cur_size == 0; }

KVPair *array_heap_pull(Array_Heap *heap) {
    if (array_heap_is_empty(heap)) {
        return NULL;
    }

    KVPair *data = heap->data[0];
    heap->data[0] = heap->data[--heap->cur_size];
    bubble_down(heap, 0);

    return data;
}

void *array_heap_pull_v(Array_Heap *heap) {
    if (array_heap_is_empty(heap)) {
        return NULL;
    }

    KVPair *data = heap->data[0];
    void *value = data->value;

    if (heap->destroy_key_ctx) {
        heap->destroy_key_ctx->destroy(data->key, heap->destroy_key_ctx->ctx);
    }
    free(data);
    
    heap->data[0] = heap->data[--heap->cur_size];
    bubble_down(heap, 0);

    return value;
}

KVPair *array_heap_peek(const Array_Heap *heap) {
    return array_heap_is_empty(heap) ? NULL : heap->data[0];
}

void *array_heap_peek_v(const Array_Heap *heap) {
    if (array_heap_is_empty(heap)) {
        return NULL;
    }

    return heap->data[0]->value;
}

size_t array_heap_size(const Array_Heap *heap) { return heap->cur_size; }
bool array_heap_is_full(const Array_Heap *heap) {
    return heap->cur_size == heap->size;
}

Array_Heap *array_heap_heapify(void *arr, size_t nmemb, size_t item_size, const comp_ctx *comp_ctx) {
    Array_Heap *heap = array_heap_init(nmemb, comp_ctx, NULL, NULL);

    for (size_t i = 0; i < nmemb; i++) {
        void *val = (char *) arr + i * item_size;
        heap->data[i] = create_entry(val, val);
    }

    heap->cur_size = nmemb;

    //heapify half the elements
    for (int i = nmemb / 2 - 1; i >= 0; i--) {
        bubble_down(heap, i);
    }
    
    return heap;
}

Bounded_Min_Queue *bounded_min_queue_init(size_t n, const comp_ctx *comp_ctx, const destroy_ctx *destroy_key, const destroy_ctx *destroy_value) {
    return array_heap_init(n, comp_ctx, destroy_key, destroy_value);
}
void bounded_min_queue_destroy(Bounded_Min_Queue *q) {
    array_heap_destroy(q);
}

void bounded_min_queue_insert(Bounded_Min_Queue *q, void *key, void *value) {
    if (!bounded_min_queue_is_full(q)) {
        //if the heap is not yet full, the element can just be inserted
        array_heap_insert(q, key, value);
    }
    else {
        //the new element to insert is better than the current worst
        if (q->comp_ctx->comp(key, array_heap_peek_v(q), q->comp_ctx->ctx) < 0) {
            //remove the old head
            if (q->destroy_key_ctx) {
                q->destroy_key_ctx->destroy(q->data[0]->key, q->destroy_key_ctx->ctx);
            }
            if (q->destroy_value_ctx) {
                q->destroy_value_ctx->destroy(q->data[0]->value, q->destroy_value_ctx->ctx);
            }
            free(q->data[0]);

            //insert and bubble down to fix the heap
            q->data[0] = create_entry(key, value);
            bubble_down(q, 0);
        }
    }
}

KVPair *bounded_min_queue_pop_unordered(Bounded_Min_Queue *q) {
    return array_heap_pull(q);
}

KVPair **bounded_min_queue_getall_ordered(Bounded_Min_Queue *q) {
    KVPair **data = malloc(sizeof(*data) * q->cur_size);
    for (int i = q->cur_size - 1; i >= 0; i--) {
        data[i] = array_heap_pull(q);
    }
    bounded_min_queue_destroy(q);
    return data;
}

size_t bounded_min_queue_size(const Bounded_Min_Queue *q) {
    return q->cur_size;
}
bool bounded_min_queue_is_empty(const Bounded_Min_Queue *q) {
    return array_heap_is_empty(q);
}
bool bounded_min_queue_is_full(const Bounded_Min_Queue *q) {
    return array_heap_is_full(q);
}
