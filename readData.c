/* readData.c
 *
 * Written by Selina (z5208109) & Yasmin (z5207093)
 * Group: duckduckgo
 * Start Date: 10/10/18
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "graph.h"
#include "set.h"
#include "BSTree.h"
#include "invertedIndex.h"

#define SEEN   0
#define UNSEEN 1
#define URL_LENGTH 6

/* Trims leading and ending spaces 
 * Written by jas for 1521 mymysh.c
 */
static void trim(char *str) {
	int first, last;
	first = 0;
	while (isspace(str[first])) first++;
	last  = strlen(str)-1;
	while (isspace(str[last])) last--;
	int i, j = 0;
	for (i = first; i <= last; i++) str[j++] = str[i];
	str[j] = '\0';
}

/* Creates a set of all URLs in collection.txt. */
Set getCollection()
{
	FILE *file = fopen("collection.txt", "r");
	if (!file) { perror("fopen failed"); exit(EXIT_FAILURE); }

	// Gets every URL and adds it to set.
	Set URLList = newSet();
	char URL[URL_LENGTH];
	while (fscanf(file, "%s ", URL) != EOF) {
		trim(URL);
		insertInto(URLList, URL);
	}
	return URLList;
}

/*
Functions we need to have:
	getGraph(List_of_Urls)
		Returns Graph g
		Create empty graph (use graph ADT in say graph.h and graph.c)
		For each url in the above list
			- Read <url>.txt file, and update graph by adding a node and outgoing links
*/
// Graph getGraph(set listOfUrls)
// {
// 	graph g = newGraph();
// 	// go through listOfUrls and update the graph by adding a node and outgoing links
// 	return g;
// }

/*
GetInvertedList(List_of_Urls)
		Create empty inverted list (use say List of lists, BST where values are lists, etc)
		For each url in List_of_Urls
			- Read <url<.txt file, and update inverted index
*/
BSTree getInvertedList(Set URLList)
{
	// BSTree invList = newBSTree();
	// Iterate through set.
	Link curr = URLList->elems;
	while (curr != NULL) {
		// Open URL.
		// Read url.txt file.
		// Update inverted index.
	}

	return NULL;
}

int main(int argc, char **argv) {
	Set URLList = getCollection();
	showSet(URLList);
	BSTree t;
	t = newBSTree();
	t = BSTreeInsert(t, "hello");
	t = BSTreeInsert(t, "abc");
	t = BSTreeInsert(t, "bcd");
	t = BSTreeInsert(t, "zeas");
	t = BSTreeInsert(t, "mother");
	BSTreeInfix(t);
	printf("\n");
	return 0;
}