#include "anagram_naive.h"
#include "../testing/test.h"

int main() {
    assert_true(anagram("orchestra", "carthorse"), NULL);
    assert_true(anagram("parliament", "partialmen"), NULL);
    assert_true(anagram("rearrangement", "greenermantra"), NULL);
    assert_true(anagram("quizsolution", "suntooilquiz"), NULL);
    assert_true(anagram("anagram", "gamanra"), NULL);
    assert_false(anagram("anagram", "waewfed"), NULL);
    assert_false(anagram("testt", "test"), NULL);
    assert_false(anagram("test", "testt"), NULL);
}
