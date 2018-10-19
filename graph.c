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
struct urlLink *newGraphLinks(char *urlNum)
{
    struct urlLink *newLink = calloc(1, sizeof(struct urlLink));
    newLink->URLName = malloc(strlen(urlNum)+NULL_TERM);
    newLink->URLName = strdup(urlNum);
    newLink->next = NULL;
    return newLink;
}

// creating the nodes that represent urls
struct urlNode *newGraphNode(char *urlNum, char *text)
{
    struct urlNode *newURL = calloc(1, sizeof(struct urlNode));
    newURL->URLName = malloc(strlen(urlNum)+NULL_TERM);
    newURL->URLName = strdup(urlNum);
    newURL->numOutLinks = 0;newURL->numInLinks = 0;
    newURL->text = malloc(strlen(text)+NULL_TERM);
    newURL->text = strdup(text);
    newURL->inLink = NULL;
    newURL->outLink = NULL;
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
void insertOutLinks(URL URLNode, char *URL)
{
    if (URLNode->outLink == NULL) {
        URLNode->outLink = newGraphLinks(URL);
    } else {
        Link curr = URLNode->outLink;
        while (curr->next != NULL) curr = curr->next;
        curr->next = newGraphLinks(URL);
    }
}

// inserting inlinks for a URL
void insertInLinks(URL URLNode, char *URL)
{
    if (URLNode->inLink == NULL) {
        URLNode->inLink = newGraphLinks(URL);
    } else {
        Link curr = URLNode->inLink;
        while (curr->next != NULL) curr = curr->next;
        curr->next = newGraphLinks(URL);
    }
}