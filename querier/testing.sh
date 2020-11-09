# bash test script for TSE querier
# assumes there exists a directory "queryTest" containing all the necessary files 
# to run functionality tests
# Garrett Johnston CS50 20X


# Make a directory with crawlerDirectory & index if we need to 
# assumes if "testDir" exists already it has these two files
if [[ ! -d "testDir" ]]; then
    # make directory w/ toscrape depth 1 - lots of words & files

    mkdir "testDir"
    chmod u+rw "testDir"

    # toscrape to depth 1
    echo "getting toscrape-depth-1 and its index"
    cp -r ~cs50/data/tse-output/toscrape-depth-1 ./testDir
    cp ~cs50/data/tse-output/toscrape-index-1 ./testDir

fi


#### INVALID ARGUMENTS ####
# invalid # args
./querier

# invalid crawler directory
./querier ../common doesntmatter

# if indexFile exists, it's assumed to be written by indexer
# non-existant file
./querier ./testDir/toscrape-depth-1 doesntexist


#### FUNCTIONALITY ####
# assortment of invalid queries
./querier ./testDir/toscrape-depth-1 ./testDir/toscrape-index-1 < ./queryTest/invalids

# For each testfile(singletons, simpleOr, simpleAnd)
# I've created an index of expected scores based on BNF and the index
# with format docID score [docID score ...] So that results from the tests are easily verified

# singleton queries from the top 50 of the index file - should be easily validated by eye in scores_singletons
./querier ./testDir/toscrape-depth-1 ./testDir/toscrape-index-1 < ./queryTest/queries_singletons

# queries w/ "or" operator - only have 2 words, ie a or b. Should still be easily validated by eye by sum in scores_simpleOr
./querier ./testDir/toscrape-depth-1 ./testDir/toscrape-index-1 < ./queryTest/queries_simpleOr

# queries w/ "and" operator - only have 2 words, ie a and be. Same words as simpleOr - easily validated by eye in scores_simpleAnd
./querier ./testDir/toscrape-depth-1 ./testDir/toscrape-index-1 < ./queryTest/queries_simpleAnd

# queries that when queried in the reverse order should be the same, as operators are associative. Should be the same, if in different order
# In pairs of 2. Should be easily validated by eye in scores_equivalents
./querier ./testDir/toscrape-depth-1 ./testDir/toscrape-index-1 < ./queryTest/queries_equivalents

# clean up testDir
rm -r testDir
