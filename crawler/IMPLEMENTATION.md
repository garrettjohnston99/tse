# crawler.c 
## Garrett Johnston CS50 20X

Implemented as pseudocode is described in DESIGN.md & REQUIREMENTS.md

#### DECOMPOSITION

* crawler.c: validates commandline arguments - internal seed URL(http://old-www.cs.dartmouth.edu), valid directory, and valid max Depth.
Initializes bag of struct webpages to visit, as well as a hashtable of seen URLs. Inserts the seed webpage and calls crawler().

* crawl(): While the bag of webpages to visit is not empty, crawler extracts a webpage from the bag and adds it to the hashtable of seen URLs. Then, barring errors, successively calls pageFetcher(), pageSaver(), and pageScanner() from pagedir.c.

    * pageFetcher(): takes a webpage struct as an argument and fleshes out the webpage->html field by calling webpage_fetch. Returns true if successful, false otherwise. 

    * pageSaver(): takes a webpage struct and the directory passed as a commandline argument. Creates an enumerated file in the directory and writes the URL and depth on the first two lines, then the page's HTML character by character. Returns true if successful, false if error creating the file. 

    * pageScanner(): takes the bag of URLs to visit and the hashtable of seen URLs, as well as a webpage struct. Leverages webpage_getNextURL to find all URLs in the given webpage's HTML, adding the URL to the hashtable, and adding a new webpage struct with that URL to the bag if the URL was not previously in the hashtable.

This implementation makes use of the modules *bag* and *hashtable*, and therefore *set* indirectly, which are described in bag.h, hashtable.h, and set.h.
