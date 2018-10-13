#ifndef GRAPH_H
#define GRAPH_H

typedef struct urlNode *URL;
typedef struct urlLink *outLink;
typedef struct urlGraph *Graph;


outLink newGraphLinks(char *);
URL newGraphNode(char *, char *);
Graph newGraph();
void insertIntoGraph(Graph, char*);


#endif
