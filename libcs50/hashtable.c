/*
 * hashtable.c - see hashtable.h
 * Implementation of hashtable using Jenkins hash(jhash.h, jhash.c)
 * Makes use of set module(set.h), which is implemented as a linked list
 *
 * Garrett Johnston CS50 20X
 */

 #include <stdio.h> 
 #include <stdlib.h>
 #include <string.h>
 #include "hashtable.h"
 #include "set.h"
 #include "jhash.h"


/******** GLOBAL TYPE ********/
typedef struct hashtable {
    set_t **array; // Array of sets
    unsigned long mod; // # slots in the hashtable to pass to jhash
} hashtable_t;


/*
 * hashtable_new - see hashtable.h
 * Initializes num_slots size array of set pointers
 */
hashtable_t *hashtable_new(const int num_slots) {
    if (num_slots < 1) return NULL;

    hashtable_t *ht = malloc(sizeof(hashtable_t));
    if (ht == NULL) return NULL;

    ht->array = malloc(num_slots*sizeof(set_t*));
    // Initialize memory to NULL
    memset(ht->array, 0, num_slots*sizeof(set_t*));
    ht->mod = num_slots;

    return ht;
}


/*
 * hashtable_insert - see hashtable.h
 * Returns false if pre-existing key or NULL arguments, true iff good insertion
 */
bool hashtable_insert(hashtable_t *ht, const char *key, void *item) {
    if (ht == NULL || key == NULL || item == NULL) return false;

    unsigned long slot = JenkinsHash(key, ht->mod);

    // Haven't initialized a set at this index
    if (ht->array[slot] == NULL) {
        ht->array[slot] = set_new();
    }

    // Return success of set insertion, which copies key for us
    return set_insert(ht->array[slot], key, item);
}


/* 
 * hashtable_find - see hashtable.h
 * Returns NULL for NULL arguments or key not found; corresponding item otherwise
 */
void * hashtable_find(hashtable_t *ht, const char *key) {
    if (ht == NULL || key == NULL) return NULL;

    unsigned long slot = JenkinsHash(key, ht->mod);

    // Return success of set find - will give NULL 
    // if slot hasn't been initialized yet, which is intended
    return set_find(ht->array[slot], key);
}


/*
 * hashtable_print - see hashtable.h
 * Print a text representation of the hashtable to a file
 */
void hashtable_print(hashtable_t *ht, FILE *fp, 
                    void (*itemprint)(FILE *fp, const char *key, void *item)) {

    if (fp != NULL && ht != NULL) {
        fprintf(fp, "{\n");
        if (itemprint != NULL) {
            for (int i=0; i<ht->mod; i++) {
                // Make sure set at this index has been initialized
                if (ht->array[i] != NULL) {
                    set_print(ht->array[i], fp, itemprint);
                }
            }

        } else {
            // Just print a newline for each slot
            for (int i=0; i<ht->mod; i++) {
                if (i < ht->mod -1) {
                    fputc('\n', fp);
                }
            }
        }

        fprintf(fp, "}\n");
    }              
}


/*
 * hashtable_iterate - see hashtable.h
 * Call (*itemfunc) on each item 
 */
void hashtable_iterate(hashtable_t *ht, void *arg, 
                        void (*itemfunc)(void *arg, const char *key, void *item)) {

    if (ht != NULL && itemfunc != NULL) {

        for (int i=0; i<ht->mod; i++) {
            // Pass to set_iterate which  handles NULL set case
            set_iterate(ht->array[i], arg, itemfunc);
        }

    }                       
}


/*
 * hashtable_delete - see hashtable.h
 * Deletes the hashtable, freeing all keys & items(w/ itemdelete)
 * Passes work to set_delete
 */
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item)) {
    if (ht != NULL) {

        for (int i=0; i<ht->mod; i++) {
            // Pass to set_delete which handles NULL set case
            set_delete(ht->array[i], itemdelete);
        }

        free(ht->array);
        free(ht);
    }
}
