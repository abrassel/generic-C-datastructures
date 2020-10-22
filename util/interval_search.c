#include "interval_search.h"

int interval_search(const void *arr, size_t n, size_t elm_size, const void *key, const comp_ctx *comp_ctx) {
    // array interval
    // 0 indicates left of array.  n indicates right of array
    int l_arr = 0, u_arr = n - 1;

    while (l_arr <= u_arr) {
        size_t midpoint = (l_arr + u_arr) / 2;
        void *elm = (char *) arr + midpoint * elm_size;
        int comp_res = comp_ctx->comp(key, elm, comp_ctx->ctx);

        if (comp_res == 0) {
            // exact match
            // neg indicates an exact match
            // disambiguate zero
            return (midpoint == 0) ? INT_MIN : -(int)midpoint;
        }

        if (comp_res < 0) {
            // key is smaller than midpoint, pick left
            // don't need midpoint since it has already been examined
            u_arr = midpoint - 1;
        }
        else {
            //key is larger than midpoint, pick right
            l_arr = midpoint + 1;
        }
    }

    /**
     * since there is no exact match, an overstep is guaranteed
     *
     * If left bound overstepped, then left - 1 is correct lower bound index
     *
     * If right bound understepped, then left is at the correct location
     *
     * No matter what, result is indexed relative to the spaces between the array elements, so 1 must be added to the result.
     */
    if (l_arr == (int) n) {
        return n;
    }
    if (comp_ctx->comp(key, (char *) arr + l_arr * elm_size, comp_ctx->ctx) < 0) {
        return l_arr;
    }
    else {
        return l_arr + 1;
    }
}
