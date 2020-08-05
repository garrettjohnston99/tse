/* 
 * index.h - interface for index.c
 * includes index struct(wrapper for hashtable) to modulate operations
 * functions for saving and loading index files for TSE
 * Garrett Johnston CS50 20X
 */

 #include <stdio.h>
 #include "../libcs50/hashtable.h"
 #include "../libcs50/counters.h"


 /***** INDEX STRUCT FUNCTIONS *****/

/***** GLOBAL TYPE *****/
// Struct to hold the index
typedef struct index index_t;

/***** INDEX_GET_HT *****/
// Getter for the index's wrapped up hashtable
hashtable_t * index_get_ht(index_t *idx);

/***** INDEX_NEW *****/
// Creates new index struct and initializes its hashtable with num_slots
index_t * index_new(const int num_slots);

/***** INDEX_INCREMENT *****/
/* Increments the counter for the docID of a given word. 
 * If word isn't in the index yet, place it with new counter (docID, 1)
 * If docID isn't in the counter yet, add it to the counter
 * Assumes word has already been normalized.
 * word can(should) be free'd after increment is called as the hashtable makes a copy.
 * Returns TRUE for successful increment/add and FALSE for null arguments.
 */
void index_increment(index_t *idx, const char *word, const int docID);

/***** INDEX_INSERT *****/
/* Wrapper for inserting a word/counters pair into the index's hashtable
 * Leverages hashtable_insert
 */
void index_insert(index_t *idx, const char *word, counters_t *ctrs);

/***** INDEX_DELETE *****/
/* Deletes the entire index
 * Passes off to hashtable_delete with counters_delete as the item deleter
 * Then frees the entire index struct
 */
void index_delete(index_t *idx);

// Index delete uses this alias for counters_delete
// So that the signature required by hashtable_delete matches
void counters_delete_alias(void *counters);


/***** FILE-EXTERNAL FUNCTIONS *****/
/*
 * index_build: given a directory of files created by crawler(verified) and an empty hashtable(index)
 * scans files created by crawler, normalizing words and building the index hashtable
 * which maps from normalized words to docID, frequency pairs
 */
void index_build(const char *pageDir, index_t *idx);

/*
 * index_save: given an existing, writeable file and a built index
 * Assumes the file is valid and the index is built. 
 * writes index in the format described in REQUIREMENTS.md
 * Leverages hashtable_iterate and counters_iterate
 */
void index_save(const char *saveFile, index_t *idx);


/*
 * index_load: given a file assumed to have been written by indexer
 * parse file to create an index_t struct and return it.
 * returns NULL if problem opening indexFile for reading.
 * Used in indextest.c
 */
index_t * index_load(const char* indexFile);
