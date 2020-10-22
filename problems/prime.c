#include "prime.h"

bool is_prime(size_t x) {
    for (size_t i = 2; i * i <= x; i++) {
        if (x % i == 0) {
            return false;
        }
    }

    return true;
}

size_t next_prime(size_t x) {
    if (x % 2 == 0) {
        x++;
    }

    while (!is_prime(x)) {
        x += 2;
    }

    return x;
}
