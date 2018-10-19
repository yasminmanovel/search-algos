// set.h ... interface to simple Set of Strings
// Written by John Shepherd, September 2015

#ifndef SET_H
#define SET_H

typedef struct Node *SetNode;

typedef struct Node {
	char *val;
	SetNode  next;
} Node;
	
typedef struct SetRep {
	int      nelems;
	SetNode  elems;
} SetRep;

typedef struct SetRep *Set;

// Function signatures

Set newSet();
void disposeSet(Set);
void insertInto(Set,char *);
void dropFrom(Set,char *);
int  isElem(Set,char *);
int  nElems(Set);
void showSet(Set);

#endif
