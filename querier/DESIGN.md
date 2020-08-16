# TSE Querier Design Spec
## Garrett Johnston CS50 20X

Requirements Spec: REQUIREMENTS.md

The TSE **querier** reads the index file produced by TSE **indexer** and page files produced by TSE **crawler** to answer search queries submitted via stdin.

### User Interface

Querier is executed from the commandline, always having two arguments, as below:

```
./querier crawlerDirectory indexFile
```

Where *crawlerDirectory* is a directory of page files produced by **crawler** and *indexFile* is the corresponding index produced by **indexer**. 

Once the program has been executed, the user is prompted for queries. 

### Inputs and outputs

Inputs: command-line parameters(above), and queries. Queries may make use of 'and' and 'or' operators to limit or increase the scope of a query. Queries must adhere to the following limitations which are also described in REQUIREMENTS.md: 

* A query may not begin with 'and' or 'or' literals
* A query may not contain adjacent 'and' or 'or' literals
* A query may not contain characters other than letters and spaces

Outputs: For each query, 
* If valid, a list of URLs ranked by score 
* If invalid, a corresponding error message

### Functional Decomposition into Modules

Querier makes use of the common module, in particular leveraging index.c and word.c to load an index_t struct from *indexFile* and normalize queries to lowercase, respectively. Querier also makes use of *queryScore* structs which are wrappers for docID, score, and URL. 

* *main* parses arguments and initializes *processQueries*
* *processQueries* validates and cleans query syntax using *cleanQuery*, then calls initializes the following modules
* *scoreQuery* scores each document based on the given query
* *printResults* sorts the documents by score in decreasing order, and prints them to stdout


### Pseudcode for Logic/Algorithmic Flow

1. Run from commandline; validate arguments. *crawlerDirectory* must be created by **crawler**. Assumes that if *indexFile* is readable, it was created by **indexer**. 
2. Load an index struct.
3. While there are still queries to process: 
    4. Normalize the query; if it contains non-alphabetical characters other than spaces, continue to the next query
    5. Process the query word by word, making sure it adheres to the limitations described above. 
    6. If the query is valid, create a counterset (docID -> score) for the query by scoring andsequences(see Backus-Nour Form) individually. 
    7. If there are documents that match the query, print the results in decreasing order.

### Dataflow Through Modules

*main* parses and validates commandline parameters, then loads an index from *indexFile* and passes it to *processQueries*, which validates and cleans query syntax using *cleanQuery*. 

*cleanQuery* uses *normalizeWord* in word.c to convert a query to all lowercase. *normalizeWord* also returns the first index of a non-alphabetical character if it exists in the query. 

After a query's syntax is cleaned and validated, *processQueries* passes a query and the index to *scoreQuery*.

Once the query is scored, if there are any documents that match the query, the results are printed in decreasing score order by *printResults*.

### Major Data Structures

1. *index*(hashtable) of (word) -> (documentID, count)
2. *queryScore* struct that contains a document's score for a single query, the document ID, and its URL


### Testing Plan

*Integration testing*: First test with some invalid commandline parameters - wrong number of arguments, invalid crawler directory, and non-existant indexFile. 

All files needed for the below tests are included in the queryTest directory - queries_xxxx are files containing queries to be piped, scores_xxxx have the expected documents/scores, possibly in a different order than querier prints them. 

0. Test the program with greater than or less than 2 commandline arguments. 

0. Test the program with a directory that was not created by crawler.

0. Test the program with a non-existant indexFile. 

1. Test the program on a file of singleton queries and verify by eye that the results of these queries match the frequencies in the indexFile.

2. Test the program on a file of simple "or" queries and verify by eye that the results of these queries match the sum of frequencies in the indexFile.

3. Test the program on a file of simple "and" queries and verify by eye that the results of these queries match the minimum of each word's frequency in the indexFile.

4. Test the program on a file of differently ordered, but equivalent queries and verify by eye that the results of these queries are correct but are also equivalent. 
