#include "anagram_naive.h"

static void swap_to_end(char *str, size_t i, size_t len) {
    char tmp = str[i];
    str[i] = str[len-1];
    str[len-1] = tmp;
}

bool anagram(const char *w1, const char *w2) {
    size_t w1_len = strlen(w1);
    size_t w2_len = strlen(w2);

    //null terminating byte
    char *w2_copy = malloc(sizeof(char) * (w2_len+1));
    strncpy(w2_copy, w2, w2_len + 1);

    for (size_t i = 0; i < w1_len; i++) {
        bool match_found = false;
        for (size_t j = 0; j < w2_len; j++) {
            if (w1[i] == w2_copy[j]) {
                match_found = true;
                swap_to_end(w2_copy, j, w2_len--);
                break;
            }
        }

        if (!match_found) {
            free(w2_copy);
            return false;
        }
    }

    free(w2_copy);
    return w2_len == 0;
}
