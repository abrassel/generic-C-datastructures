#include "kdtree.h"

typedef struct Node_ {
    double *point;
    void *value;
    struct Node_ *l, *r;
} Node;

struct KDTree_ {
    size_t size;
    size_t k;
    Node *root;
    const destroy_ctx *destroy_key_ctx;
    const destroy_ctx *destroy_value_ctx;
};

static Node *create_node(double *point, void *value) {
    Node *n = malloc(sizeof(*n));
    n->point = point;
    n->value = value;
    n->l = n->r = NULL;
    return n;
}

KDTree *kdtree_init(size_t k, const destroy_ctx *destroy_key_ctx, const destroy_ctx *destroy_value_ctx) {
    KDTree *tree = malloc(sizeof(*tree));
    tree->size = 0;
    tree->k = k;
    tree->root = NULL;
    tree->destroy_key_ctx = destroy_key_ctx;
    tree->destroy_value_ctx = destroy_value_ctx;
    return tree;
}

void kdtree_destroy(KDTree *tree) {
    Stack *s = stack_init(tree->size, NULL);
    stack_push(s, tree->root);
    while (!stack_is_empty(s)) {
        Node *cur = stack_pop(s);
        if (!cur) {
            continue;
        }

        if (tree->destroy_key_ctx) {
            tree->destroy_key_ctx->destroy(cur->point, tree->destroy_key_ctx->ctx);
        }
        if (tree->destroy_value_ctx) {
            tree->destroy_value_ctx->destroy(cur->value, tree->destroy_value_ctx->ctx);
        }

        stack_push(s, cur->l);
        stack_push(s, cur->r);
        free(cur);
    }
    stack_destroy(s);
    free(tree);
}

typedef enum {LEFT, RIGHT} direction;

static Node *kdtree_int_search(const KDTree *tree, const double *point, Node **parent_ptr, direction *dir) {
    Node *cur = tree->root;
    size_t dim = 0;
    if (parent_ptr) {
        *parent_ptr = NULL;
    }

    while (cur) {
        //small interval for equality prevents accidentally going the wrong way
        int comp = comp_d(point[dim], cur->point[dim]);
        if (comp == 0) {
            if (eq_p(tree->k, point, cur->point)) {
                return cur;
            }
        }
        else {
            if (parent_ptr) {
                *parent_ptr = cur;
            }
            if (comp > 0) {
                if (dir) {
                    *dir = RIGHT;
                }
                cur = cur->r;
            }
            else {
                if (dir) {
                    *dir = LEFT;
                }
                cur = cur->l;
            }
        }

        //cycle through dimensions
        dim = (dim + 1) % tree->k;
    }

    return NULL;
}

void *kdtree_search(const KDTree *tree, const double *point) {
    Node *child = kdtree_int_search(tree, point, NULL, NULL);
    return child ? child->value : NULL;
}

void kdtree_insert(KDTree *tree, double *point, void *val) {
    Node *parent;
    direction dir;
    Node *leaf = kdtree_int_search(tree, point, &parent, &dir);

    if (leaf) {
        //already exists
        leaf->value = val;
        return;
    }

    Node *new_node = create_node(point, val);
    tree->size++;

    if (!parent) {
        //this happens when the tree is empty
        tree->root = new_node;
        return;
    }

    //if none of these, insert at appropriate location for child
    switch (dir) {
    case LEFT:
        parent->l = new_node;
        break;
    case RIGHT:
        parent->r = new_node;
        break;
    }
}

struct dim_tracker {
    size_t dim;
    Node *n, *prev;
    direction prev_dir;
};

static struct dim_tracker *create_dim_tracker(Node *n, Node *prev, int dim, direction prev_dir) {
    struct dim_tracker *d = malloc(sizeof(*d));
    d->dim = dim;
    d->n = n;
    d->prev = prev;
    d->prev_dir = prev_dir;
    return d;
}

static Node *find_min(Node *n, size_t dim, size_t size, size_t k, Node **parent, direction *dir, size_t *min_dim) {
    Node *min_node = n;
    *min_dim = (dim + 1) % k;

    Stack *s = stack_init(size, NULL);
    stack_push(s, create_dim_tracker(n, *parent, (dim + 1) %k, *dir));

    while (!stack_is_empty(s)) {
        struct dim_tracker *cur = stack_pop(s);
        if (cur->n->point[dim] < min_node->point[dim]) {
            *parent = cur->prev;
            *dir = cur->prev_dir;
            *min_dim = cur->dim;
            min_node = cur->n;
        }

        if (cur->n->l) {
            stack_push(s, create_dim_tracker(cur->n->l, cur->n, (cur->dim + 1) % k, LEFT));
        }
        if (cur->n->r && cur->dim != dim) {
            //small optimization - don't need to explore for min/max in wrong side of splitter
            stack_push(s, create_dim_tracker(cur->n->r, cur->n, (cur-> dim + 1) % k, RIGHT));
        }
        free(cur);
    }

    stack_destroy(s);
    return min_node;
}

