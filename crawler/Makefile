# Makefile for crawler
# Garrett Johnston CS50 20X


PROG = crawler
OBJS = crawler.o 
LIBS = ../libcs50/libcs50-given.a ../common/common.a -lm

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../common -I../libcs50

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(PROG)


crawler.o: ../libcs50/webpage.h ../libcs50/memory.h 

.PHONY: clean test 

test:
	bash -v testing.sh

clean:
	rm -f $(PROG)
	rm -f *~ *.o 
	rm -f testDir/*
