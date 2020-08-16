/*
 * word.h - header for word.c
 * Utilities for parsing text in TSE
 * Garrett Johnston CS50 20X
 */

#ifndef __WORD_H
#define __WORD_H

/*
 * normalizeWord: take a char * and normalize to all lowercase
 * leverages toLower() function from ctype.h
 * also checks whether there are non-whitespace, non-alphabetical characters in the word
 * returns -1 if word is completely alphabetical
 * otherwise returns the first index of a non-whitespace, non-alphabetical character.
 * normalizes the entire string regardless of finding a non-alphabetical char
 */
int normalizeWord(char *word);

#endif // __WORD_H
