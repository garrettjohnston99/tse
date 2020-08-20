/*
 * querier.c - querier for TSE
 * See README.md, IMPLEMENTATION.md
 * 
 * Usage: ./querier crawlerDir indexFile
 * Where crawlerDir is a directory created by TSE crawler, and indexFile is created by TSE indexer
 *
 * Garrett Johnston CS50 20X
 */


 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 #include <math.h>
 #include <unistd.h>
 #include "pagedir.h"
 #include "index.h"
 #include "word.h"
 #include "file.h"
 #include "counters.h"
 #include "hashtable.h"


/***** LOCAL TYPE FOR SORTING SCORES *****/
typedef struct queryScore {
    int key;
    int score;
    char *URL;
} queryScore_t;


/***** LOCAL STRUCT FUNCTION PROTOTYPES *****/
queryScore_t *queryScore_new(const int key, const int score);
bool queryScore_getURL(queryScore_t *qs, const char *crawlerDir);
void queryScore_delete(queryScore_t *qs);


/***** FUNCTION PROTOTYPES *****/
int fileno(FILE *stream);

// Main modules
void processQueries(index_t *idx, const char *crawlerDir);
bool cleanQuery(char *query);
counters_t * scoreQuery(index_t *idx, char *query);
counters_t * scoreSequence(index_t *idx, char *queryCopy, bool first);
void printResults(counters_t *scores, int size, const char *crawlerDir);

// Utility & helpers
counters_t * counters_clone(counters_t *cloneFrom);
void clone_helper(void *newCtrs, const int key, const int count);
void counters_intersection(counters_t *totals, counters_t *wordctrs_copy);
void intersection_helper(void *arg, const int key, const int count);
void counters_sum(counters_t *totals, counters_t *andScores);
void sum_helper(void *arg, const int key, const int count);
int counters_size(counters_t *ctr);
void size_helper(void *arg, const int key, const int count);
queryScore_t ** counters_toArray(counters_t *ctrs, int size);
void toArray_helper(void *query_array, const int key, const int count);
int min(int a, int b);
static void prompt(void);



int main(const int argc, const char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Wrong number of arguments\n");
        return 0;
    }

    // Validate directory created by crawler 
    if (!validateDirectory(argv[1], ".crawler", "r")) {
        fprintf(stderr, "Invalid directory: %s\n", argv[1]);
        return 1;
    }

    // Assumes if a file is readable it was created by indexer
    if (!validateFile(argv[2], "r")) {
        fprintf(stderr, "Invalid index file: %s\n", argv[2]);
        return 2;
    }

    // Load index structure
    index_t *idx = index_load(argv[2]);

    // Make sure that index was created successfully
    if (idx == NULL) {
        fprintf(stderr, "Error initializing index from %s\n", argv[2]);
        return 3;
    }


    processQueries(idx, argv[1]);
    

    index_delete(idx); // Clean up index structure

    return 0;
}


/*
 * processQueries - see DESIGN.md & IMPLEMENTATION.md
 * while there are still queries from stdin, clean them with cleanQuery(query)
 * If valid(ie cleanQuery returns true), pass query and index to scoreQuery
 *
 */
void processQueries(index_t *idx, const char *crawlerDir) {
    char *query; // Query from stdin; must free as it is malloc'd by readlinep

    prompt();
    while ((query = readlinep()) != NULL) {
        // Valid query
        if (cleanQuery(query)) {
            // Get scores for this query
            counters_t *scores = scoreQuery(idx, query);

            // Get size of scores counterset
            int size = counters_size(scores);
            if (size == 0) {
                printf("No documents match\n");
            } else {
                // Need to print results
                printResults(scores, size, crawlerDir);
            }

            counters_delete(scores);
        }

        // Clean up this query
        free(query);
        prompt();
    }

    printf("\n");
}


/*
 * clean - see DESIGN.md & IMPLEMENTATION.md
 * clean a query, ensuring that it is valid as defined below and in REQUIREMENTS.md
 *
 * cannot begin/end with 'and'/'or' operators
 * 'and'/'or' operators cannot be adjacent
 * cannot contain non-alphebetical characters
 * 
 * returns TRUE if no syntax errors in query, FALSE for errors. 
 */
