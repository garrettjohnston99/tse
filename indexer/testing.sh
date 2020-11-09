# bash test script for ./indexer
# uses diff and indices in ~cs50/data/tse-output to verify correctness of indexer.c
# run by make test as bash -v testing.sh &> testing.out
# Garrett Johnston CS50 20X


# Make a directory of directories(created by crawler) to index
# if testDir already exists, assumed to have directories in it
if [[ ! -d "testDir" ]]; then
    # make directoryy

    mkdir "testDir"
    chmod u+rw "testDir"

    # letters to depth 4
    echo "getting letters-depth-4 and its index"
    cp -r ~cs50/data/tse-output/letters-depth-4/ ./testDir
    cp ~cs50/data/tse-output/letters-index-4 ./testDir

    # toscrape to depth 1
    echo "getting toscrape-depth-1 and its index"
    cp -r ~cs50/data/tse-output/toscrape-depth-1 ./testDir
    cp ~cs50/data/tse-output/toscrape-index-1 ./testDir

    # wikipedia to depth 1
    echo "getting wikipedia-depth-1 and its index"
    cp -r ~cs50/data/tse-output/wikipedia-depth-1 ./testDir
    cp ~cs50/data/tse-output/wikipedia-index-1 ./testDir
fi


#### INVALID ARGUMENTS ####
# invalid # args
./indexer

# invalid directory
./indexer ../doesntExist doesntmatter

# valid directory, but not created by crawler
./indexer ../common doesntmatter


#### FUNCTIONALITY/ACCURACY ####
# letters to depth 4
./indexer ./testDir/letters-depth-4 ./testDir/my-letters-index-4

# toscrape to depth 1
./indexer ./testDir/toscrape-depth-1 ./testDir/my-toscrape-index-1

# wikipedia to depth 1
./indexer ./testDir/wikipedia-depth-1 ./testDir/my-wikipedia-index-1


# check differences
if diff ./testDir/letters-index-4 ./testDir/my-letters-index-4; then
    echo "letters passed"
else
    echo "letters failed"
fi

if diff ./testDir/toscrape-index-1 ./testDir/my-toscrape-index-1; then
    echo "toscrape passed"
else
    echo "toscrape failed"
fi

if diff ./testDir/wikipedia-index-1 ./testDir/my-wikipedia-index-1; then
    echo "wikipedia passed"
else
    echo "wikipedia failed"
fi

# clean up
echo "removing testDir"
rm -r testDir
