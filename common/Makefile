# Makefile for common
# Garrett Johnston CS50 20X

OBJS = index.o pagedir.o word.o 
LIB = common.a
LIBS = ../libcs50/libcs50-given.a -lm

FLAGS = 

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) 
CC = gcc

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h 
word.o: word.h 
index.o: index.h 

.PHONY: clean

clean:
	rm -f core
	rm -f $(LIB) *~ *.o