bool cleanQuery(char *query) {

    int norm = normalizeWord(query); // see word.h
    printf("\nQuery: %s\n", query);

    // Contains non-alphabetical characters 
    if (norm != -1) {
        printf("Error: bad character '%c' in query\n", query[norm]);
        return false;   
    }

    // Make a copy so strtok() doesn't ravage the original query
    char *copy = malloc(strlen(query) + 1);
    strcpy(copy, query);

    char *token = strtok(copy, " ");

    // Query contained only whitespace
    if (token == NULL) {
        printf("\n");
        free(copy);
        return false;
    }

    // Make sure query doesn't start with an operator
    if (strcmp(token, "and") == 0 || strcmp(token, "or") == 0) {
        printf("'%s' cannot be first\n", token);
        free(copy);
        return false;
    } 

    char *next; // For looking ahead if(when) we see an operator
    while ((token = strtok(NULL, " ")) != NULL) {
        if (strcmp(token, "and") == 0 || strcmp(token, "or") == 0) {
            next = strtok(NULL, " "); // Look one ahead

            // Query ends with an operator
            if (next == NULL) {
                printf("'%s' cannot be last\n", token);
                free(copy);
                return false;
            }

            // Has adjacent operators
            if (strcmp(next, "and") == 0 || strcmp(next, "or") == 0) {
                printf("'%s' and '%s' cannot be adjacent\n", token, next);
                free(copy);
                return false;
            }
        }
    }

    free(copy);
    return true;
}


/*
 * scoreQuery - see DESIGN.md & IMPLEMENTATION.md
 * create a counter set of (docID -> score) for the given query
 * Guaranteed that the query is valid(see REQUIREMENTS.md) by cleanQuery()
 * Calls scoreSequence() to handle andsequences as defined by Backus-Naur Form 
 * Uses a counterset to keep running total of query scores
 */
counters_t * scoreQuery(index_t *idx, char *query) {
    // Make copy of query to mangle with strtok
    char *copy = malloc(strlen(query) + 1);
    strcpy(copy, query);

    // Counters object of docID -> running score for the entire query
    counters_t *totals = NULL;

    // Pointer for scores of each andsequence
    counters_t *andSeq = scoreSequence(idx, copy, true);
    while (andSeq != NULL) {
        // First andsequence; total scores are the scores for this andsequence
        if (totals == NULL) {
            // Clone into totals
            totals = counters_clone(andSeq);
        } else {
            // Sum into totals 
            counters_sum(totals, andSeq);
        }

        // Clean up this sequence's counterset and prep the next one
        counters_delete(andSeq);
        // NULL queryCopy just in case something goes wrong - don't want to accidentally
        // Restart strtok which would create infinite loop
        andSeq = scoreSequence(idx, NULL, false); 
    }

    
    free(copy); // clean up malloc'd copy of query
    return totals;
}


/*
 * scoreSequence - see DESIGN.md & IMPLEMENTATION.md
 * Scores an andsequence for a query - terminates & returns the counters_t struct
 * when encountering 'or'/null terminator, for summing with the running totals
 * 
 * takes index, the query(only used on first call for this query)
 * and a bool first, which indicates whether this is the first call for the query
 * if true, calls strtok with the query; otherwise, calls NULL to continue tokenizing
 * the same query
 * 
 * returns NULL if strtok(query) is NULL on its first call in this function(we're done)
 * otherwise returns a counters_t struct of (docID) -> score for this andsequence
 */
