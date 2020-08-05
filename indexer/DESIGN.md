# TSE Indexer Design Spec

Requirements Spec: REQUIREMENTS.md

The TSE **indexer** reads the HTML text files made by **crawler** and writes the index to a file.

### User Interface

The indexer is executed from the commandline, always having two arguments, as below:

```
./indexer pageDirectory indexFilename
```

Where *pageDirectory* is a readable directory consisting of enumerated(1, 2, 3...) files of HTML discovered by crawler and *indexFilename* is the name of a file to write the index to. If *indexFilename* already exists, it will be overwritten.

### Inputs and outputs

Inputs: command-line parameters, see above.

Outputs: An index file in the following format(from REQUIREMENTS.md).

* one line per word, one word per line
* each line provides the word and one or more (docID, count) pairs, in the format
* *word docID count [docID count]...*
 * where *word* is a string of lower-case letters,
 * where *docID* is a positive non-zero integer,
 * where *count* is a positive non-zero integer,
 * where the word and integers are separated by spaces.

### Functional Decomposition into Modules

* index_t struct: wrapper for the index of words found by scanning the documents created by crawler. 
* index.c: contains index_t logic as well as utility functions index_build, which builds the index struct from the given files, and index_save, which saves the index struct to a file.

### Pseudocode for Logic/Algorithmic Flow

1. Run from commandline; validate arguments.
2. Create an index struct and pass to index_build:
    3. index_build: starting with pageDir/1, open enumerated files in the given directory. 
    4. Create a webpage struct for each file, and fetch the page's HTML.
    5. Insert each word in the page's HTML into the index after normalizing and ensuring the length is greater than or equal to 3. 
    6. Repeat until there are no more files in pageDir.
7. Take the built index and pass to index_save:
    8. index_save: iterate over the contents of the index using hashtable_iterate and counters_iterate.
9. Clean up; delete the index.

### Dataflow Through Modules

*main* parses and validates commandline parameters, then passes them to index_build and index_save.
*index_build* uses a wrapped-up hashtable(index) and counters structs to count which words appear in which documents, and how frequently. 
*index_save* outputs the built index to a given file. 

### Major Data Structures

1. *hashtable* of (word) -> (documentID, count)
2. *set* indirectly used by hashtable
3. *counters* to store (documentID, count) pairs

### Testing Plan

*Integration testing*: first test with a variety of invalid commandline parameters - wrong number of arguments, invalid directory, directory not created by crawler. 

In each case that a successful index is created, independently use indextest to compare the created index file and a reverse-engineered index file. 

Each index should be visually verified with indices in ~cs50/data/tse-output. 

0. Test the program with a less than or more than 2 commandline arguments.

0. Test indexer with an invalid page directory.

0. Test indexer with a valid directory not created by crawler.

1. Index a small, closed directory of cross-linked pages, then with index in ~cs50/data/tse-output.

2. Index a larger directory of cross-linked pages and verify with index in ~cs50/data/tse-output.

3. Once confident in the indexer, test on part of the cs50 playgrounda and verify with index in ~cs50/data/tse-output.
