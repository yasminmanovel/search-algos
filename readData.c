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
#include "graph.c"
#include "set.h"
#include "BSTree.h"
#include "invertedIndex.h"
#include "set.c"
// #include "invertedIndex.h"
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
#define SEEN_ONCE       1
#define SEEN_TWICE      2
#define URL_LENGTH      55
#define MAX_LINE        1001
#define NULL_TERM_SPACE 1
#define START_TAG_LEN   16
#define END_TAG_LEN     14

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

/* Places section 1 and section 2 of fileName into urls & texts */
void readPage(char *urls, char *text, char *fileName) {
	int seen = 0;
	char line[MAX_LINE] = {0};
	FILE *page = fopen(fileName, "r");
	while (fgets(line, MAX_LINE, page) != NULL) {
		// Increments seen at every start tag.
		if (strncmp(line, "#start Section-1", START_TAG_LEN) == 0 
		|| strncmp(line, "#start Section-2", START_TAG_LEN) == 0) { seen++; continue; }
		// Ignores end tags and nwln.
		if (strncmp(line, "#end Section-1", END_TAG_LEN) == 0
		|| strncmp(line, "#end Section-2", END_TAG_LEN) == 0
		|| strncmp(line, "\n", 1) == 0) continue;

		if (seen == SEEN_ONCE) { strcat(urls, line); }
		if (seen == SEEN_TWICE) { strcat(text, line); }
	}
	// Changes '\n's into space.
	for (int i = 0; text[i] != '\0'; i++)
		if (text[i] == '\n') text[i] = ' ';

	fclose(page);
}

/* Calculates space required for section 1 and 2 */
static void spaceRequired(char *fileName, int *url_size, int *text_size)
{
	int seen = 0;
	char line[MAX_LINE] = {0};
	FILE *page = fopen(fileName, "r");
	*url_size = NULL_TERM_SPACE;
	*text_size = NULL_TERM_SPACE;
	while (fgets(line, MAX_LINE, page) != NULL) {
		if (strncmp(line, "#start Section-1", 16) == 0 
		|| strncmp(line, "#start Section-2", 16) == 0) { seen++; continue; }

		if (strncmp(line, "#end Section-1", 14) == 0
		|| strncmp(line, "#end Section-2", 14) == 0
		|| strncmp(line, "\n", 1) == 0) continue;
		if (seen == SEEN_ONCE) *url_size = *url_size + strlen(line);
		if (seen == SEEN_TWICE) *text_size = *text_size + strlen(line);
	}

	fclose(page);
}

/* Creates a graph of URLs. */
Graph getGraph(Set URLList)
{
	Graph g = newGraph();
	char fileName[URL_LENGTH] = {0};
	for (Link curr = URLList->elems; curr != NULL; curr = curr->next) {
		strcpy(fileName, curr->val);
		strcat(fileName, ".txt");
		int url_size; int text_size;
		spaceRequired(fileName, &url_size, &text_size);
		char *urls = calloc(url_size, sizeof(char));
		char *text = calloc(text_size, sizeof(char));
		readPage(urls, text, fileName);
		//insertIntoGraph(g, curr->val);
	}
	// go through listOfUrls and update the graph by adding a node and outgoing links
	return g;
}

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

/* Creates a list of url for each word found in urls. */
BSTree getInvertedList(Set URLList)
{
	BSTree invList = newBSTree();
	// Iterate through set to get urls.
	Link curr = URLList->elems;
	char fileName[URL_LENGTH] = {0};
	while (curr != NULL) {
		sprintf(fileName, "%s.txt", curr->val);
		// printf("filename = %s\n", fileName);
		// Gets information from txt file.
		int url_size; int text_size;
		spaceRequired(fileName, &url_size, &text_size);
		char *urls = calloc(url_size, sizeof(char));
		char *text = calloc(text_size, sizeof(char));
		readPage(urls, text, fileName);
		// Update inverted index.
		char *dump = text; // Keeps pointer to text to free.
		char *found;
		while((found = strsep(&text, " ")) != NULL) {
			char *word = normalise(found);
			if (strcmp(word, "") != 0) {
				invList = BSTreeInsert(invList, word, curr->val);
			}
		}
		free(dump);
		curr = curr->next;
	}

	return invList;
}


// int main(int argc, char **argv) {
// 	Set URLList = getCollection();
// 	showSet(URLList);
// 	BSTree invList = getInvertedList(URLList);
// 	BSTreeInfix(invList);
// 	printf("\n");
// 	return 0;
// }