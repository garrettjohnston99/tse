# Garrett Johnston CS50 20X Lab 6 - Katherine Taylor
# Total: 94/100

## Makefiles: 12/12

## GIt Repo Requirements: 3/3

## Documentation: 15/15
Note: Your design.md is a bit specific - some of that information should be in Implementation.md. Design.md should be abstract design of the querier with no language-specific details (aka has decomposition of modules, use of major data structures, and your general strategy)

## Coding Style: 20/20

## Functionality: 36/40
For all of the below test cases I ran `./querier ~cs50/data/tse-output/toscrape-depth-1/ ~cs50/data/tse-output/toscrape-index-1`


### -1 - incorrect output
query: `modern music or classical poem`

your output is:
```
Matches 2 documents
score: 1         document: 67    http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/philosophy_7/index.html
score: 1         document: 56    http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/add-a-comment_18/index.html
```

but the correct output is:
```
Matches 1 documents (ranked):
score   1 doc  56: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/add-a-comment_18/index.html
```

### -3 - incorrect output - documents with a score of 0 get posted:
I'm only going to put the correct output for the sake of space (but you can run these queries with `./querier ~cs50/data/tse-output/toscrape-depth-1/ ~cs50/data/tse-output/toscrape-index-1`)

#### query: `political nonfiction or modern art or humor story`

your output had 61 matching documents

the expected output:
```
Query: political nonfiction or modern art or humor story 
Matches 6 documents (ranked):
score   1 doc  42: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/history_32/index.html
score   1 doc  56: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/add-a-comment_18/index.html
score   1 doc   2: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/page-2.html
score   1 doc  49: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/art_25/index.html
score   1 doc  63: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/childrens_11/index.html
score   1 doc  68: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/classics_6/index.html
```
-----------------------------  
#### query: `eat fat get thin or the time keeper or the black maria`

your output had 65 matching documents

the expected output:
```
Query: eat fat get thin or the time keeper or the black maria 
Matches 12 documents (ranked):
score   6 doc  13: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/the-black-maria_991/index.html
score   1 doc  27: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/health_47/index.html
score   1 doc   1: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html
score   1 doc   7: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/our-band-could-be-your-life-scenes-from-the-american-indie-underground-1981-1991_985/index.html
score   1 doc   8: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/rip-it-up-and-start-again_986/index.html
score   1 doc   9: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/scott-pilgrims-precious-little-life-scott-pilgrim-1_987/index.html
score   1 doc  10: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/set-me-free_988/index.html
score   1 doc  11: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/shakespeares-sonnets_989/index.html
score   1 doc  12: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/starving-hearts-triangular-trade-trilogy-1_990/index.html
score   1 doc  51: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/poetry_23/index.html
score   1 doc  64: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/fiction_10/index.html
score   1 doc  73: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books_1/index.html
```
-------------------------------------  
#### query: `paris kitchen recipes baking or white cat bear whale or best reviews`

your output had 65 matching documents

the expected output:
```
Query: paris kitchen recipes baking or white cat bear whale or best novels 
Matches 6 documents (ranked):
score   1 doc  41: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/food-and-drink_33/index.html
score   1 doc  63: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/childrens_11/index.html
score   1 doc   1: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html
score   1 doc   4: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/libertarianism-for-beginners_982/index.html
score   1 doc  58: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/science-fiction_16/index.html
score   1 doc  73: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books_1/index.html
```

## Memory Leaks: 8/10
-2 - memory blocks are "still reachable" (ex: when run `myvalgrind ./testing.sh)