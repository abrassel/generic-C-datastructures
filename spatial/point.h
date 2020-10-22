#ifndef POINT_H
#define POINT_H

#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include <string.h>

#include "../util/array_lookup.h"
#include "../util/double_comp.h"

typedef enum {X, Y, Z} dims;

typedef double Point2D[2];
typedef Point2D Rectangle[2];

typedef enum { SW, NE, SE, NW } direction2D;

bool eq_p(size_t k, const double *ds1, const double *ds2);
bool eq_p_casted(const void *ds1, const void *ds2, const void *ctx);

double squared_distance(const double *p1, const double *p2, size_t len);

direction2D quadrant(const Point2D center, const Point2D point);

void infinite_rectangle(Rectangle rect);
void get_center(Rectangle rect, Point2D center);
void split_rectangle(Rectangle rect, Rectangle rects[4]);

void copy_rectangle(Rectangle r1, Rectangle r2);

void build_rectangle(Rectangle r1, double swx, double swy, double nex,
                     double ney);

extern eq_ctx point2d_eq_ctx;
#endif
