#include "swapping.h"

void swap(void *pos1, void *pos2, size_t item_size) {
    unsigned char *p = pos1, *q = pos2, temp;
    while (item_size--) {
        temp = *p;
        *p++ = *q;
        *q++ = temp;
    }
}
