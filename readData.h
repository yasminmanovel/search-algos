/* readData.h
 *
 * Written by Selina (z5208109) & Yasmin (z5207093)
 * Group: duckduckgo
 * Start Date: 10/10/18
 *; * Description:
 * Header file for readData.c.
 */

#include "set.h"
#include "graph.h"
#include "BSTree.h"

#ifndef READDATA_H
#define READDATA_H

void trim(char *str);
char **tokenise(char *str, char *sep);
char *normalise(char *str);
Set getCollection();
void readPage(char *urls, char *text, char *fileName);
void spaceRequired(char *fileName, int *url_size, int *text_size);
BSTree getInvertedList(Set URLList);
Graph getGraph(Set URLList);
void freeTokens(char **toks);
void freeLinks(Link head);
void freeGraph(Graph g);
int alreadyInserted(char *outLinkName, URL node);
int linkAlreadyExists(Link start, char *name);
#endif