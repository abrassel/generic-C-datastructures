#include "adjacency_matrix.h"

struct Adjacency_Matrix_ {
    double **weights;
    size_t n;
    bool is_directed;
};

Adjacency_Matrix *adjacency_matrix_init(size_t n, bool is_directed) {
    Adjacency_Matrix *m = malloc(sizeof(*m));

    m->n = n;
    m->weights = malloc(sizeof(double *) * n);
    for (size_t i = 0; i < n; i++) {
        //weights need to be initially zero
        m->weights[i] = calloc(n, sizeof(double));
    }
    m->is_directed = is_directed;
    return m;
}
void adjacency_matrix_destroy(Adjacency_Matrix *m) {
    for (size_t i = 0; i < m->n; i++) {
        free(m->weights[i]);
    }
    free(m->weights);
    free(m);
}

bool adjacency_matrix_adjacent(const Adjacency_Matrix *graph, size_t v1, size_t v2) {
    if (v1 >= graph->n || v2 >= graph->n) {
        return false;
    }

    return !eq_d(graph->weights[v1][v2], 0.0);
}
double *adjacency_matrix_neighbors(const Adjacency_Matrix *graph, size_t label) {
    if (label >= graph->n) {
        return NULL;
    }

    return graph->weights[label];
}

//can only add up to graph->n (inclusive)
void adjacency_matrix_add_vertex(Adjacency_Matrix *graph, size_t label) {
    //particularly expensive operation
    if (label > graph->n) {
        return;
    }

    //add new row
    graph->weights = realloc(graph->weights, (graph->n + 1) * sizeof(*graph->weights));
    //shift elements as necessary
    memmove(graph->weights + label + 1, graph->weights + label, (graph->n - label) * sizeof(*graph->weights));
    //initialize new row
    graph->weights[label] = calloc((graph->n + 1), sizeof(double));
    //add 1 column to all of the old rows
    //and shift elements as necessary
    for (size_t i = 0; i < graph->n + 1; i++) {
        graph->weights[i] = realloc(graph->weights[i], (graph->n + 1) * sizeof(*graph->weights[i]));
        memmove(graph->weights[i] + label + 1, graph->weights[i] + label, (graph->n - label) * sizeof(*graph->weights[i]));
        graph->weights[i][label] = 0;
    }
    graph->n++;
}
void adjacency_matrix_remove_vertex(Adjacency_Matrix *graph, size_t label) {
    if (label >= graph->n) {
        return;
    }

    //get rid of old row
    free(graph->weights[label]);

    //move all of the other rows up
    memmove(graph->weights + label, graph->weights + label + 1, (graph->n - label - 1) * sizeof(*graph->weights));

    //shrink the total allocation
    graph->weights = realloc(graph->weights, (graph->n - 1) * sizeof(*graph->weights));

    //iterate through the rows and repeat the shifting process
    for (size_t i = 0; i < graph->n-1; i++) {
        //no freeing necessary
        memmove(graph->weights[i] + label, graph->weights[i] + label + 1, (graph->n - label - 1) * sizeof(*graph->weights[i]));

        /* //shrink the row allocation */
        graph->weights[i] = realloc(graph->weights[i], (graph->n - 1) * sizeof(*graph->weights));
    }

    graph->n--;
}

bool adjacency_matrix_add_edge(Adjacency_Matrix *graph, size_t x, size_t y, double weight) {
    if (x >= graph->n || y >= graph->n) {
        return false;
    }

    adjacency_matrix_set_edge_value(graph, x, y, weight);
    return true;
}

bool adjacency_matrix_remove_edge(Adjacency_Matrix *graph, size_t x, size_t y) {
    if (x >= graph->n || y >= graph->n ) {
        return false;
    }

    graph->weights[x][y] = 0;
    if (!graph->is_directed) {
        graph->weights[y][x] = 0;
    }

    return true;
}

double adjacency_matrix_get_edge_value(const Adjacency_Matrix *graph, size_t x, size_t y) {
    if (x >= graph->n || y >= graph->n) {
        return 0;
    }

    return graph->weights[x][y];
}

void adjacency_matrix_set_edge_value(Adjacency_Matrix *graph, size_t x, size_t y, double weight) {
    if (x >= graph->n || y >= graph->n) {
        return;
    }

    graph->weights[x][y] = weight;

    if (!graph->is_directed) {
        graph->weights[y][x] = weight;
    }
}
