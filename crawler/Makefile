# Makefile for crawler
# Garrett Johnston CS50 20X


PROG = crawler
OBJS = crawler.o 
LIBS = ../libcs50/libcs50.a ../common/common.a -lm

CC = gcc
FLAGS = -I../common -I../libcs50
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(PROG)


crawler.o: ../libcs50/webpage.h

.PHONY: clean test 

test:
	bash -v testing.sh

clean:
	rm -f $(PROG)
	rm -f *~ *.o 
	rm -f testDir/*