static Node *find_max(Node *n, size_t dim, size_t size, size_t k, Node **parent, direction *dir, size_t *max_dim) {
    Node *max_node = n;
    *max_dim = (dim + 1) % k;

    Stack *s = stack_init(size, NULL);
    stack_push(s, create_dim_tracker(n, *parent, (dim + 1) %k, *dir));

    while (!stack_is_empty(s)) {
        struct dim_tracker *cur = stack_pop(s);
        if (cur->n->point[dim] > max_node->point[dim]) {
            *parent = cur->prev;
            *dir = cur->prev_dir;
            *max_dim = cur->dim;
            max_node = cur->n;
        }

        if (cur->n->r) {
            stack_push(s, create_dim_tracker(cur->n->r, cur->n, (cur->dim + 1) % k, LEFT));
        }
        if (cur->n->l && cur->dim != dim) {
            //small optimization - don't need to explore for min/max in wrong side of splitter
            stack_push(s, create_dim_tracker(cur->n->l, cur->n, (cur-> dim + 1) % k, RIGHT));
        }
        free(cur);
    }

    stack_destroy(s);
    return max_node;
}

void *kdtree_remove(KDTree *tree, const double *point) {
    //find subtree root
    Node *parent;
    direction dir;
    Node *cur = kdtree_int_search(tree, point, &parent, &dir);
    if (tree->destroy_key_ctx) {
        tree->destroy_key_ctx->destroy(cur->point, tree->destroy_key_ctx->ctx);
    }
    void *orig_val = cur->value;

    //move down the tree with each find-min / find-max
    size_t dim = 0;
    while (cur) {
        //classify between right tree, left tree, and leaf deletion
        if (cur->r || cur->l) {
            Node *new_cur;
            parent = cur;
            if (cur->r) {
                dir = RIGHT;
                new_cur = find_min(cur->r, dim, tree->size, tree->k, &parent, &dir, &dim);
            }
            else {
                dir = LEFT;
                new_cur = find_max(cur->l, dim, tree->size, tree->k, &parent, &dir, &dim);
            }
            cur->point = new_cur->point;
            cur->value = new_cur->value;
            cur = new_cur;
        }
        else {
            //leaves must be destroyed explicitly
            tree->size--;
            free(cur);

            //in leaf case, check for singleton
            if (!parent) {
                //must clear root
                tree->root = NULL;
            }
            else {
                switch (dir) {
                case LEFT:
                    parent->l = NULL;
                    break;
                case RIGHT:
                    parent->r = NULL;
                    break;
                }
            }
            return orig_val;
        }
    }

    return NULL;
}

bool kdtree_is_empty(const KDTree *tree) {
    return tree->size == 0;
}
size_t kdtree_size(const KDTree *tree) { return tree->size; }

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

struct KD_Point *kdtree_nn(const KDTree *tree, const double *anchor) {
    double distance = DBL_MAX;
    Node *closest = NULL;

    if (kdtree_is_empty(tree)) {
        return NULL;
    }

    Stack *s = stack_init(tree->size, NULL);
    stack_push(s, create_dim_tracker(tree->root, NULL, 0, LEFT));

    while(!stack_is_empty(s)) {
        struct dim_tracker *cur = stack_pop(s);

        //check for new best
        double new_distance = squared_distance(anchor, cur->n->point, tree->k);
        if (new_distance < distance) {
            //update distance
            distance = new_distance;
            closest = cur->n;
        }

        //test splitter dim for pruning (note this is not squared)
        double splitter_distance = cur->n->point[cur->dim] - anchor[cur->dim];

        bool prune = splitter_distance * splitter_distance > distance;
        //find far side
        direction far_side = splitter_distance < 0 ? LEFT : RIGHT;

        //do not use far side if pruning
        if (cur->n->l && !(prune && far_side == LEFT)) {
            stack_push(s, create_dim_tracker(cur->n->l, NULL, (cur->dim + 1) % tree->k, LEFT));
        }

        if (cur->n->r && !(prune && far_side == RIGHT)) {
            stack_push(s, create_dim_tracker(cur->n->r, NULL, (cur->dim + 1) % tree->k, RIGHT));
        }
        free(cur);
    }

    stack_destroy(s);
    return (struct KD_Point *) closest;
}

void kdtree_print(const KDTree *tree, void (*print_value)(const void *val)) {
    if(kdtree_is_empty(tree)) {
        printf("This tree is empty");
        return;
    }

    Stack *s = stack_init(tree->size, &naive_destroy_ctx);

    size_t count = 0;
    stack_push(s, create_child(count, tree->root));

    while (!stack_is_empty(s)) {
        struct child *cur = stack_pop(s);
        printf("Id: %d\n--------------------\nKey: ", cur->id);
        print_double_array(cur->child->point, tree->k);
        printf(", Value: ");
        print_value(cur->child->value);
        printf("\nChildren: [");
        if (!cur->child->l) {
            printf("NULL, ");
        }
        else {
            printf("%lu, ", ++count);
            stack_push(s, create_child(count, cur->child->l));
        }

        if (!cur->child->r) {
            printf("NULL, ");
        }
        else {
            printf("%lu, ", ++count);
            stack_push(s, create_child(count, cur->child->r));
        }

        printf("]\n\n");
        free(cur);
    }


    stack_destroy(s);
}
