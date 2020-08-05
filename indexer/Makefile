# Makefile for indexer
# Garrett Johnston CS50 20X

	
PROG1 = indexer
PROG2 = indextest
OBJS1 = indexer.o 
OBJS2 = indextest.o 
LIBS = ../common/common.a ../libcs50/libcs50-given.a -lm

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../common -I../libcs50

.PHONY: clean test all

all: $(PROG1) $(PROG2)

test:
	bash -v testing.sh

clean:
	rm -f $(PROG1)
	rm -f $(PROG2)
	rm -f *~ *.o 

$(PROG1): $(OBJS1)
	$(CC) $(CFLAGS) $(OBJS1) $(LIBS) -o $(PROG1)

$(PROG2): $(OBJS2)
	$(CC) $(CFLAGS) $(OBJS2) $(LIBS) -o $(PROG2)


indextest.o:
indexer.o: 




