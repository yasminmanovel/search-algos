# -*- Makefile -*-
CC=gcc
CFLAGS=-Wall -Werror -g
OBJS=set.o graph.o BSTree.o readData.o

searchTfIdf : searchTfIdf.o $(OBJS)
	gcc searchTfIdf.o $(OBJS) -o searchTfIdf

invertedIndex : invertedIndex.o $(OBJS)
	gcc invertedIndex.o $(OBJS) -o invertedIndex

readData : $(OBJS)
	gcc readData.o graph.o set.o BSTree.o -o readData

searchTfIdf.o : searchTfIdf.c 
	gcc -c searchTfIdf.c 

invertedIndex.o : invertedIndex.c 
	gcc -c invertedIndex.c 

readData.o : readData.c
	gcc -c readData.c

graph.o : graph.c
	gcc -c graph.c

set.o : set.c
	gcc -c set.c

BSTree.o : BSTree.c
	gcc -c BSTree.c

clean:
	rm -f $(OBJS) searchTfIdf.o invertedIndex.o
