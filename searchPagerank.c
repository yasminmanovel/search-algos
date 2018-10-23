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

#define MAX_LINE    1001
#define URL_LENGTH  55
#define SHIFT       1
#define PAGERANK    0
#define SEARCHTERMS 1


typedef struct url *urlPR;

struct url {
	int searchTerms;
	float pageRank;
	char *URL;
};

void dumpSearchPR(urlPR *array, int nElems)
{
    int i;
    for (i = 0; i < nElems; i++) {
        free(array[i]->URL);
        free(array[i]);
    }
    free(array);
}


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
	fclose(invIndex);
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
	int i;
	for (i = 0; i < elems; i++) {
		int j;
		for (j = 0; URLs[j] != NULL; j++) {
			if (strcmp(searchPR[i]->URL, URLs[j]) == 0) searchPR[i]->searchTerms++;
		}
	}
}

int numOfElems()
{
    FILE *pagerankList = fopen("pagerankList.txt", "r");
    if (!pagerankList) { perror("fopen failed"); exit(EXIT_FAILURE); }
	int i = 0;
    char line[MAX_LINE] = {0};
    while (fgets(line, MAX_LINE, pagerankList) != NULL) i++;
	fclose(pagerankList);
	return i;
}

urlPR *getPageRanks(int *elems) 
{
	*elems = numOfElems();
    FILE *pagerankList = fopen("pagerankList.txt", "r");
    if (!pagerankList) { perror("fopen failed"); exit(EXIT_FAILURE); }
    char line[MAX_LINE] = {0};
	int links;
	urlPR *searchPR = malloc(sizeof(struct url) * *elems);
	int i = 0;
    while (fgets(line, MAX_LINE, pagerankList) != NULL) {
		searchPR[i] = newSearchPRNode();
        sscanf(line, "%s %d, %f", searchPR[i]->URL, &links, &searchPR[i]->pageRank);
		searchPR[i]->URL[strlen(searchPR[i]->URL)-1] = '\0';
        // Finds the wanted word.
		i++;
    }
	fclose(pagerankList);
	return searchPR;
}


// helper function for the Merge Sort
void PRmerge(int searchBy, urlPR *array, int start, int middle, int end)
{
    int leftLength = middle - start + SHIFT;
    int rightLength = end - middle;

    // split given array in half
    urlPR *left = malloc(sizeof(urlPR) * leftLength);
    urlPR *right = malloc(sizeof(urlPR) * rightLength);
    for (int i = 0; i < leftLength; i++) left[i] = array[start + i];
    for (int i = 0; i < rightLength; i++) right[i] = array[middle + 1 + i];

    // merging back in order
    int i = 0, j = 0, k = start;
	/*
	PRmergeSort(PAGERANK, searchPR, 0, elems-SHIFT);
	PRmergeSort(SEARCHTERMS, searchPR, 0, elems-SHIFT);
	*/
    while (i < leftLength && j < rightLength) {
        if (left[i]->pageRank >= right[j]->pageRank && searchBy == PAGERANK) {
            array[k] = left[i];
            i++;
		} else if (left[i]->searchTerms >= right[j]->searchTerms && searchBy == SEARCHTERMS) {
            array[k] = left[i];
            i++;
        } else {
            array[k] = right[j];
            j++;
        }
        k++;
    }
    // merging remaining elements
    while (i < leftLength) {
        array[k] = left[i];
        i++; k++;
 
    }
    while (j < rightLength) {
        array[k] = right[j];
        j++; k++;
    }

    free(left); free(right);
}


// Merge Sort that is used to order the URLS by  their Page Rank
void PRmergeSort(int searchBy, urlPR *array, int start, int end)
{
    if (start < end) {
        // same as (start + end)/2, but apparently avoids overflow for large 
        // numbers
        int middle = start + (end - start)/2;
        // sort the two halves of the array
        PRmergeSort(searchBy, array, start, middle);
        PRmergeSort(searchBy, array, middle + SHIFT, end);
        // merge these sorted halves
        PRmerge(searchBy, array, start, middle, end);

    }
}

// Does it matter if the same word occurs twice in a url?
int main(int argc, char **argv)
{
	int i;
	int elems;
	urlPR *searchPR = getPageRanks(&elems);
	for (i = 1; i < argc; i++) {
		char **URLs = getURLs(argv[i]);
		countOccurences(URLs, searchPR, elems);
		freeTokens(URLs);
	}
	PRmergeSort(PAGERANK, searchPR, 0, elems-SHIFT);
	PRmergeSort(SEARCHTERMS, searchPR, 0, elems-SHIFT);
	for (i = 0; i < elems && i < 30; i++) {
		if (searchPR[i]->searchTerms == 0) continue;
		printf("%s\n", searchPR[i]->URL);
	}
	dumpSearchPR(searchPR, elems);
	return 0;
}


