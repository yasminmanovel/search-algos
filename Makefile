# -*- Makefile -*-
CC=gcc
CFLAGS=-Wall -Werror -g
OBJS=set.o graph.o BSTree.o readData.o

searchTfIdf : searchTfIdf.o $(OBJS)
	gcc $(CFLAGS) searchTfIdf.o $(OBJS) -o searchTfIdf

pagerank: pagerank.o readData.o graph.o set.o BSTree.o
	gcc $(CFLAGS) pagerank.o readData.o graph.o set.o BSTree.o -o pagerank

invertedIndex : invertedIndex.o $(OBJS)
	gcc $(CFLAGS) invertedIndex.o $(OBJS) -o invertedIndex

readData : $(OBJS)
	gcc $(CFLAGS) readData.o graph.o set.o BSTree.o -o readData

pagerank.o : pagerank.c 
	gcc $(CFLAGS) -c pagerank.c 

searchTfIdf.o : searchTfIdf.c 
	gcc $(CFLAGS) -c searchTfIdf.c 

invertedIndex.o : invertedIndex.c 
	gcc $(CFLAGS) -c invertedIndex.c 

readData.o : readData.c
	gcc $(CFLAGS) -c readData.c

graph.o : graph.c
	gcc $(CFLAGS) -c graph.c

set.o : set.c
	gcc $(CFLAGS) -c set.c

BSTree.o : BSTree.c
	gcc $(CFLAGS) -c BSTree.c

clean:
	rm -f $(OBJS) searchTfIdf.o invertedIndex.o
