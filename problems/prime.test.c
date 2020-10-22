#include "../testing/test.h"
#include "prime.h"

static void test_primes() {
    assert_true(is_prime(3), NULL);
    assert_true(is_prime(23), NULL);
    assert_true(is_prime(2), NULL);
    assert_false(is_prime(4), NULL);
    assert_false(is_prime(8), NULL);
    assert_false(is_prime(25), NULL);
    assert_false(is_prime(21), NULL);
}

static void test_next_prime() {
    assert_equal_int(5, next_prime(4), NULL);
    assert_equal_int(31, next_prime(30), NULL);
    assert_equal_int(97, next_prime(90), NULL);
}


int main() {
    test_primes();
    test_next_prime();
}

