#include "btree.h"

typedef struct Node_ {
    KVPair *keys;
    size_t cur_size;
    struct Node_ **children;
} Node;

struct Btree_ {
    size_t size;
    size_t p;
    comp_ctx comp_ctx;
    const destroy_ctx *destroy_key;
    const destroy_ctx *destroy_value;
    Node *root;
};

/* insertion search and deletion **/
struct path_down {
    Node *parent;
    size_t interval;
};

static struct path_down *create_path_down(Node *parent, size_t interval) {
    struct path_down *p = malloc(sizeof(*p));
    p->interval = interval;
    p->parent = parent;

    return p;
}

static int comp_key_to_pair(const void *key, const void *pair, const void *ctx) {
    const comp_ctx *key_comp = (const comp_ctx *) ctx;
    return key_comp->comp(key, ((const KVPair *) pair)->key, key_comp->ctx);
}

static void shift_entries_right(Node *cur, size_t interval) {
    memmove(cur->keys + interval + 1, cur->keys + interval,
            (cur->cur_size - interval) * sizeof(KVPair));
    memmove(cur->children + interval + 1, cur->children + interval,
            (cur->cur_size - interval) * sizeof(KVPair));
}

static size_t height(Btree *tree) {
    return btree_is_empty(tree) ? 0 : (size_t) ceil(log2(tree->size) / log2(tree->p));
}

static Node *create_node(size_t p) {
    Node *n = malloc(sizeof(*n));
    if (!n) {
        return NULL;
    }

    n->children = calloc(p+1, sizeof(*n->children));
    n->cur_size = 0;
    n->keys = malloc(sizeof(*n->keys) * p); // 1 extra space for overflow

    return n;
}

static void rebalance_btree(Btree *tree, Stack *s) {
    // after insertion is complete, walk up stack, rebalancing as necessary
    while (!stack_is_empty(s)) {
        struct path_down *cur = stack_pop(s);
        if (cur->parent->cur_size == tree->p) { // overfull
            // try to grab parent for shifting
            struct path_down *actual_parent = stack_peek(s);

            // if there is no parent, cur is root, must grow
            // fake a parent for the rebalancing procedure
            bool no_parent = actual_parent == NULL;
            if (no_parent) {
                Node *new_root = create_node(tree->p);
                tree->root = new_root;
                actual_parent = create_path_down(new_root, 0);

                //new fake node so needs a pointer to root
                new_root->children[0] = cur->parent;
            }

            //the parent must make space for the median element.
            //because it is coming from the child, the median element will be inserted between the seperators in the parent for the child
            //the path saves the seperators on the way down for reuse here
            //the children need to be moved as well in the parent
            //care must be taken to not move the children out of bounds.  extra space in children array for this purpose
            shift_entries_right(actual_parent->parent, actual_parent->interval);

            Node *new_node = create_node(tree->p);
            //new node wil go in the newly created spot.  no key (yet)
            actual_parent->parent->children[actual_parent->interval + 1] = new_node;

            //this method moves a portion of the keys to the new node (which becomes the right node)
            //the key is inserted directly into the parent in the new empty spot post shift
            size_t split_ind = split_sorted_array_on_median(
              cur->parent->keys, cur->parent->cur_size,
              sizeof(KVPair), new_node->keys,
              actual_parent->parent->keys + actual_parent->interval);

            //maintain size invariants
            actual_parent->parent->cur_size++;
            cur->parent->cur_size = split_ind;
            new_node->cur_size = (tree->p) - (split_ind + 1);

            //when the keys are split, the children (if they exist) must be moved too
            //left gets [0, split], right gets (split, p+1)
            memcpy(new_node->children, cur->parent->children + split_ind + 1, (new_node->cur_size + 1) * sizeof(Node *));
            //zero out the old children
            memset(cur->parent->children + split_ind + 1, 0, (new_node->cur_size + 1) * sizeof(Node *));

            if (no_parent) {
                //made a fake parent so it must be freed since it will never become cur.
                free(actual_parent);
            }
        }
        free(cur);
    }
}

/* setup and teardown **/

