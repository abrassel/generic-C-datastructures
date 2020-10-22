#ifndef DOUBLE_COMP_H
#define DOUBLE_COMP_H

#include <math.h>
#include <stdbool.h>

#define eps .00001

static inline bool eq_d(double d1, double d2) {
    return fabs(d1 - d2) < eps;
}
static inline int comp_d(double d1, double d2) {
    return eq_d(d1, d2) ? 0 : d1 - d2;
}

#endif
