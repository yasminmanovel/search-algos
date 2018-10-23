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

// Creates a new inlink given a URLNode.
Link newInLink(URL);
// Creats a new outlink based on a char *.
Link newOutLink(char *);
// Creates a new graph node.
URL newGraphNode(char *, char *);
// Creates a new empty graph.
Graph newGraph();
// Inserts a new char * into the graph.
void insertIntoGraph(Graph, char*);
// Inserts outlinks with URLName.
void insertOutLinks(URL, char *);
// Inserts inlinks which contains a pointer to nodes.
void insertInLinks(URL, URL);
// Returns the head of listOfUrls in a Graph.
URL *listOfUrls(Graph);
// Returns num of URLs in a graph.
int numURLs(Graph);

#endif