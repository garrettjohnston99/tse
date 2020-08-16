/*
 * word.c - see word.h
 *
 * Garrett Johnston CS50 20X
 */ 


 #include <ctype.h>


// Normalize a word to all lowercase. See word.h
int normalizeWord(char *word) {
    int ret = -1;

    for (int i=0; word[i] != '\0'; i++) {
        // Non-whitespace character is non-alphabetical: flag -> false
        if (!isspace(word[i]) && !isalpha(word[i])) {
            if (ret == -1) ret = i;
            continue;
        }

        word[i] = tolower(word[i]);
    }

    return ret;
 }
 