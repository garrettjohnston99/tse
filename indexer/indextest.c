/*
 * indextest.c - test module for TSE indexer
 * Takes two commandline parameters: oldIndexFile and newIndexFile
 * Parses index from old file, and loads it into an index_t struct
 * Then writes the index_t struct to newIndexFile
 * 
 * Commandline argument validation is minimal. 
 *
 * Garrett Johnston CS50
 */ 


 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 #include "index.h"


 int main(const int argc, const char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Invalid # of arguments\n");
        return 1;
    }

    // Make copies of commandline arguments
    char *old = malloc(strlen(argv[1]) + 1);
    sprintf(old, "%s", argv[1]);
    char *new = malloc(strlen(argv[2]) + 1);
    sprintf(new, "%s", argv[2]);


    index_t *oldIndex = index_load(old); // load index struct from old file
    index_save(new, oldIndex); // save struct to new file


    // clean up
    free(old);
    free(new);
    index_delete(oldIndex);

    return 0;
 }

