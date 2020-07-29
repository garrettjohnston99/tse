/*
 * crawler.c 
 *
 *
 *
 *
 *
 *
 * Garrett Johnston CS50 20X
 */


 #include <stdio.h>
 #include <string.h>
 #include "../libcs50/hashtable.h"
 #include "../libcs50/bag.h"
 #include "../common/pagedir.h"
 #include "../libcs50/webpage.h"
 #include "../libcs50/memory.h"



void crawl(char *seed, const char* pageDir, const int maxDepth);


int main(const int argc, char *argv[]) {
    static const int depthLimit = 10; // Max depth

    // Validate commandline arguments.
    // 1: seedURL, 2: pageDir, 3: maxDepth
    if (argc != 4) {
        fprintf(stderr, "invalid # of arguments\n");
        return 1; // Invalid # args
    }
    
    /***** VALIDATE URL *****/
    if (!IsInternalURL(argv[1])) {
        fprintf(stderr, "Given URL is not internal.");
        fprintf(stderr, " Must start with http://old-www.cs.dartmouth.edu\n");
        return 2;
    }

    /***** VALIDATE DIRECTORY *****/
    if (!validateDirectory(argv[2])) {
        fprintf(stderr, "%s is not a directory or unwriteable\n", argv[2]);
        return 3;
    }

    /***** VALIDATE MAXDEPTH *****/
    int maxDepth;
    if (sscanf(argv[3], "%d", &maxDepth) != 1 || 
                maxDepth < 0 || 
                maxDepth > depthLimit) {

        fprintf(stderr, "Max depth argument either not an integer or out of bounds.");
        fprintf(stderr, " Must be between 0 and 10\n");
        return 4; 
    } 

    crawl(argv[1], argv[2], maxDepth);

    return 0;
}


/*********** CRAWL *********/
// toVisit: bag of struct webpages
// seen: hashtable of url -> depth
// pageDir: valid writeable directory to pass to pageSaver function
void crawl(char *seed, const char *pageDir, const int maxDepth) {
    int id = 1;

    // Heap copy of seed
    char *seedCopy = malloc(strlen(seed)+1);
    strcpy(seedCopy, seed);

    bag_t *toVisit = bag_new(); // webpages in bag do not have html yet
    hashtable_t *seen = hashtable_new(200); 

    webpage_t *seedPage = webpage_new(seedCopy, 0, NULL); 
    bag_insert(toVisit, seedPage);
    hashtable_insert(seen, seedCopy, ""); // "" used as item, since really using as hashset

    webpage_t *curr;
    while ((curr = bag_extract(toVisit)) != NULL) {
        // Successfully got html
        if (webpage_fetch(curr)) {
            printf("got html for %s\n", webpage_getURL(curr));
            pageSaver(curr, pageDir, id);
            id++;
            
            if (webpage_getDepth(curr) < maxDepth) {
                printf("scanning %s for URLs\n", webpage_getURL(curr));
                pageScanner(toVisit, seen, curr);           
            }
            
        } else {
            fprintf(stderr, "Unable to fetch webpage at %s\n", webpage_getURL(curr));
        }

        webpage_delete(curr);
    }

    // clean up
    bag_delete(toVisit, webpage_delete); 
    hashtable_delete(seen, NULL);
}
