# TSE Indexer Implementation Spec
## Garrett Johnston CS50 20X

Implemented as pseudocode is described in DESIGN.md & REQUIREMENTS.md

#### DECOMPOSITION

* indexer.c: validates commandline arguments: directory created by crawler, and an index filename. 

    * struct index: essentially a wrapper for a hashtable of counters. Functions and typedef in common/index.c.

    * index_build(): while there are still files of HTML to get words from, index_build reads from files in the directory created by crawler, building an index_t struct that maps from words to counters of document ID and frequency. 

    * index_save(): pass off work to hashtable_iterate and counters_iterate in order to write the contents of the index_t struct built by index_build() in the format described in REQUIREMENTS.md

* indextest.c: reads an index from a file creater by indexer.c and creates an index_t struct, using index_load(), then saves it to a separate file using index_save() which is described above. 
    
    * index_load(): reads in an index line-by-line and creates an index_t struct by creating counters for each word and inserting the word/counters pairs into the index's hashtable. 


This implementation makes use of the *counters* and *hashtable* modules, and therefore *set* indirectly. The *hashtable* and *set* modules are wrapped up in an *index* struct. These modules are all described in counters.h, hashtable.h, set.h, and index.h, respectively. 