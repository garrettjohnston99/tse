# Makefile for common
# Garrett Johnston CS50 20X

OBJS = pagedir.o 
LIB = common.a

FLAGS = 

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc -lm

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h


.PHONY: clean

clean:
	rm -f core
	rm -f $(LIB) *~ *.o