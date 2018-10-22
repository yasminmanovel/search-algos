/*
 * 
 * NOTES:
 * - Use adjacency list.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "graph.h"

#define NULL_TERM 1

// creating the nodes that represent outlinks & inlinks
struct urlLink *newInLink(URL PointTo)
{
    struct urlLink *newLink = calloc(1, sizeof(struct urlLink));
    newLink->URLName = strdup(PointTo->URLName);
    newLink->URLPointer = PointTo;
    newLink->next = NULL;
    return newLink;
}


struct urlLink *newOutLink(char *URLName)
{
    struct urlLink *newLink = calloc(1, sizeof(struct urlLink));
    newLink->URLName = strdup(URLName);
    newLink->URLPointer = NULL;
    newLink->next = NULL;
    return newLink;
}

// creating the nodes that represent urls
struct urlNode *newGraphNode(char *urlNum, char *text)
{
    struct urlNode *newURL = calloc(1, sizeof(struct urlNode));
    newURL->URLName = strdup(urlNum);
    newURL->numOutLinks = 0; newURL->numInLinks = 0;
    newURL->text = strdup(text);
    newURL->inLink = NULL; newURL->outLink = NULL;
    return newURL;
}

// create a new Graph
struct urlGraph *newGraph()
{
    struct urlGraph *newGraph = calloc(1, sizeof(struct urlGraph));
    newGraph->numURLs = 0;
    newGraph->listOfUrls = NULL;
    return newGraph;
}

// inserting outlinks for a URL
void insertOutLinks(URL URLNode, char *URLName)
{
    if (URLNode->outLink == NULL) {
        URLNode->outLink = newOutLink(URLName);
    } else {
        Link curr = URLNode->outLink;
        while (curr->next != NULL) curr = curr->next;
        curr->next = newOutLink(URLName);
    }
}

// inserting inlinks for a URL
void insertInLinks(URL URLNode, URL URLPointer)
{
    if (URLNode->inLink == NULL) {
        URLNode->inLink = newInLink(URLPointer);
    } else {
        Link curr = URLNode->inLink;
        while (curr->next != NULL) curr = curr->next;
        curr->next = newInLink(URLPointer);
    }
}