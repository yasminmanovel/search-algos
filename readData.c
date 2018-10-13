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
#define SEEN_ONCE 1
#define SEEN_TWICE 2
#define URL_LENGTH 55
#define MAX_LINE 1001
#define NULL_TERM_SPACE 1

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

void readPage(char *urls, char *text, char *fileName) {
	int seen = 0;
	char line[MAX_LINE] = {0};
	FILE *page = fopen(fileName, "r");
	while (fgets(line, MAX_LINE, page) != NULL) {
		if (strncmp(line, "#start Section-1", 16) == 0 
		|| strncmp(line, "#start Section-2", 16) == 0){ seen++; continue;}

		if (strncmp(line, "#end Section-1", 14) == 0
		|| strncmp(line, "#end Section-2", 14) == 0
		|| strncmp(line, "\n", 1) == 0) continue;
		if (seen == SEEN_ONCE) {strcat(urls, line);}
		if (seen == SEEN_TWICE) {strcat(text, line);}
	}
	for (int i = 0; text[i] != '\0'; i++) {
		if (text[i] == '\n') text[i] = ' ';
	}
	fclose(page);
}

static void spaceRequired(char *fileName, int *url_size, int *text_size)
{
	int seen = 0;
	char line[MAX_LINE] = {0};
	FILE *page = fopen(fileName, "r");
	*url_size = NULL_TERM_SPACE;
	*text_size = NULL_TERM_SPACE;
	while (fgets(line, MAX_LINE, page) != NULL) {
		if (strncmp(line, "#start Section-1", 16) == 0 
		|| strncmp(line, "#start Section-2", 16) == 0){ seen++; continue;}

		if (strncmp(line, "#end Section-1", 14) == 0
		|| strncmp(line, "#end Section-2", 14) == 0
		|| strncmp(line, "\n", 1) == 0) continue;
		if (seen == SEEN_ONCE) *url_size = *url_size + strlen(line);
		if (seen == SEEN_TWICE) *text_size = *text_size + strlen(line);
	}
	fclose(page);
}

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

int main(void) {
	return 0;
}

// int main(void)
// {
// 	int url_size; int text_size;
// 	spaceRequired("url11.txt", &url_size, &text_size);
// 	char *urls = calloc(url_size, sizeof(char));
// 	char *text = calloc(text_size, sizeof(char));
// 	readPage(urls, text, "url11.txt");
// 	printf("FINAL URLS: %s\nFINAL TEXT: %s\n", urls, text);
// }

// list getInvertedList(set listOfUrls)
// {
// }

// int main(int argc, char **argv) {
// 	Set URLList = getCollection();
// 	showSet(URLList);
// 	return 0;
// }
