/*
 * hashtable.c 
 * Implementation of hashtable using jenkins hash(jhash.c)
 * See hashtable.h
 *
 * Garrett Johnston CS50 20x
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "hashtable.h"
 #include "memory.h"
 #include "set.h"
 #include "jhash.h"

 /********* Global Type *********/
 typedef struct hashtable {
    set_t **array; // Array of sets
    unsigned long mod; // # of slots in the hashtable
}  hashtable_t;


// Create a new hashtable. Initializes num_slots size array of 
// pointers to sets. See hashtable.c
hashtable_t *hashtable_new(const int num_slots) {
    hashtable_t *ht = count_malloc(sizeof(hashtable_t*));
    if (ht == NULL) return NULL;

    // Allocate memory for array of pointers to sets
    /** Initialize to null?? **/
    ht->array = count_calloc(num_slots, sizeof(set_t*)); 
    ht->mod = num_slots;
    
    return ht;
}


// Insert key/item pair into the hashtable. 
// Returns false if key already exists or null arguments/error.
// True if item was successfully inserted. See hashtable.h
bool hashtable_insert(hashtable_t *ht, const char *key, void *item) {
    if (ht == NULL || key == NULL || item == NULL) return false;

    unsigned long slot = JenkinsHash(key, ht->mod); // Get index from hash function

    // Set for this index hasn't been initialized
    if (ht->array[slot] == NULL) {
        ht->array[slot] = set_new(); // Make new set for this index
    } 

    // Return whether or not insertion into set was successful
    // set_insert makes a copy of key for us, so no need to do so here
    return (set_insert(ht->array[slot], key, item));
}


// Find item for a given key and return it. Returns null for null arguments 
// or key not found. See hashtable.h
void *hashtable_find(hashtable_t *ht, const char *key) {
    if (ht == NULL || key == NULL) return NULL;

    unsigned long slot = JenkinsHash(key, ht->mod);

    // Check that set at index has been initialized
    if (ht->array[slot] == NULL) return NULL; // Uninitialized set

    // Pass to set_find
    return set_find(ht->array[slot], key);
}  


// Prints the entire hashtable to fp using (*itemprint).
// if null fp, prints nothing. prints null if null hashtable.
// If null itemprint, one line per hash slot. See hashtable.c
void hashtable_print(hashtable_t *ht, FILE *fp, 
                    void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (fp != NULL && ht != NULL) {
        fprintf(fp, "{\n");
        if (itemprint != NULL) {
            for (int i=0; i<ht->mod; i++) {
                // Make sure set at this index has been initialized
                if (ht->array[i] != NULL) {
                    // Pass to set_print
                    set_print(ht->array[i], fp, itemprint);
                }
            } 

        } else {
        // Null itemprint. Print a newline for each hashslot; ie {\n} for 1 slot
        for (int i=0; i<ht->mod; i++) {
            if (i != ht->mod - 1) {
                fputc('\n', fp);
            }
        }
        }
        fprintf(fp, "}\n");   
    } 
}


// Call (*itemfunc) with void *arg on each key/item pair in hashtable.
// Does nothing if ht or itemfunc is null. See hashtable.h
void hashtable_iterate(hashtable_t *ht, void *arg,
                        void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (ht != NULL && itemfunc != NULL) {
        for (int i=0; i<ht->mod; i++) {
            if (ht->array[i] != NULL) {
                // Pass off to set_iterate
                set_iterate(ht->array[i], arg, itemfunc);
            }
        }
    }
}


// Delete the hashtable Does nothing if ht is null or itemdelete is null.
// See hashtable.h
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item)) {
    if (ht != NULL && itemdelete != NULL) {
        for (int i=0; i<ht->mod; i++) {
            if (ht->array[i] != NULL) {
                // Pass to set_delete
                set_delete(ht->array[i], itemdelete);
            }
        }

        count_free(ht->array); // Free the array of pointers
        count_free(ht);
    }
}