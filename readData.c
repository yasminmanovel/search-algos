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
#define URL_LENGTH 55

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

/* Removes trailing spaces and punctuation at the end of word
 * Also converts all letters to lowercase.
 */
static char *normalise(char *str) {
	char *word = strdup(str);
	trim(word);
	// Converts to all lowercase.
	int i;
	for (i = 0; word[i] != '\0'; i++) 
		word[i] = tolower(word[i]);
	// Removes punctuation at the end.
	int lastLetter = word[strlen(word) - 1];
	if (word[lastLetter] == '.'
	 || word[lastLetter] == '?'
	 || word[lastLetter] == ','
	 || word[lastLetter] == ';') word[lastLetter] = '\0';
	
	return word;
}

/*
GetInvertedList(List_of_Urls)
		Create empty inverted list (use say List of lists, BST where values are lists, etc)
		For each url in List_of_Urls
			- Read <url<.txt file, and update inverted index
*/
BSTree getInvertedList(Set URLList)
{
	BSTree invList = newBSTree();
	// Iterate through set.
	Link curr = URLList->elems;
	char fileName[URL_LENGTH] = {0};
	while (curr != NULL) {
		sprintf(fileName, "%s.txt", curr->val);
		// Update inverted index.
		char *urls = NULL, *text = NULL;
		readPage(urls, text, fileName);
		// For every word, add node to tree or add url to urlList.
		char *dump = text;
		char *found;
		while((found = strsep(&text, " ")) != NULL) {
			char *word = normalise(found);
			BSTreeInsert(invList, word, curr->val);
		}
		free(dump);
	}

	return invList;
}

// int main(int argc, char **argv) {
// 	Set URLList = getCollection();
// 	showSet(URLList);
// 	BSTree t;
// 	t = newBSTree();
// 	t = BSTreeInsert(t, "hello");
// 	t = BSTreeInsert(t, "abc");
// 	t = BSTreeInsert(t, "bcd");
// 	t = BSTreeInsert(t, "zeas");
// 	t = BSTreeInsert(t, "mother");
// 	BSTreeInfix(t);
// 	printf("\n");
// 	return 0;
// }