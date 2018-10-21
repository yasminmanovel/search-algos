#ifndef GRAPH_H
#define GRAPH_H

typedef struct urlNode *URL;
typedef struct urlLink *Link;
typedef struct urlGraph *Graph;

struct urlLink {
    char *URLName;
    URL URLPointer;
    Link next;
};

struct urlNode {
    char *URLName;
    int numOutLinks;
    int numInLinks;
    char *text;
    Link inLink;
    Link outLink;
};

struct urlGraph {
    int numURLs;
    URL *listOfUrls;
};

Link newInLink(URL);
Link newOutLink(char *);
URL newGraphNode(char *, char *);
Graph newGraph();
void insertIntoGraph(Graph, char*);
void insertOutLinks(URL, char *);
void insertInLinks(URL, URL);

#endif