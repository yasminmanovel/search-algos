# -*- Makefile -*-
CC=gcc
CFLAGS=-Wall -Werror

searchTfIdf : searchTfIdf.o readData.o set.o graph.o BSTree.o
	gcc searchTfIdf.o readData.o set.o graph.o BSTree.o -o searchTfIdf

invertedIndex : invertedIndex.o readData.o graph.o set.o BSTree.o
	gcc invertedIndex.o readData.o graph.o set.o BSTree.o -o invertedIndex

readData : readData.o graph.o set.o BSTree.o
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
