/* 
 * pagedir.c - module for page/directory operations used in TSE
 * See pagedir.h for descriptions
 *
 *
 * Garrett Johnston CS50 20X
 */


#include "pagedir.h"
#include "../libcs50/webpage.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include <math.h>
#include <string.h>



/*** validateDirectory ***/
// Makes sure directory exists & writeable to by creating a dummy file
bool validateDirectory(const char *dirPath) {
    char *dummyFile = malloc(strlen(dirPath)+1+6);
    strcpy(dummyFile, dirPath);
    strcat(dummyFile, "/dummy");

    FILE *dummy = fopen(dummyFile, "w");
    if (dummy == NULL) {
        fprintf(stderr, "%s does not exist or is not writeable\n", dirPath);
        fclose(dummy);
        free(dummyFile);
        return false;
    }

    // clean up
    fclose(dummy); 
    remove(dummyFile);
    free(dummyFile);

    return true;
}



// Save a page's html to /pagedir/id
// *assumes that pageDir has already been validated as a writeable directory
// *writes URL \n depth \n HTML
void pageSaver(webpage_t *page, const char *pageDir, int id) {
    
    // Length of pageDir + '/' + length of id + '\0'
    char *filePath = malloc(strlen(pageDir) + (1+floor(log10(id))) + 2);
    sprintf(filePath, "%s/%d", pageDir, id);

    FILE *fp = fopen(filePath, "w");
    
    // Write URL, then depth, then HTML
    fprintf(fp, "%s\n%d\n%s\n", webpage_getURL(page), 
                                webpage_getDepth(page),
                                webpage_getHTML(page));

    fclose(fp);
    free(filePath);
}

// Finds all URLs in a page's HTML using webpage_getNextUrl
// Assumes page is valid and has base_url and html
void pageScanner(bag_t *toVisit, hashtable_t *seen, webpage_t *page) {
    int pos = 0;
    char *found; // freed in webpage_delete

    while ((found = webpage_getNextURL(page, &pos)) != NULL) {
        // IsInternalURL normalizes URL before checking if internal
        if (IsInternalURL(found)){
            if (hashtable_insert(seen, found, "")) {
                webpage_t *foundPage = webpage_new(found, webpage_getDepth(page) + 1, NULL);
                bag_insert(toVisit, foundPage); 
            }
        }
        free(found);
    }
    
}
