/*
 * pagedir.h - prototypes for pagedir.c 
 * webpage/directory operations
 *
 * Garrett Johnston CS50 20X
 */ 


#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/bag.h"

/*
 * Validates a directory as existing & writeable to
 * Creates a dummy file to write to
 */
bool validateDirectory(const char *dirPath);


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
