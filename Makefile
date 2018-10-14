# -*- Makefile -*-
CC=gcc
CFLAGS=-Wall -Werror

invertedIndex : invertedIndex.o readData.o graph.o set.o BSTree.o
	gcc invertedIndex.o readData.o graph.o set.o BSTree.o -o invertedIndex

readData : readData.o graph.o set.o BSTree.o
	gcc readData.o graph.o set.o BSTree.o -o readData

readData.o : readData.c
	gcc -c readData.c

graph.o : graph.c
	gcc -c graph.c

set.o : set.c
	gcc -c set.c

BSTree.o : BSTree.c
	gcc -c BSTree.c
