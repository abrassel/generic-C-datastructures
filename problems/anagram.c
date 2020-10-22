#include "anagram.h"

bool anagram(const char *w1, const char *w2) {
    int counts[NUM_LETTERS] = {0};

    for (size_t i = 0; i < strlen(w1); i++) {
        counts[w1[i] - 'a']++;
    }

    for (size_t j = 0; j < strlen(w2); j++) {
        counts[w2[j] - 'a']--;
    }

    for (size_t k = 0; k < NUM_LETTERS; k++) {
        if (counts[k]) {
            return false;
        }
    }

    return true;
}
