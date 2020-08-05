/*
 * indexer.c 
 *
 *
 *
 *
 * Garrett Johnston CS50 20X
 */

 #include <stdio.h>
 #include <string.h>
 #include "../libcs50/hashtable.h"
 #include "../libcs50/counters.h"
 #include "../libcs50/file.h"
 #include "../common/pagedir.h"
 #include "../common/index.h"


 int main(const int argc, char *argv[]) {
     /***** VALIDATE COMMANDLINE ARGS *****/
     // Number
     if (argc != 3) {
         fprintf(stderr, "invalid number of arguments\n");
         return 1;
     }


    // pageDirectory: just check if argv[1]/.crawler can be opened
    if (!validateDirectory(argv[1], "/.crawler", "r")) {
        fprintf(stderr, "%s is not a directory created by crawler\n", argv[1]);
        return 2;
    }

    
    // indexFilename: open will overwrite if already exists
    if (!validateFile(argv[2])) {
        fprintf(stderr, "couldn't create %s\n", argv[2]);
        return 3;
    }

    // word -> counters objs for each docID
    index_t *idx = index_new(1000); // # of unique words? need better estimate
    if (idx == NULL) {
        fprintf(stderr, "index couldn't be created\n");
        return 4;
    }
    
    // Make copies of commandline args
    char *pageDir = malloc(strlen(argv[1]) + 1);
    sprintf(pageDir, "%s", argv[1]);
    char *saveFile = malloc(strlen(argv[2]) + 1);
    sprintf(saveFile, "%s", argv[2]);


    index_build(pageDir, idx);

    index_save(saveFile, idx);


    // clean up copies 
    free(pageDir);
    free(saveFile);
    index_delete(idx); // clean up index 

    return 0;
 }

