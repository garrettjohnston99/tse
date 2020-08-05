/* 
 * index.c - utilities for saving/loading indices for TSE
 * see index.h
 *
 * Garrett Johnston CS50 20X
 */

 #include "../libcs50/counters.h"
 #include "../libcs50/hashtable.h"
 #include "../libcs50/file.h"
 #include "../libcs50/webpage.h"
 #include "index.h"
 #include "word.h"
 #include <math.h>
 #include <stdlib.h>
 #include <string.h>


/********** STATIC FUNCTION PROTOTYPES **********/
/*
 * save_ht: passed to hashtable_iterate to save index
 * arg is a file pointer, counter is a counters_t struct
 * passes work off to counters_iterate with save_counters
 */
static void save_ht(void *arg, const char *word, void *counters);


/*
 * save_counters: called by save_ht to save contents of each counters in 
 * the index's hashtable. 
 * arg is a file pointer. Prints to format described in REQUIREMENTS.md
 */
static void save_counters(void *arg, const int key, const int count);


/********** STATIC FUNCTIONS ***********/

// For local use in index_save. Passes work to save_counters
static void save_ht(void *arg, const char *word, void *counters) {
    FILE *fp = arg; 
    counters_t *ctrs = counters;

    fprintf(fp, "%s", word);
    counters_iterate(ctrs, fp, save_counters);
    fprintf(fp, "\n");
}


// For local use by save_ht for each counters_struct in the index
static void save_counters(void *arg, const int key, const int count) {
    FILE *fp = arg;
    fprintf(fp, " %d %d", key, count);
}



/********** STRUCT INDEX FUNCTIONS **********/

/***** GLOBAL TYPE *****/
// Global type: wrapper for hashtable (word -> counters)
typedef struct index {
    hashtable_t *ht;
} index_t;


/***** INDEX_GET_HT *****/
// See index.h
hashtable_t * index_get_ht(index_t *idx) {
    return (idx->ht);
}


/***** INDEX_NEW *****/
// See index.h
index_t * index_new(const int num_slots) {
    index_t *idx = malloc(sizeof(index_t));
    hashtable_t *ht = hashtable_new(num_slots);

    idx->ht = ht;

    return idx;
}


/***** INDEX_INCREMENT *****/
/* Increments the counter for the docID of a given word. 
 * See index.h
 */
void index_increment(index_t *idx, const char *word, const int docID) {
    if (idx != NULL && word != NULL && docID > 0) {

        // Try and retrieve the corresponding counters struct
        counters_t *ctrs = hashtable_find(idx->ht, word);

        // Word not in index yet; make new ctrs and add to index
        if (ctrs == NULL) {
            ctrs = counters_new();
            counters_add(ctrs, docID);
            hashtable_insert(idx->ht, word, ctrs);
        } else {
            // Word in index, need to increment counter for this docID
            counters_add(ctrs, docID);
        }
    }
}


/***** INDEX_INSERT *****/
/* Inserts a word/counters pair into the index.
 * Used in index_load. See index.h
 */
 void index_insert(index_t *idx, const char *word, counters_t *ctrs) {
    hashtable_insert(index_get_ht(idx), word, ctrs);
 }


/***** INDEX_DELETE *****/
// Deletes the index struct - see index.h
void index_delete(index_t *idx) {
    // Pass off to hashtable_delete with counters_delete as itemdelete 
    hashtable_delete(index_get_ht(idx), counters_delete_alias);
    free(idx);
}


void counters_delete_alias(void *counters) {
    counters_t *ctrs = counters;
    counters_delete(ctrs);
}


// Build the index from enumerated documents in the given directory.
// See index.h
void index_build(const char *pageDir, index_t *idx) {
    int docID = 1; // starting document number

    // length of pageDir/docId + null terminator. 
    char *filePath = malloc(strlen(pageDir) + (1+floor(log10(docID))) + 2);
    sprintf(filePath, "%s/%d", pageDir, docID);

    FILE *fp = fopen(filePath, "r");
    free(filePath);
    while (fp != NULL) {
        // process first two lines into a webpage_struct(URL, depth)
        char *URL = freadwordp(fp);

        char *tmp = freadwordp(fp);
        int depth = atoi(tmp);
        free(tmp);

        webpage_t *page = webpage_new(URL, depth, NULL);
        webpage_fetch(page); // Get HTML to use webpage_getNextWord

        int pos = 0;

        char *word;
        while ((word = webpage_getNextWord(page, &pos)) != NULL) {
            // Also want to ignore words less than three characters long
            if (strlen(word) > 2) {
                normalizeWord(word); // Normalize to all lowercase
                index_increment(idx, word, docID);
            }

            free(word); // Free as required in file.h, copied by hashtable
        }
        // clean up this doc
        fclose(fp);
        webpage_delete(page);
        

        // Prep next file for next iteration
        char *filePath = malloc(strlen(pageDir) + (1+floor(log10(++docID))) + 2);
        sprintf(filePath, "%s/%d", pageDir, docID);
        fp = fopen(filePath, "r");
        free(filePath);
    }
    
}


// Save a built index to a given file. see index.h
void index_save(const char *saveFile, index_t *idx) {
    FILE *fp = fopen(saveFile, "w"); // set up

    // Iterate over index with save_ht/save_counters
    hashtable_iterate(index_get_ht(idx), fp, save_ht);

    fclose(fp); // clean up
}



// Load an index struct from a file
// Assumed file was created by indexer - see index.h
index_t * index_load(const char* indexFile) {
    FILE *fp = fopen(indexFile, "r");
    if (fp == NULL) {
        fprintf(stderr, "Issue opening %s for load\n", indexFile);
        return NULL;
    }

    index_t *idx = index_new(lines_in_file(fp));

    char *line; // entire line read from file
    char *word; // first token from line, key for hashtable

    char *docID; // first int of each pair
    char *freq; // second int of each pair

    while ((line = freadlinep(fp)) != NULL) {
        counters_t *ctrs = counters_new();
        word = strtok(line, " "); 

        // now call strtok in pairs
        while ((docID = strtok(NULL, " ")) != NULL) {
            freq = strtok(NULL, " ");
            // Convert to int and add to counters for this word
            counters_set(ctrs, atoi(docID), atoi(freq));
        }

        index_insert(idx, word, ctrs);

        free(line);
    }

    fclose(fp);
    return idx;
}
