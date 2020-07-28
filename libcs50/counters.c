/*
 * counters.c - CS50 counter module
 * Implements key/counter pairs
 * See counters.h
 * Garrett Johnston CS50 20X 
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include "counters.h"
 #include "memory.h"

/* Local data type for counters */
typedef struct counterNode {
    int key; // Key, >= 0
    int counter; // Counter
    struct counterNode *next; // Next counter
} counterNode_t;

/* Global type */
typedef struct counters {
    struct counterNode *head;
} counters_t;

/******** Functions ********/
static counterNode_t * counterNode_new(int key);


// Local: Make a new counternode_t with given key, see counters.h
static counterNode_t * counterNode_new(int key) {
    counterNode_t *node = count_malloc(sizeof(counterNode_t));
    // Error allocating mem for node
    if (node == NULL) return NULL;

    node->key = key; 
    node->counter = 1; // Initialize counter to 1
    return node;
}

// Initialize new counters. See counters.h
counters_t * counters_new(void) {
    counters_t *ctrs = count_malloc(sizeof(counters_t));
    // Error allocating mem for counters
    if (ctrs == NULL) return NULL;

    ctrs->head = NULL;
    return ctrs;
}

// Add a new counter. See counters.h
int counters_add(counters_t *ctrs, const int key) {
    if (ctrs == NULL || key < 0) return 0; // Error w/ ctrs or invalid key

    // Traverse to see if key already exists
    for (counterNode_t *trav = ctrs->head; trav != NULL; 
    trav = trav->next) {
        if (trav->key == key) {
            trav->counter++;
            return trav->counter; // Return counter of given key
        }
    }
    // Didn't find given key; need to create a new node and insert at head
    counterNode_t *new = counterNode_new(key);
    new->next = ctrs->head;
    ctrs->head = new;
    return 1; // Counter of newly added node
}

// Get counter associated with given key. See counters.h
int counters_get(counters_t *ctrs, const int key) {
    if (ctrs == NULL || key < 0) return 0; // Errors w/ ctrs or invalid key

    // Traverse to find key
    for (counterNode_t *trav = ctrs->head; trav != NULL; 
    trav = trav->next) {
        if (trav->key == key) return trav->counter;
    }
    return 0; // Didn't find, return 0
}

// Set counter for a given key. See counters.h
bool counters_set(counters_t *ctrs, const int key, const int count) {
    /***** OUT OF MEMORY CASE??? *****/
    if (ctrs == NULL || key < 0 || count < 0) return false;

    for (counterNode_t *trav = ctrs->head; trav != NULL; 
    trav = trav->next) {
        if (trav->key == key) {
            trav->counter = count;
            return true;
        }
    }
    // Key doesn't exist; add
    counterNode_t *new = counterNode_new(key);
    new->counter = count;
    new->next = ctrs->head;
    ctrs->head = new;
    return true;
}

// Print all counters to a given file. See counters.h
void counters_print(counters_t *ctrs, FILE *fp) {
    if (fp != NULL) {
        if (ctrs != NULL) {
            fputc('{', fp);
            for (counterNode_t *trav = ctrs->head; trav != NULL; 
            trav = trav->next) {
                fprintf(fp, "%d=%d", trav->key, trav->counter);
                if (trav->next != NULL) {
                    fputc(',', fp);
                }
            }
            fputc('}', fp);
            fputc('\n', fp);
        } else {
            fprintf(fp, "null");
        }
    } 
}

// Iterate through all counters & do something to them with *itemfunc. See counters.h
void counters_iterate(counters_t *ctrs, void *arg, void (*itemfunc)
                        (void *arg, const int key, const int count)) {
    if (ctrs != NULL && itemfunc != NULL) {
        for (counterNode_t *trav = ctrs->head; trav != NULL; 
        trav = trav->next) {
            (*itemfunc)(arg, trav->key, trav->counter);
        }
    }
}

// Delete the entire counterset. See counters.h
void counters_delete(counters_t *ctrs) {
    if (ctrs != NULL) {
        for (counterNode_t *node = ctrs->head; node != NULL;) {
            counterNode_t *next = node->next;
            count_free(node); 
            node = next;
        }
        count_free(ctrs);
    }
}