counters_t * scoreSequence(index_t *idx, char *queryCopy, bool first) {

    char *word = (first) ? strtok(queryCopy, " ") : strtok(NULL, " ");

    // If first token for this andsequence is NULL, we're done with the query already. 
    if (word == NULL) {
        return NULL;
    }
    
    counters_t *scores = NULL; // scores for this andsequence
    
    // Stop if we reach null terminator(word == NULL); done with this query
    // Also stop if we reach "or" operator; need to start a new andsequence
    while (word != NULL && strcmp(word, "or") != 0) {
        // Process scores using min and score counters

        // Reach "and" operator - just skip to next word in query
        if (strcmp(word, "and") != 0) {
            // Counterset for this word from the index
            counters_t *wordctrs = index_get(idx, word);

            // If NULL, wordctrs should be an empty counterset to calculate intersection properly
            bool notfound = wordctrs == NULL;
            if (notfound) wordctrs = counters_new();
            
            // First word in the andsequence, clone wordctrs into score
            if (scores == NULL) {
                scores = counters_clone(wordctrs); 
                if (notfound) counters_delete(wordctrs);
            } else {
                // Merge word scores into total scores with a copy of wordctrs
                counters_t *wordcopy = counters_clone(wordctrs);

                counters_intersection(scores, wordcopy);
                counters_delete(wordcopy);

                if (notfound) counters_delete(wordctrs);
            }
        
        }

        // Prep next word in this andsequence
        word = strtok(NULL, " ");
    }

    return scores;
}


/*
 * printResults - see DESIGN.md & IMPLEMENTATION.md
 * Given a complete counterset of scores for a query
 * Sort document IDs by the corresponding value in the counter
 * 
 * Then starting with the document with the highest score, print in format:
 * score: [score]\t document: [docID]\t URL
 * 
 */
void printResults(counters_t *scores, int size, const char *crawlerDir) {
    // Array of score structs initialized to NULL
    queryScore_t **qsArray = counters_toArray(scores, size);

    printf("Matches %d documents\n", size);
    for (int i=0; i<size; i++) {
        // Fill out URL fields in now sorted queryScore structs
        if (queryScore_getURL(qsArray[i], crawlerDir)) {
            // Now we can print
            printf("score: %d\t document: %d\t %s\n", 
                    qsArray[i]->score, qsArray[i]->key, qsArray[i]->URL);
        } else {
            // Trouble getting URL
            printf("Error getting URL for document %d\n", qsArray[i]->key);
            printf("score: %d\t document: %d\n", 
                    qsArray[i]->score, qsArray[i]->key);
        }
        
        queryScore_delete(qsArray[i]);
    }
    
    // Free the array now that all structs have been deleted
    free(qsArray);
}






/***** UTILITY FUNCTIONS *****/


/*
 * counters_clone - see DESIGN.md and IMPLEMENTATION.md
 * returns a clone of the given counterset, which must later be free'd.
 * does not clone key/count pairs with count == 0.
 * returns NULL if NULL cloneFrom counterset, or error allocating memory for the clone.
 * Passes work to counters_clone_helper using counters_iterate
 */
counters_t * counters_clone(counters_t *cloneFrom) {
    if (cloneFrom == NULL) return NULL;

    counters_t *clone = counters_new();
    if (clone == NULL) return NULL;

    counters_iterate(cloneFrom, clone, clone_helper);
    return clone;
}


void clone_helper(void *newCtrs, const int key, const int count) {
    counters_t *ctrs = newCtrs;
    if (count > 0) {
        counters_set(ctrs, key, count);
    }
}


/*
 * counters_intersection - see DESIGN.md & IMPLEMENTATION.md
 * Used to evaluate scores of andsequences - merges scores into totals counterset
 * word_ctrs_copy MUST be a copy of a counterset retrieved from index
 */
void counters_intersection(counters_t *totals, counters_t *word_ctrs_copy) {
    // First merge totals into word_ctrs, taking minimums
    counters_iterate(totals, word_ctrs_copy, intersection_helper);

    // Merge back into totals 
    counters_iterate(word_ctrs_copy, totals, intersection_helper);
}


void intersection_helper(void *arg, const int key, const int count) {
    counters_t *totals = arg;
    counters_set(totals, key, min(counters_get(totals, key), count));
}  


/*
 * counters_sum - see DESIGN.md & IMPLEMENTATION.md
 * for summing scores in countersets on either side of "or" operator
 * totals is a counterset of total scores for a query
 * andScores is a counterset of scores for an andsequence returned by scoreSequence
 */
