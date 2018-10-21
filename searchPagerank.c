/* searchPagerank.c
 *
 * Written by Selina (z5208109) & Yasmin (z5207093)
 * Group: duckduckgo
 * Start Date: 10/10/18
 */

/* PSEUDOCODE:
 * Get query words from arguments
 * matchedURLList = findMatchedURLs("invertedIndex.txt", queryWords);
 * matchedURLswithPR = findPagerank("pagerankList.txt", matchedURLList);
 * 
 * Output -> ordered URLs to stdout.
 */

/*
Get query words from arguments
Need these functions:
	findMatchedUrls("invertedIndex.txt", queryWords)
		return matched_Url_list
	findPagerank("pagerankList.txt", matched_Url_list)
		return matched_Urls_with_PR
Output order urls on stdout

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "set.h"
#include "graph.h"
#include "BSTree.h"
#include "readData.h"

#define MAX_LINE 1001
#define URL_LENGTH 55

typedef struct url *urlPR;

struct url {
	int search_terms;
	float page_rank;
	char *URL;
};


/* Gets the URLs that contain word. */
char **getURLs(char *word) 
{
    FILE *invIndex = fopen("invertedIndex.txt", "r");
    if (!invIndex) { perror("fopen failed"); exit(EXIT_FAILURE); }

    char line[MAX_LINE] = {0};
    char lineWord[MAX_LINE] = {0};
    char *urlString = NULL;
    char **urls = NULL;
    while (fgets(line, MAX_LINE, invIndex) != NULL) {
        sscanf(line, "%s", lineWord);
        // Finds the wanted word.
        if (strcmp(lineWord, word) == 0) {
            urlString = line + strlen(word); // Moves pointer to urls part.
            trim(urlString);
            // Get an array of url names.
            urls = tokenise(urlString, " ");
            break;
        }
    }
    return urls;
}


void getPageRanks(int elems, urlPR *searchPR) 
{
    FILE *pagerankList = fopen("pagerankList.txt", "r");
    if (!pagerankList) { perror("fopen failed"); exit(EXIT_FAILURE); }
    char line[MAX_LINE] = {0};
	searchPR = malloc(sizeof(struct url) * elems);
	int i = 0;
	int links;
    while (fgets(line, MAX_LINE, pagerankList) != NULL) {
		searchPR[i]->URL = malloc(URL_LENGTH);
        sscanf(line, "%s, %d, %f", searchPR[i]->URL, links, searchPR[i]->page_rank);
        // Finds the wanted word.
		i++;
    }
}


int main(int argc, char **argv)
{
	char **URLs;
	int i;
	Set WordsSearched;
	urlPR *searchPR;
	Set URLList = getCollection();
	int elems = nElems(URLList);
	getPageRanks(elems, searchPR);
	//Graph g = getGraph(URLList);
	for (i = 1; i < argc; i++) {
		insertInto(WordsSearched, argv[i]);
		getURLs(argv[i]);
	}
	// sort
	// print
}
