# TSE Querier Implementation Spec
## Garrett Johnston CS50 20X

Implemented as pseudocode describes in REQUIREMENTS.md & DESIGN.md

#### DECOMPOSITION

* querier.c: validates commandline arguments - directory created by crawler, and an index file assumed to have been created by indexer. 

    * processQueries(): While there are still queries to process from stdin, for each one: read from stdin. Validate using cleanQuery(). If valid, use scoreQuery() to score queries. If any documents match the query, use printResults() to print results in order of decreasing score.

    * cleanQuery(): Normalize a query to lowercase using normalizeWord(), saving the first index of a non-alphabetical character if there is one. Print the normalized query, then begin scanning for violations of the limitations of the query as described in REQUIREMENTS.md and DESIGN.md, printing an error message if there are any. 

    * scoreQuery(): Scores an entire query by passing off the work of scoring andsequences(see Backus-Nour Form) to scoreSequence(). Initializes a counterset(docID -> score) of totals to NULL - clones the first andsequence counterset to the totals counterset. While there are still andsequences to score, score them and sum into the total scores counterset. 

    * scoreSequence(): Called by scoreQuery. On the first call, call strtok on a copy of the query - on subsequent calls, call strtok on NULL to prevent an infinite loop. Like scoreQuery(), initialize a score counterset to NULL, which will become the first score counterset of the first word in the andsequence. While there are still words in the andsequence that are not the "or" operator, get frequency countersets for the words from the index. Merge into the total andsequence counterset(see Backus-Nour Form) with counters_merge. If the "and" operator is encountered, skip over it - adjacent non-operators are implicit "and" operators anyway. 

    * printResults(): Allocates an array of queryScore_t structs initialized to NULL. Using the counterset of scores, iterates over each docID/score pair and creates a queryScore_t struct and inserts it into the array in order of decreasing score. Then, for each queryScore_t struct in the array, deletes each struct and frees the array after retrieving the corresponding URL and printing the result in the format: 

    score: (score)  document: (docID)   URL

    


This implementation also makes use of a number of helper/utility functions, which are described below:

* counters_clone(): Returns a clone of a given counterset, or NULL if the given counterset is NULL or there was an error allocating memory for the clone. Passes work to counters_iterate with clone_helper(), which uses counters_set on the clone for each key/count pair in the given counterset. Importantly, does NOT clone key/count pairs with count == 0.

* counters_intersection(): Takes the intersection of two countersets(totals, word_ctrs_copy). Totals becomes the resultant counterset. Uses counters_iterate and intersection_helper() to first merge totals into word_ctrs_copy; this eliminates key/count pairs just in totals. Then merges word_ctrs_cpy back into totals, which eliminates key/count pairs just in word_ctrs_copy. Used in evaluating scores for andsequences. word_ctrs_copy must be a *copy* of a counterset retrieved from index, otherwise the index will be irreversibly changed.

* counters_sum(): Sum scores from an andsequence into a counterset of total scores. Uses counters_iterate and sum_helper() to sum each count into the total score counterset. 

* counters_size(): Returns the size of a counterset by incrementing a counter initialized to 0 for each pair in a counterset. Returns 0 if the given counterset is NULL. Uses counters_iterate and size_helper().

* counters_toArray(): Given a counterset and its size(from counters_size()), returns an array of queryScore_t structs(see below) sorted in decreasing order by score. Allocates memory for the array and initializes the memory to NULL, then uses counters_iterate and toArray_helper(). The array and its contents must later be free'd. 

* min(): Returns the minimum of two integers, defaulting to the second if they are equal. Called in mergeCounters().

* prompt(): Prompts for a query if taking input from a keyboard user. 


Finally, this implementation makes use of a local type: struct queryScore_t, which has the following fields:
* key: document ID
* score: score for the given query
* URL: the URL found in the document ID

Its functions are described below.

* queryScore_new(): Allocates memory for a new queryScore_t struct, returning NULL if memory was not allocated. Initializes key and score fields to the given values. 

* queryScore_getURL(): Opens crawlerDir/key for the given queryScore_t struct, reads the first line(URL), and saves it into the queryScore_t URL field. Returns true if successful, false if the given struct is NULL or there was a problem opening the file.

* queryScore_delete(): Deletes a queryScore_t struct; only needs to free the URL since it was malloc'd. 


This implementation also makes use of the *hashtable* and *set* modules indirectly, since they are wrapped up by an *index* struct. These modules are described in hashtable.h, set.h, and index.h, respectively. 
