/*
 * 
 * NOTES:
 * - Use adjacency list.
 */
#include "set.h"
#include <stdio.h>
#include <string.h>


typedef struct urlNode *URL;
typedef struct urlLink *outLink;
typedef struct urlGraph *Graph;

struct urlLink {
    char *URLName;
    outLink next;
};

struct urlNode {
    char *URLName;
    int numEdges;
    char *text;
    outLink first;
    URL next;
};

struct urlGraph {
    int numURLs;
    URL *listOfUrls;
};


outLink newLinks(char *urlNum)
{
    outLink newLink;
    newLink->URLName = malloc(strlen(urlNum)+1);
    newLink->URLName = strdup(urlNum);
    newLink->next = NULL;
    return newLink;
}

URL newNode(char *urlNum)
{g
    URL newURL;
    newURL->URLName;
    newURL->numEdges;
    newURL->text;
    newURL->first;
    newURL->next;
}

Graph newGraph(Set listOfUrl)
{
    Graph newGraph;
    newGraph->numURLs = NULL;
    newGraph->numURLs = 0;
    return newGraph;
}