Btree *btree_init(size_t p, const comp_ctx *comp, const destroy_ctx *destroy_key, const destroy_ctx *destroy_value) {
    Btree *tree = malloc(sizeof(*tree));

    tree->size = 0;
    tree->comp_ctx.comp = comp_key_to_pair;
    tree->comp_ctx.ctx = comp;
    tree->destroy_key = destroy_key;
    tree->destroy_value = destroy_value;
    tree->p = p;
    tree->root = NULL;

    return tree;
}

void btree_destroy(Btree *tree) {
    Stack *s = stack_init(tree->size, NULL);

    if (!btree_is_empty(tree)) {
        stack_push(s, tree->root);
        while(!stack_is_empty(s)) {
            Node *n = stack_pop(s);

            for (size_t i = 0; i < n->cur_size; i++) {
                if (tree->destroy_key) {
                    tree->destroy_key->destroy(n->keys[i].key, tree->destroy_key->ctx);
                }
                if (tree->destroy_value) {
                    tree->destroy_value->destroy(n->keys[i].value, tree->destroy_value->ctx);
                }

                if (n->children[i]) {
                    stack_push(s, n->children[i]);
                }
            }
            if (n->children[n->cur_size]) {
                stack_push(s, n->children[n->cur_size]);
            }
            free(n->keys);
            free(n->children);
            free(n);
        }
    }

    stack_destroy(s);
    free(tree);
}

void btree_insert(Btree *tree, void *key, void *value) {
    // at each step, look for the correct interval
    // if a match is found, stop.  No duplicates allowed.
    // Add results onto a stack as they are encountered.
    // When null is encountered, go a level up to leaf, add there.
    // recurse up the stack until rebalancing is no longer necessary.
    if (btree_is_empty(tree)) {
        tree->root = create_node(tree->p);
    }

    Stack *s = stack_init(height(tree) + 1, &naive_destroy_ctx);

    Node *cur = tree->root;
    while (cur) {
        int interval = interval_search(cur->keys, cur->cur_size, sizeof(KVPair), key,
                                       &tree->comp_ctx);
        stack_push(s, create_path_down(cur, interval));
        if (interval < 0) {
            return; // match found
        }

        cur = cur->children[interval];
    }

    // top of stack contains leaf level
    // tree is not empty at this point
    // interval contains where the key should be inserted
    struct path_down *p = stack_peek(s);
    Node *leaf = p->parent;
    size_t interval = p->interval;
    shift_entries_right(leaf, interval);
    leaf->keys[interval].key = key;
    leaf->keys[interval].value = value;
    leaf->cur_size++;
    tree->size++;

    rebalance_btree(tree, s);
    stack_destroy(s);
}

void *btree_search(const Btree *tree, const void *key) {
    // at each step, find the interval.
    // if a match is found, return value
    // if the end of the tree is met, return NULL

    Node *cur = tree->root;

    while (cur) {
        int interval =
            interval_search(cur->keys, cur->cur_size, sizeof(KVPair), key, &tree->comp_ctx);
        if (interval < 0) {
            if (interval == INT_MIN) {
                interval = 0;
            }
            return cur->keys[-interval].value;
        }

        cur = cur->children[interval];
    }

    return NULL;
}

int btree_size(const Btree *tree) { return tree->size; }
bool btree_is_empty(const Btree *tree) { return tree->size == 0; }


struct child {
    int id;
    Node *child;
};

static struct child *create_child(int id, Node *child) {
    struct child *c = malloc(sizeof(*c));
    c->id = id;
    c->child = child;

    return c;
}

void btree_print(const Btree *tree, void (*print_key)(const void *key)) {
    if (btree_is_empty(tree)) {
        printf("This tree is empty");
        return;
    }

    Stack *s = stack_init(tree->size, &naive_destroy_ctx);

    size_t count = 0;
    stack_push(s, create_child(count, tree->root));

    while (!stack_is_empty(s)) {
        struct child *cur = stack_pop(s);
        printf("Id: %d\n--------------------\nKeys: [", cur->id);
        for (size_t i = 0; i < cur->child->cur_size; i++) {
            print_key(cur->child->keys[i].key);
            printf(", ");
        }
        printf("]\nChildren: [");
        for (size_t i = 0; i <= tree->p; i++) {
            if (!cur->child->children[i]) {
                printf("NULL, ");
            }
            else {
                printf("%lu, ", ++count);
                stack_push(s, create_child(count, cur->child->children[i]));
            }
        }
        printf("]\n\n");
        free(cur);
    }


    stack_destroy(s);
}
