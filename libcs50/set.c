/*
 * set.c
 * Implementation of unordered set of string/item pairs
 * See set.h
 * Garrett Johnston CS50 20X
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "set.h"
 #include "memory.h"


/******** Local type ********/
typedef struct setNode {
    char *key; // Pointer to key given to initialization function
    void *item;
    struct setNode *next;
} setNode_t;

/********* Local functions ********/
static setNode_t * setNode_new(char *key, void *item);


// Allocate and initialize new node. 
static setNode_t * setNode_new(char *key, void *item) { 
    setNode_t *node = count_malloc(sizeof(setNode_t));

    // Error allocating mem for node
    if (node == NULL) return NULL;

    node->key = key; 
    node->item = item;

    return node;
}

/******** Global Type ********/
typedef struct set {
    struct setNode *head;
} set_t;

// Initialize new set object. See set.h
set_t *set_new(void) {
    set_t *set = count_malloc(sizeof(set_t));

    // Error allocating mem for set pointer
    if (set == NULL) return NULL;

    set->head = NULL;
    return set;
}

// Insert a key/item pair into the set. If duplicate/bad arguments return false. 
// Return true if successful insertion. See set.h
bool set_insert(set_t *set, const char *key, void *item) {
    if (set == NULL || key == NULL || item == NULL) return false;
    
    // Check if key exists
    for (setNode_t *trav = set->head; trav != NULL; trav = trav->next) {
        // Key already exists
        if (strcmp(key, trav->key) == 0) return false;
    }

    // Copy given key to new address
    char *k = count_malloc(strlen(key)+1);
    strcpy(k, key);

    // Create new node and insert at head
    setNode_t *new = setNode_new(k, item);
    new->next = set->head;
    set->head = new;

    return true;
}

// Return the item corresponding to the given key, if it exists. See set.h
void *set_find(set_t *set, const char *key) {
    if (set == NULL || key == NULL) return NULL;

    for (setNode_t *trav = set->head; trav != NULL; trav = trav->next) {
        if (strcmp(key, trav->key) == 0) return trav->item;
    }
    // Key not found
    return NULL;
}

// Print all items in the set to fp with (*itemprint) function. See set.h
void set_print(set_t *set, FILE *fp, 
                void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (set != NULL && fp != NULL) {
        if (itemprint != NULL) {
            fputc('{', fp);
            for (setNode_t *trav = set->head; trav != NULL; trav = trav->next) {
                // Function responsible for putting parens around items
                (*itemprint)(fp, trav->key, trav->item);
                if (trav->next != NULL) {
                    fputc(',', fp);
                }
            }
            fputc('}', fp);
        } else {
            // Null itemprint function
            fprintf(fp, "{}");
        }
    }
    fputc('\n', fp);
}

// Iterate over given set and do (*itemfunc) to each item. See set.h
void set_iterate(set_t *set, void *arg,
                    void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (set != NULL && itemfunc != NULL) {
        for (setNode_t *trav = set->head; trav != NULL; trav = trav->next) {
            (*itemfunc)(arg, trav->key, trav->item);
        }
    }
}

// Delete the entire set after calling (*itemdelete) on each item. See set.h
void set_delete(set_t *set, void (*itemdelete)(void *item)) {
    if (set != NULL && itemdelete != NULL) {
        for (setNode_t *trav = set->head; trav != NULL; ) {
            // Delete the item using given function if it's not null
            (*itemdelete)(trav->item);
            
            count_free(trav->key); // Free the string key
            setNode_t *next = trav->next;
            count_free(trav); // Free this node
            trav = next;
        }

        count_free(set);
    }
}
