# Makefile for querier
# Garrett Johnston CS50 20X

	
PROG = querier
OBJS = querier.o
LIBS = ../common/common.a ../libcs50/libcs50.a  -lm 

CC = gcc
FLAGS = -I../common -I../libcs50
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)


$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(PROG)

querier.o: 


.PHONY: clean test

test:
	bash -v testing.sh

clean:
	rm -f $(PROG)
	rm -f *~ *.o 
