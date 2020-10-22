#include "pr_quadtree.h"
#include "point.h"

typedef struct Node_ {
    //null = WHITE
    union {
        struct {
            Point2D points[4];
            size_t cur_size;
        } element_record;
        struct Node_ *children[4];
        
    } *contents;
    enum {BLACK, WHITE, GRAY} node_kind;
} Node;

struct PRQuadtree_ {
    Node *root;
    size_t size, max_size;
    Rectangle boundaries;
};

static Node *find_loc(Node *cur, Rectangle rect, const Point2D point) {
    Point2D center;
    Rectangle rectangles[4];
    
    while (cur->node_kind == GRAY) {
        get_center(rect, center);
        direction2D cur_quadrant = quadrant(center, point);
        
        //split the quadrant and move to the correct one for recursion
        split_rectangle(rect, rectangles);
        copy_rectangle(rect, rectangles[cur_quadrant]);
        cur = cur->contents->children[cur_quadrant];
    }

    return cur;
}

PRQuadtree *prquadtree_init(Rectangle boundary) {
    PRQuadtree *tree = malloc(sizeof(*tree));

    tree->root = malloc(sizeof(*tree->root));
    tree->root->node_kind = WHITE;
    tree->size = tree->max_size = 0;
    copy_rectangle(tree->boundaries, boundary);
    
    return tree;
}

void prquadtree_destroy(PRQuadtree *tree) {
    Stack *s = stack_init(tree->max_size + 1, NULL);

    stack_push(s, tree->root);

    while (!stack_is_empty(s)) {
        Node *cur = stack_pop(s);

        //fallthroughs on purpose
        switch (cur->node_kind) {
        case GRAY:
            for (int i = 0; i < 4; i++) {
                stack_push(s, cur->contents->children[i]);
            }
            __attribute__ ((fallthrough));
        case BLACK:
            free(cur->contents);
            __attribute__ ((fallthrough));
        case WHITE:
            free(cur);
        }
    }

    free(tree);
    stack_destroy(s);
}

static void prquadtree_add_r(Node *subtree, Rectangle rect, Point2D point) {
    Node *cur = find_loc(subtree, rect, point);
    
    switch (cur->node_kind) {
    case WHITE:
        cur->node_kind = BLACK;
        cur->contents = malloc(sizeof(*cur->contents));
        cur->contents->element_record.cur_size = 0;
        memcpy(
            cur->contents->element_record.points[cur->contents->element_record.cur_size++],
            point,
            2 * sizeof(*point)
            );
        
        break;
    case BLACK: ;
        size_t cur_size = cur->contents->element_record.cur_size;
        //need to decide if splitting or not
        if (cur_size == 4) {
            cur->node_kind = GRAY;
            Point2D points[4];
            memcpy(points, cur->contents->element_record.points, 4 * sizeof(*points));
            for (size_t i = 0; i < 4; i++) {
                cur->contents->children[i] = malloc(sizeof(Node));
                cur->contents->children[i]->node_kind = WHITE;
            }
            for (size_t i = 0; i < 4; i++) {
                Rectangle child_copy;
                copy_rectangle(child_copy, rect);
                prquadtree_add_r(cur, child_copy, points[i]);
            }
            prquadtree_add_r(cur, rect, point);
        }
        else {
            memcpy(cur->contents->element_record.points[cur_size], point, 2 * sizeof(*point));
            cur->contents->element_record.cur_size++;
        }
        break;
    default:
        //impossible case
        return;
    }
}

void prquadtree_add(PRQuadtree *tree, Point2D point) {
    Rectangle rect;
    copy_rectangle(rect, tree->boundaries);
    prquadtree_add_r(tree->root, rect, point);
    tree->size ++;
    if (tree->size > tree->max_size) {
        tree->max_size = tree->size;
    }
}

bool prquadtree_search(const PRQuadtree *tree, const Point2D point) {
    Rectangle rect;
    copy_rectangle(rect, (double(*)[2]) tree->boundaries);
    Node *cur = find_loc(tree->root, rect, point);
    return cur->node_kind == WHITE ?
        false :
        array_contains(
            cur->contents->element_record.points,
            cur->contents->element_record.cur_size,
            sizeof(Point2D),
            point,
            &point2d_eq_ctx
            );
}

void prquadtree_remove(PRQuadtree *tree, const Point2D point) {
    Rectangle rect;
    copy_rectangle(rect, tree->boundaries);
    Node *cur = find_loc(tree->root, rect, point);
    if (cur->node_kind == BLACK) {
        int index = array_ind(
            cur->contents->element_record.points,
            cur->contents->element_record.cur_size,
            sizeof(Point2D),
            point,
            &point2d_eq_ctx
            );

        if (index == -1) {
            return;
        }

        if (cur->contents->element_record.cur_size == 1) {
            free(cur->contents);
            cur->node_kind = WHITE;
        }
        else {
            /* shift all elements left one to override */
            memmove(cur->contents->element_record.points + index,
                    cur->contents->element_record.points + index + 1,
                    (cur->contents->element_record.cur_size-- - index - 1) * sizeof(Point2D)
                );
        }
        tree->size--;
    }
}

//nn
    // knn

    // range query

size_t prquadtree_size(const PRQuadtree *tree) {
    return tree->size;
}

bool prquadtree_is_empty(const PRQuadtree *tree) { return tree->size == 0; }

size_t node_height(const Node *n) {
    int max_child_height = -1;
    switch (n->node_kind) {
    case WHITE:
        return -1;
    case BLACK:
        return 0;
    case GRAY:
        for (int i = 0; i < 4; i++) {
            int child_height = node_height(n->contents->children[i]);
            if (child_height > max_child_height) {
                max_child_height = child_height;
            }
        }
    }

    return 1 + max_child_height;
}

size_t prquadtree_height(const PRQuadtree *tree) {
    return node_height(tree->root);
}
