/*
 * 
 * NOTES:
 * - Use adjacency list.
 */
#include "set.h"
#include <stdio.h>
#include <string.h>
#include "graph.h"




struct urlLink {
    char *URLName;
    outLink next;
};

struct urlNode {
    char *URLName;
    int numEdges;
    char *text;
    outLink firstLink;
    URL next;
};

struct urlGraph {
    int numURLs;
    URL *listOfUrls;
};


struct urlLink *newGraphLinks(char *urlNum)
{
    struct urlLink *newLink;
    newLink->URLName = malloc(strlen(urlNum)+1);
    newLink->URLName = strdup(urlNum);
    newLink->next = NULL;
    return newLink;
}

struct urlNode *newGraphNode(char *urlNum, char *text)
{
    struct urlNode *newURL;
    newURL->URLName = malloc(strlen(urlNum)+1);
    newURL->URLName = strdup(urlNum);
    newURL->numEdges = 0;
    newURL->text = malloc(strlen(text)+1);
    newURL->text = strdup(text);
    newURL->firstLink = NULL;
    newURL->next = NULL;
    return newURL;
}

struct urlGraph *newGraph()
{
    struct urlGraph *newGraph;
    newGraph->numURLs = 0;
    newGraph->listOfUrls = NULL;
    return newGraph;
}

void insertIntoGraph(Graph g, char *name)
{
}


/*
void insertInto(Set s, char *str)
{
	assert(s != NULL);
	Link curr, prev;
	int found = findNode(s->elems,str,&curr,&prev);
	if (found) return; // already in Set
	Link new = newNode(str);
	s->nelems++;
	if (prev == NULL) {
		// add at start of list of elems
		new->next = s->elems;
		s->elems = new;
	}
	else {
		// add into list of elems
		new->next = prev->next;
		prev->next = new;
	}
}*/