void counters_sum(counters_t *totals, counters_t *andScores) {
    counters_iterate(andScores, totals, sum_helper);
}


void sum_helper(void *arg, const int key, const int count) {
    counters_t *totals = arg;
    
    // Just sum the given count(from andsequence_scores)
    // and the score from totals, which defaults to 0 if it doesn't have the key
    counters_set(totals, key, count + counters_get(totals, key));
}


/*
 * counters_size - get size of a counterset 
 * Returns 0 if NULL counterset
 */
int counters_size(counters_t *ctrs) {
    int size = 0;
    counters_iterate(ctrs, &size, size_helper);
    return size;
}


void size_helper(void *arg, const int key, const int count) {
    int *size = arg;
    (*size)++;
}


/*
 * counters_toArray - see DESIGN.md & IMPLEMENTATION.md
 * Returns an array of queryScore_t structs sorted in decreasing order by score
 * The array and its contents must later be free'd 
 */
queryScore_t ** counters_toArray(counters_t *ctrs, int size) {
    queryScore_t **array = malloc(size*sizeof(queryScore_t));
    memset(array, 0, size*sizeof(queryScore_t));
    // Insert keys sorted in decreasing order w/ helper
    counters_iterate(ctrs, array, toArray_helper);

    return array;
}


void toArray_helper(void *query_array, const int key, const int count) {
    queryScore_t **array = query_array;
    // Create new struct for this key/score pair
    queryScore_t *qs = queryScore_new(key, count);

    int i = 0;
    while (array[i] != NULL) {
        // Need to insert this queryScore_t in front of the one at this index
        if (qs->score > array[i]->score) {
            int j = i + 1;
            // Find first NULL(empty) index since we can't easily calculate size
            while (array[j] != NULL) j++;
            // Rightshift all elements
            while (j > i) {
                array[j] = array[j-1];
                j--;
            }

            break;
        }

        i++;
    }
        
    // Insert
    array[i] = qs;
}


/*
 * min - returns minimum of integers a and b for use in scoring andsequences
 */
int min(int a, int b) { 
    return ((a < b) ? a : b); 
}




/***** LOCAL TYPE FUNCTIONS *****/

/*
 * queryScore_new
 * Create a new queryScore_t struct w/ given key and score
 * returns NULL if problem allocating memory for struct
 */
queryScore_t *queryScore_new(const int key, const int score) {
    queryScore_t *qs = malloc(sizeof(queryScore_t));

    if (qs == NULL) {
        return NULL;
    }
    qs->key = key;
    qs->score = score;

    return qs;
}


/*
 * queryScore_getURL
 * Retrieve the URL for a queryScore to fill out the URL field
 * opens crawlerDir/key and reads the first line
 * returns false if given queryScore struct is null or error getting URL, true otherwise
 */
bool queryScore_getURL(queryScore_t *qs, const char *crawlerDir) {
    if (qs == NULL) {
        return false;
    }

    // crawlerDir/key + null terminator
    char *doc = malloc(strlen(crawlerDir) + (1+floor(log10(qs->key))) + 2);
    sprintf(doc, "%s/%d", crawlerDir, qs->key);

    FILE *fp = fopen(doc, "r");
    if (fp == NULL) {
        return false;
    }

    qs->URL = freadlinep(fp);

    fclose(fp);
    free(doc);
    return true;
}


/* 
 * queryScore_delete - delete a queryScore struct
 * just need to free URL since it was malloc'd by freadlinep
 * do nothing if NULL, otherwise free the URL if not NULL and free the struct
 */
void queryScore_delete(queryScore_t *qs) {
    if (qs != NULL) {
        if (qs->URL != NULL) {
            free(qs->URL);
        }
        free(qs);
    }
}


/*
 * prompt - print a prompt for a query if taking input from a keyboard user
 */
static void prompt(void) {
    if (isatty(fileno(stdin))) {
        printf("Query? ");
    }
}
