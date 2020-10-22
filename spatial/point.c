#include "point.h"
#include <float.h>

bool eq_p(size_t k, const double *ds1, const double *ds2) {
    for (size_t i = 0; i < k; i++) {
        if (!eq_d(ds1[i], ds2[i])) {
            return false;
        }
    }

    return true;
}

double squared_distance(const double *p1, const double *p2, size_t len) {
    double total = 0;
    for (size_t i = 0; i < len; i++) {
        double diff = p2[i] - p1[i];
        total += diff * diff;
    }

    return total;
}

direction2D quadrant(const Point2D center, const Point2D point) {
    bool right = point[X] - center[X] > 0;
    bool above = point[Y] - center[Y] > 0;

    return right ? above ? NE : SE : above ? NW : SW;
}

void infinite_rectangle(Rectangle rect) {
    rect[SW][X] = rect[SW][Y] = -DBL_MAX;
    rect[NE][X] = rect[NE][Y] = DBL_MAX;
}

void get_center(Rectangle rect, Point2D center) {
    center[X] = (rect[SW][X] + rect[NE][X]) / 2;
    center[Y] = (rect[SW][Y] + rect[NE][Y]) / 2;
}

void copy_rectangle(Rectangle r1, Rectangle r2) {
    memcpy(r1, r2, sizeof(*r2) * 2);
}

void build_rectangle(Rectangle r1, double swx, double swy, double nex, double ney) {
    r1[SW][X] = swx;
    r1[SW][Y] = swy;
    r1[NE][X] = nex;
    r1[NE][Y] = ney;
}

void split_rectangle(Rectangle rect, Rectangle rects[4]) {
    Point2D center;
    get_center(rect, center);

    /*
     * ---------
     * |NW |NE |
     * ---------
     * |SW |SE |
     * ---------
     */
    //rects are in SW, NE, SE, NW order
    build_rectangle(rects[SW], rect[SW][X], rect[SW][Y], center[X], center[Y]);
    build_rectangle(rects[NE], center[X], center[Y], rect[NE][X], rect[NE][Y]);
    build_rectangle(rects[SE], center[X], rect[SW][Y], rect[NE][X], center[Y]);
    build_rectangle(rects[NW], rect[SW][Y], center[X], center[Y], rect[NE][X]);
}

bool eq_p_casted(const void *ds1, const void *ds2, const void *ctx) {
    struct array_eq *arr_ctx = (struct array_eq *) ctx;

    return eq_p(arr_ctx->nmemb, (const double *) ds1, (const double *) ds2);
}

struct array_eq point2d_dims = {
    2, 0, NULL
};

eq_ctx point2d_eq_ctx = {
    eq_p_casted,
    &point2d_dims
};

