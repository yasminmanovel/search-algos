/* readData.h
 *
 * Written by Selina (z5208109) & Yasmin (z5207093)
 * Group: duckduckgo
 * Start Date: 10/10/18
 * 
 * Description:
 * Header file for readData.c.
 */

#include "graph.h"
#include "set.h"
#include "BSTree.h"

Set getCollection();

Graph getGraph(Set URLList);

BSTree getInvertedList(Set URLList);

