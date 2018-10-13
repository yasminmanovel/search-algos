/* readData.c
 *
 * Written by Selina (z5208109) & Yasmin (z5207093)
 * Group: duckduckgo
 * Start Date: 10/10/18
 */

#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "invertedIndex.h"

/*
Functions we need to have:
	getCollection()
		returns List_of_Urls
		Create a set (list) of urls to process by reading data from file "collection.txt"
	getGraph(List_of_Urls)
		Returns Graph g
		Create empty graph (use graph ADT in say graph.h and graph.c)
		For each url in the above list
			- Read <url>.txt file, and update graph by adding a node and outgoing links
	GetInvertedList(List_of_Urls)
		Create empty inverted list (use say List of lists, BST where values are lists, etc)
		For each url in List_of_Urls
			- Read <url<.txt file, and update inverted index	

*/

/* Goes through collection.txt and returns the set of all urls.
 */
Set getCollection() {

}

/* Creates a graph with all the urls as a set.
 */
Graph getGraph() {
	/*
	PSEUDOCODE:
	Create empty graph (use graph ADT in say graph.h and graph.c)
	For each url in the above list
		Read <url>.txt file, and update graph by adding a node and outgoing links
	Returns Graph g
	*/
}


<<<<<<< HEAD
=======
*/

#define SEEN 0
#define UNSEEN 1

set getCollection()
{
	FILE *file = fopen("collection.txt", "r");
	char *collection;
	int fscanf(file, collection);
	for (char *token = strtok(collection, " "); token != NULL; token = strtok(NULL, " ")) {
		// put the token in the set
	}
	
	free(collection);
}

graph getGraph(set listOfUrls)
{
	graph g = newGraph();
	// go through listOfUrls and update the graph by adding a node and outgoing links
	return g;
}

list getInvertedList(set listOfUrls)
{
}
>>>>>>> 49fa7796e8e3303cecec55e53024faa8cb59dc5e
