#include "tree_heap.h"

typedef struct Tree_Node_ {
    void *key;
    void *value;
    struct Tree_Node_ *left;
    struct Tree_Node_ *right;
} Tree_Node;

struct Tree_Heap_ {
    int size;
    Tree_Node *root;
    const comp_ctx *comp_ctx;
    const destroy_ctx *destroy_key_ctx;
    const destroy_ctx *destroy_value_ctx;
};

static Tree_Node *init_node(void *key, void *element) {
    Tree_Node *n = malloc(sizeof(Tree_Node));
    n->left = n->right = NULL;
    n->key = key;
    n->value = element;

    return n;
}

static size_t tree_height(size_t size) { return (int)log2(size); }

static Stack *last_node(Tree_Node *cur, size_t size) {
    /*
     * Given the size of the tree, one can calculate the correct direction to go.
     * If the leaf level is more than halfway full, go right, otherwise go left.
     *
     * Tree with 9 nodes has binary 1001.  Each digit is half as large as the
     value preceding it.
     * Therefore, examine the second binary digit 1(0)01; it is 0, so the leaf
     level is less than half full.
     * Go left.  If it was 14, then 1(1)10, so go right.  At each step, half of
     the nodes are removed.
     * Therefore, examine the next digit. More details in the lecture.
     **/
    
    size_t height = !size ? 0 : tree_height(size);
    Stack *path = stack_init(height + 1, NULL);

    if (!size) {
        return path; //return empty stack if size == 0
    }

    for (int i = height - 1; i >= 0; i--) {
        stack_push(path, cur);
        cur = size & 1 << i ? cur->right : cur->left;
    }

    stack_push(path, cur);
    
    return path;
}

static void swap(Tree_Node *cur, Tree_Node *parent) {
    void *temp_key = cur->key;
    void *temp_value = cur->value;

    cur->key = parent->key;
    cur->value = parent->value;

    parent->key = temp_key;
    parent->value = temp_value;
}

static void bubble_up(Stack *path, const comp_ctx *comp_ctx) {
    Tree_Node *cur = stack_pop(path);

    while(!stack_is_empty(path)) {
        Tree_Node *parent = stack_pop(path);

        if (comp_ctx->comp(parent->key, cur->key, comp_ctx->ctx) < 0) {
            swap(cur, parent);
        }

        cur = parent;
    }
}

static void bubble_down(Tree_Heap *heap) {
    Tree_Node *cur = heap->root;

    while (cur) {
        Tree_Node *max = NULL;
        if (!cur->left) {
            max = cur->right;
        }
        else if (!cur->right) {
            max = cur->left;
        }
        else {
            max = heap->comp_ctx->comp(cur->left->key, cur->right->key, heap->comp_ctx->ctx) > 0 ? cur->left : cur->right;
        }

        if (max && heap->comp_ctx->comp(max->key, cur->key, heap->comp_ctx->ctx) > 0) {
            swap(max, cur);
            cur = max;
        }
        else {
            cur = NULL;
        }
    }
}

Tree_Heap *tree_heap_init(const comp_ctx *comp_ctx, const destroy_ctx *destroy_key, const destroy_ctx *destroy_value) {
    Tree_Heap *heap = malloc(sizeof(*heap));
    heap->size = 0;
    heap->root = NULL;
    heap->comp_ctx = comp_ctx;
    heap->destroy_key_ctx = destroy_key;
    heap->destroy_value_ctx = destroy_value;
    return heap;
}

void tree_heap_destroy(Tree_Heap *heap) {
    //traverse tree and free each node
    Stack *s = stack_init(heap->size, NULL);
    stack_push(s, heap->root);
    while (!stack_is_empty(s)) {
        Tree_Node *cur = stack_pop(s);
        if (!cur) {
            continue;
        }

        stack_push(s, cur->left);
        stack_push(s, cur->right);

        if (heap->destroy_key_ctx) {
            heap->destroy_key_ctx->destroy(cur->key, heap->destroy_key_ctx->ctx);
        }

        if (heap->destroy_value_ctx) {
            heap->destroy_value_ctx->destroy(cur->value, heap->destroy_value_ctx->ctx);
        }
        free(cur);
    }

    stack_destroy(s);
    free(heap);
}

void tree_heap_insert(Tree_Heap *heap, void *key, void *element) {
    Tree_Node *n = init_node(key, element);    
    Stack *path_to_insert = last_node(heap->root, ++heap->size);
    stack_pop(path_to_insert); //should always be null

    if (stack_is_empty(path_to_insert)) {
        heap->root = n;
    }
    else {
        Tree_Node *parent = stack_peek(path_to_insert);
        if (heap->size % 2 == 0) {
            parent->left = n;
        }
        else {
            parent->right = n;
        }
        stack_push(path_to_insert, n);
        bubble_up(path_to_insert, heap->comp_ctx);
    }

    stack_destroy(path_to_insert);
}

bool tree_heap_is_empty(const Tree_Heap *heap) {
    return heap->size == 0;
}

void *tree_heap_pull(Tree_Heap *heap) {
    if (tree_heap_is_empty(heap)) {
        return NULL;
    }

    void *max_value = heap->root->value;
    if (heap->destroy_key_ctx) {
        heap->destroy_key_ctx->destroy(heap->root->key, heap->destroy_key_ctx->ctx);
    }

    Stack *path_to_last = last_node(heap->root, heap->size);

    Tree_Node *last = stack_pop(path_to_last);
    heap->root->key = last->key;
    heap->root->value = last->value;
    free(last);

    //get rid of last node in tree
    Tree_Node *parent = stack_pop(path_to_last);
    if (!parent) {
        heap->root = NULL;
    }
    else {
        if (heap->size % 2 == 0) {
            //even means that the last node was left child
            //odd means that it was a right child
            parent->left = NULL;
        }
        else {
            parent->right = NULL;
        }
    }
    heap->size--;

    //fix root
    bubble_down(heap);

    stack_destroy(path_to_last);

    return max_value;
}

void *tree_heap_peek(const Tree_Heap *heap) {
    return tree_heap_is_empty(heap) ? NULL : heap->root->value;
}

size_t tree_heap_size(const Tree_Heap *heap) {
    return heap->size;
}
