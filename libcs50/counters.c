/* 
 * counters.c - see counters.h
 * Implementation of counterset(key, count) pairs as linked list
 * 
 * Garrett Johnston CS50 20X
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include "counters.h"


/******** LOCAL TYPE *********/
typedef struct counterNode {
    int key; // >= 0
    int counter; // Initialized to 1
    struct counterNode *next;
} counterNode_t;


/******** LOCAL TYPE FUNCTIONS ********/
static counterNode_t * counterNode_new(int key);


/*
 * counterNode_new - create a new node with given key
 * Returns NULL if error allocating memory for node
 */
static counterNode_t * counterNode_new(int key) {
    counterNode_t *node = malloc(sizeof(counterNode_t));
    if (node == NULL) return NULL;

    node->key = key;
    node->counter = 1;
    return node;
}


/******** GLOBAL TYPE ********/
typedef struct counters {
    struct counterNode *head;
} counters_t;


/*
 * counters_new - see counters.h
 * Returns NULL if error allocating memory for struct
 */
counters_t * counters_new(void) {
    counters_t *ctrs = malloc(sizeof(counters_t));
    if (ctrs == NULL) return NULL;

    ctrs->head = NULL;
    return ctrs;
}

/* 
 * counters_add - see counters.h
 * Increments the counter for the given key
 * returns 0 if ctrs is NULL or invalid key(negative)
 * otherwise the value of the counter for the given key
 */
int counters_add(counters_t *ctrs, const int key) {
    if (ctrs == NULL || key < 0) return 0;

    counterNode_t *trav = ctrs->head;
    while (trav != NULL) {
        if (trav->key == key) {
            // Found - increment and return
            trav->counter++;
            return trav->counter;
        }

        trav = trav->next;
    }

    // Need to insert a new key/count pair
    counterNode_t *new = counterNode_new(key);
    new->next = ctrs->head;
    ctrs->head = new;
    return 1;
}


/*
 * counters_get - see counters.h
 * get the value of the counter corresponding to the given key
 * returns 0 if counterset is NULL or key not found
 */
int counters_get(counters_t *ctrs, const int key) {
    if (ctrs == NULL || key < 0) return 0;

    counterNode_t *trav = ctrs->head;
    while (trav != NULL) {
        if (trav->key == key) return trav->counter;

        trav = trav->next;
    }

    // Didn't find
    return 0;
}


/*
 * counters_set - see counters.h
 * Set a counter for a given key, or add it if it doesn't exist
 */
bool counters_set(counters_t *ctrs, const int key, const int count) {
    if (ctrs == NULL || key < 0 || count < 0) return false;

    counterNode_t *trav = ctrs->head;
    while (trav != NULL) {
        if (trav->key == key) {
            trav->counter = count;
            return true;
        }

        trav = trav->next;
    }

    // Key didn't exist; need to add 
    counterNode_t *new = counterNode_new(key);
    new->counter = count;

    new->next = ctrs->head;
    ctrs->head = new;
    
    return true;
}


/*
 * counters_print - see counters.h
 * Print all key/count pairs to a file
 */
void counters_print(counters_t *ctrs, FILE *fp) {
    if (fp != NULL) {
        if (ctrs != NULL) {
            fputc('{', fp);

            counterNode_t *trav = ctrs->head;
            while (trav != NULL) {
                fprintf(fp, "%d=%d", trav->key, trav->counter);
                if (trav->next != NULL) {
                    fputc(',', fp);
                }

                trav = trav->next;
            }
            fputc('}', fp);
            fputc('\n', fp);
        } else {
            fprintf(fp, "null\n");
        }
    }
}


/*
 * counters_iterate - see counters.h
 * Iterate through counterset and call (itemfunc) on each pair
 */
void counters_iterate(counters_t *ctrs, void *arg,
                    void (*itemfunc)(void *arg, const int key, const int count)) {

    if (ctrs != NULL && itemfunc != NULL) {

        counterNode_t *trav = ctrs->head;
        while (trav != NULL) {
            (*itemfunc)(arg, trav->key, trav->counter);

            trav = trav->next;
        }
    }
}


/*
 * counters_delete - see counters.h
 * Delete the counterset, freeing each node
 */
void counters_delete(counters_t *ctrs) {
    if (ctrs != NULL) {

        counterNode_t *trav = ctrs->head;
        while (trav != NULL) {
            counterNode_t *next = trav->next;
            free(trav);
            trav = next;
        }

        free(ctrs);
    }
}
