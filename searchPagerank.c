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
	int searchTerms;
	float pageRank;
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



urlPR newSearchPRNode() {
    urlPR newSearchNode = calloc(1, sizeof(struct url));
    newSearchNode->URL = calloc(URL_LENGTH, sizeof(char));
    newSearchNode->pageRank = 0;
    newSearchNode->searchTerms = 0;
    return newSearchNode;
}


void countOccurences(char **URLs, urlPR *searchPR, int elems)
{
	int j;
	for (j = 0; URLs[j] != NULL; j++) {
		printf("This URL: %s\n", URLs[j]);
	}
	int i;
	for (i = 0; i < elems; i++) {
		int j;
		for (j = 0; URLs[j] != NULL; j++) {
			if (strcmp(searchPR[i]->URL, URLs[j]) == 0) searchPR[i]->searchTerms++;
		}
	}
}


urlPR *getPageRanks(int *elems) 
{
	printf("in page rank\n");
    FILE *pagerankList = fopen("pagerankList.txt", "r");
    if (!pagerankList) { perror("fopen failed"); exit(EXIT_FAILURE); }
    char line[MAX_LINE] = {0};
	int links;
	urlPR *searchPR = malloc(sizeof(struct url) * *elems);
	int i = 0;
    while (fgets(line, MAX_LINE, pagerankList) != NULL) {
		searchPR[i] = newSearchPRNode();
		printf("%s\n", line);
        sscanf(line, "%s %d, %f", searchPR[i]->URL, &links, &searchPR[i]->pageRank);
		searchPR[i]->URL[strlen(searchPR[i]->URL)-1] = '\0';
		printf("%s, %d, %f\n", searchPR[i]->URL, links, searchPR[i]->pageRank);

        // Finds the wanted word.
		i++;
    }
	*elems = i;
	for (i = 0; i < *elems; i++) {
		printf("Just added: %s\t%f\t%d\n", searchPR[i]->URL, searchPR[i]->pageRank, searchPR[i]->searchTerms);
	}
	fclose(pagerankList);
	return searchPR;
}


int main(int argc, char **argv)
{
	int i;
	printf("I'm okay\n");
	int elems;
	urlPR *searchPR = getPageRanks(&elems);
	for (i = 0; i < elems; i++) {
		printf("URL: %s\t%f\t%d\n", searchPR[i]->URL, searchPR[i]->pageRank, searchPR[i]->searchTerms);
	}
	printf("argc is %d\n", argc);
	for (i = 1; i < argc; i++) {
		char **URLs = getURLs(argv[i]);
		countOccurences(URLs, searchPR, elems);
	}
	for (i = 0; i < elems; i++) {
		printf("URL: %s\t%f\t%d\n", searchPR[i]->URL, searchPR[i]->pageRank, searchPR[i]->searchTerms);
	}
	// sort
	// print
}

