#include "../testing/test.h"
#include "stack.h"

int main() {
    Stack *s = stack_init(3, NULL);
    int zero = 0;
    int three = 3;
    int five = 5;

    assert_true(stack_is_empty(s), NULL);
    assert_false(stack_is_full(s), NULL);

    assert_true(stack_push(s, &zero), NULL);
    assert_true(stack_push(s, &three), NULL);
    assert_true(stack_push(s, &five), NULL);

    assert_equal(stack_peek(s), &five, NULL, NULL);
    assert_equal(stack_pop(s), &five, NULL, NULL);

    assert_equal(stack_pop(s), &three, NULL, NULL);
    assert_true(stack_pop(s), NULL);

    assert_true(stack_push(s, &zero), NULL);
    
    stack_destroy(s);
}
