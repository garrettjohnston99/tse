/*
 * set.c - see set.h
 * Implementation of unordered set of string/void * pairs as linked list
 * 
 * Garrett Johnston CS50 20X
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "set.h"


/******** LOCAL TYPE ********/
typedef struct setNode {
    char *key;
    void *item;
    struct setNode *next;
} setNode_t;

/******** LOCAL TYPE FUNCTIONS ********/
static setNode_t * setNode_new(char *key, void *item);


/*
* setNode_new
* Initialize a new node for the set given key/item
* Returns NULL if error allocating memory for node
*/
static setNode_t * setNode_new(char *key, void *item) {
    setNode_t *node = malloc(sizeof(setNode_t));
    if (node == NULL) return NULL;

    node->key = key;
    node->item = item;
    node->next = NULL;

    return node;
}


/******** GLOBAL TYPE ********/
typedef struct set {
    struct setNode *head;
} set_t;


/*
 * set_new - see set.h
 * Initialize new set_t struct with NULL head
 * returns NULL if error allocating memory for struct
 */
set_t *set_new(void) {
    set_t *set = malloc(sizeof(set_t));
    if (set == NULL) return NULL;

    set->head = NULL;
    return set;
}


/*
 * set_insert - see set.h
 * Insert a new key/item pair into the set
 */
bool set_insert(set_t *set, const char *key, void *item) {
    if (set == NULL || key == NULL || item == NULL) return false;

    void *found = set_find(set, key);
    // Key already exists in set
    if (found != NULL) return false;

    char *keyCopy = malloc(strlen(key)+1);
    strcpy(keyCopy, key);

    // New node inserted at head
    setNode_t *newNode = setNode_new(keyCopy, item);
    newNode->next = set->head;
    set->head = newNode;
    
    return true;
}


/*
 * set_find - see set.h
 * Returns an item for the given key if it exists
 */
void *set_find(set_t *set, const char *key) {
    if (set == NULL || key == NULL) return NULL;

    setNode_t *trav = set->head;
    while (trav != NULL) {
        if (strcmp(key, trav->key) == 0) return trav->item;
        trav = trav->next;
    }

    // Not found
    return NULL;
}


/*
 * set_print - see set.h
 * Prints all items in the set to fp
 */
void set_print(set_t *set, FILE *fp, 
                void (*itemprint)(FILE *fp, const char *key, void *item)) {
    
    if (set != NULL && fp != NULL) {
        if (itemprint != NULL) {
            // Opening bracket
            fputc('{', fp);

            setNode_t *trav = set->head;
            while (trav != NULL) {
                (*itemprint)(fp, trav->key, trav->item);
                if (trav->next != NULL) fputc(',', fp);
                trav = trav->next;
            }
            
            fputc('}', fp);
        } else {
            // Null itemprint
            fprintf(fp, "{}");
        }
    }
    fputc('\n', fp);
}


/*
 * set_iterate - see set.h
 * Call (*itemfunc) on each item
 */
void set_iterate(set_t *set, void *arg,
                    void (*itemfunc)(void *arg, const char *key, void *item)) {
    
    if (set != NULL && itemfunc != NULL) {

        setNode_t *trav = set->head;
        while (trav != NULL) {
            (*itemfunc)(arg, trav->key, trav->item);
            trav = trav->next;
        }
    }
}


/*
 * set_delete - see set.h
 * Deletes the set, freeing all keys and items(w/ itemdelete)
 */
void set_delete(set_t *set, void (*itemdelete)(void *item)) {
    if (set != NULL) {
        
        setNode_t *trav = set->head;
        while (trav != NULL) {
            if (itemdelete != NULL) (*itemdelete)(trav->item);

            free(trav->key);
            setNode_t *next = trav->next;
            free(trav);
            trav = next;
        }

        free(set);
    }

}
