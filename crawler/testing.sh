# bash testfile for ./crawler
# run by make test as bash -v testing.sh &> testing.out
# compares to letters-depth-4, toscrape-depth-1, and wikipedia-depth-1
# removed dummy files in crawler.c
# Garrett Johnston CS50 20X

# create dirs if necessary
if [[ ! -d "letters4" && ! -w "letters4" ]];
then
    mkdir "letters4"
    chmod u+w "letters4"
fi

if [[ ! -d "letters-depth-4" ]];
then
    cp -r ~cs50/data/tse-output/letters-depth-4 .
    echo "getting letters-depth-4"
fi

if [[ ! -d "toScrape1" && ! -w "toScrape1" ]];
then
    mkdir "toScrape1"
    chmod u+w "toScrape1"
fi

if [[ ! -d "toscrape-depth-1" ]];
then
    cp -r ~cs50/data/tse-output/toscrape-depth-1 .
    echo "getting toscrape-depth-1"
fi

if [[ ! -d "wikipedia1" && ! -w "wikipedia1" ]];
then
    mkdir "wikipedia1"
    chmod u+w "wikipedia1"
fi

if [[ ! -d "wikipedia-depth-1" ]];
then
    cp -r ~cs50/data/tse-output/wikipedia-depth-1 .
    echo "getting wikipedia-depth-1"
fi

if [[ ! -d "testDir" && ! -w "testDir" ]];
then
    mkdir "testDir"
    chmod u+w "testDir"
fi



#### INVALID ARGUMENTS ####
# invalid # args
./crawler

# invalid seed
./crawler http://cs.dartmouth.edu testDir 0

# invalid directory
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html notTestDir 0

# invalid maxDepth
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html testDir 100


#### FUNCTIONALITY/ACCURACY ####
# letters to depth 4
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html letters4 4

# toscrape to depth 1
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html toScrape1 1

# wikipedia to depth 1
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/index.html wikipedia1 1

# check differences 
if diff letters-depth-4 letters4
then   
    echo "letters passed"
else
    echo "letters failed"
fi

if diff toscrape-depth-1 toScrape1
then   
    echo "toscrape passed"
else
    echo "toscrape failed"
fi

if diff wikipedia-depth-1 wikipedia1
then   
    echo "wikipedia passed"
else
    echo "wikipedia failed"
fi
