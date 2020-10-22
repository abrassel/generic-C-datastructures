#include "heap_sort.h"

#include <stdio.h>

void heap_sort(void *arr, size_t nmemb, size_t item_size, const comp_ctx *comp_ctx) {
    void *buf = malloc(nmemb * item_size);
    memcpy(buf, arr, nmemb * item_size);
    Array_Heap *heap = array_heap_heapify(buf, nmemb, item_size, comp_ctx);
    
    for (int i = nmemb - 1; i >= 0; i--) {
        void *res = array_heap_pull_v(heap);
        memcpy((char *) arr + i * item_size, res, item_size);
    }
    
    array_heap_destroy(heap);
    free(buf);
}
