/* pagerank.c
 *
 * Written by Selina (z5208109) & Yasmin (z5207093)
 * Group: duckduckgo
 * Start Date: 10/10/18
 */

/* PSEUDOCODE:
 * Get args: d, diffPR, maxIterations
 * listOfURLs = getCollection()
 * Graph g = getGraph()
 * 
 * listURLsPageRanks = calculatePageRank(g, d, diffPR, maxIterations);
 * orderedListURLsPageRanks = order(listURLsPageRanks)
 * Output -> orderedListURLsPageRanks to "pagerankList.txt"
 * 
 * FORMAT OF pagerankList.txt:
 *  URL, num of outgoing links, page rank
 */

#include <stdio.h>
#include <stdlib.h>
#include "set.h"
#include "graph.h"
#include "stack.h"
#include "readData.h"

typedef struct pageRankNode *PRNode;

struct pageRankNode {
    char *name;
    int   nOutLinks;
    float prevPR;
    float currPR;
};

/* Calculates the current PR of a URL given its prev PR. */
float calculateCurrPR(PRNode *urlPRs)
{
    return 0;
}

/* Calculates the new diff. */
float calculateDiffPR()
{
    return 0;
}

/* Calculates pageranks of all URLs by DFS traversal. */
PRNode *PageRankW(Set URLList, float damp, float diffPR, int maxIterations)  
{
    int i, j; // Generic counters.
    int nURLs = nElems(URLList);

    // Make a before and current PR array.
    PRNode *urlPRs = malloc(nURLs * sizeof(URL));

    // Initialise all prevPRs to 1/N and currPRs to -1.
    SetNode currLink = URLList->elems;
    for (i = 0; i < nURLs; ++i) {
        urlPRs[i]->name = strdup(currLink->val);
        urlPRs[i]->nOutLinks = 0;    // CHANGE THIS.
        urlPRs[i]->prevPR = 1.0/nURLs;
        urlPRs[i]->currPR = -1;
        currLink = currLink->next;
    }

    i = 0;
    int diff = diffPR;
    // While less than max iterations or difference is not small enough.
    while (i < maxIterations && diff >= diffPR) {
        // For each URL, calculate the new pagerank.
        for (j = 0; j < nURLs; j++) {
            urlPRs[i]->currPR = calculateCurrPR(urlPRs);
            diff = calculateDiffPR();
        }
        i++;
    }
    return urlPRs;
}


void merge(PRNode *array, int start, int middle, int end)
{
    int leftLength = middle - start + 1;
    int rightLength = end - middle;

    PRNode *left = malloc(sizeof(URL) * leftLength);
    PRNode *right = malloc(sizeof(URL) * rightLength);
    for (int i = 0; i < leftLength; i++) left[i] = array[start + i];
    for (int i = 0; i < rightLength; i++) right[i] = array[middle + start + i];

    int i = 0, j = 0, k = start;
    while (i < leftLength && j < rightLength) {
        if (left[i]->currPR <= right[j]->currPR) {

        }
    }


    free(left); free(right);
}

void mergeSort(PRNode *array, int start, int end)
{
    if (start < end) {
        // same as (start + end)/2, but apparently avoids overflow for large 
        // numbers
        int middle = start + (end - 1)/2;
        // sort the two halves of the array
        mergeSort(array, start, middle);
        mergeSort(array, middle + 1, end);
        // merge these sorted halves
        merge(array, start, middle, end);

    }
}

/* Sorts URLs by decreasing page rank order. */
void order(PRNode *urlPRs, int length)
{
    mergeSort(urlPRs, 0, length-1);
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("Usage: ./pagerank damping diffPR maxIterations\n");
        exit(EXIT_FAILURE);
    } 
    // Get args.
    int damp = argv[1];
    int diffPR = argv[2];
    int maxIterations = argv[3];
    // Creates a set of URLs and creates an adjacency list graph.
    Set URLList = getCollection();
    Graph web = getGraph(URLList);
    int nURLs = nElems(URLList);

    // Calculates pageranks and sorts them in order.
    PRNode *urlPRs = PageRankW(URLList, damp, diffPR, maxIterations);
    order(urlPRs, web->numURLs);

    // Opens file and prints to it.
    FILE *PRList = fopen("pagerankList.txt", "w");
    if (!PRList) { perror("fopen failed"); exit(EXIT_FAILURE); }
    int i;
    for(i = 0; i < nURLs; i++)
        fprintf(PRList, "%s, %d, %.7f\n", 
                    urlPRs[i].name, urlPRs[i].nOutLinks, urlPRs[i].currPR);

    return 0;
}





