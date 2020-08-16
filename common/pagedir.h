/*
 * pagedir.h - prototypes for pagedir.c 
 * webpage/directory operations
 *
 * Garrett Johnston CS50 20X
 */ 

#ifndef __PAGEDIR_H
#define __PAGEDIR_H


#include "webpage.h"
#include "hashtable.h"
#include "bag.h"

/*
 * Validates a directory as existing & writeable to
 * creates a dummy file if mode == 'w'/tries to read a dummy file if mode == 'r'
 *
 */
bool validateDirectory(const char *dirPath, const char *dummyName, const char* mode);

/*
 * Validates a file as existing & writeable to
 * If doesn't exist, creates it
 */
bool validateFile(const char *fileName, const char *mode);

/*
 * Saves a page's HTML to path given in *pageDir. True for success, false for issue w/ file
 * Saves each page in a different enumerated text file
 */
void pageSaver(webpage_t *page, const char *pageDir, int id);


/*
 * Uses webpage_getNextUrl to scan for URLs in a page's HTML. 
 * Depends on an initialized *int position, initially passed as 0.
 * Inserts found URLs to seen hashtable, and if successful insert, places in 
 * toVisit bag for later visiting.
 */
void pageScanner(bag_t *toVisit, hashtable_t *seen, webpage_t *page);

#endif // __PAGEDIR_H
