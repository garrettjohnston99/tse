/*
 * word.c - see word.h
 *
 * Garrett Johnston CS50 20X
 */ 


 #include <ctype.h>


// Normalize a word to all lowercase. See word.h
void normalizeWord(char *word) {
     for (char *p = word; *p; p++) {
         *p = tolower(*p);
     }
 }
 