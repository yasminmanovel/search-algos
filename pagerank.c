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
#include "readData.h"
#include "mystring.h"
#include <string.h>
#include <math.h>
#include <assert.h>

#define DEFAULT_VAL 1.0
#define SHIFT 1
#define NULL_TERM 1
#define INVALID_VAL -1
#define REQUIRED_ARGS 4
#define DAMPING 1
#define DIFFPR 2
#define MAX_ITER 3
#define TRUE 1
#define FALSE 0

typedef struct pageRankNode *PRNode;

struct pageRankNode {
    char *name;
    int   nOutLinks;
    int   nInlinks;
    double prevPR;
    double currPR;
    int search_words;
};


/* Calculate weight of inlinks */
double calculateWin(URL v, PRNode u, Graph web)
{
    double uIn = u->nInlinks;
    //for i in v's outlinks: add inlink
    // actual sum loop
    Link curr = v->outLink;
    double sum = 0;
    for (; curr != NULL; curr = curr->next)
        sum = sum + curr->URLPointer->numInLinks;
    return uIn/sum;
}


/* Calculate weight of outlinks */
double calculateWout(URL v, PRNode u, Graph web)
{
    double top = u->nOutLinks;
    // if no outlinks, set it to 0.5
    if (top == 0.0) top = 0.5;
    // Find v in graph.
    Link curr = v->outLink;
    double sum = 0;
    // For every outlink of v, add its outlinks.
    for(; curr != NULL; curr = curr->next) {
        sum = sum + curr->URLPointer->numOutLinks;
        // if no outlinks, set it to 0.5
        if (curr->URLPointer->numOutLinks == 0) sum = sum + 0.5;
    }
    return top/sum;
}


/* Calculates the current PR of a URL given its prev PR. */
double calculateCurrPR(PRNode currNode, PRNode *array, Graph web, double damp, int nURLs)
{
    double part1 = (1 - damp)/nURLs;
    double sum = 0;
    int i;
    // Finding currNode in graph to get inLinks and outLinks.
    for (i = 0; i < web->numURLs; i++) {
        if (strcmp(currNode->name, web->listOfUrls[i]->URLName) == 0) break;
    }
    // Calculates sum for currNode.
    Link curr = web->listOfUrls[i]->inLink;
    for (; curr != NULL; curr = curr->next) {
        double wIn = calculateWin(curr->URLPointer, currNode, web);
        double wOut = calculateWout(curr->URLPointer, currNode, web);
        int j;
        // To find the prevPR of current.
        for (j = 0; j < web->numURLs; j++) {
            if (strcmp(array[j]->name, curr->URLName) == 0) break;
        }
        sum += array[j]->prevPR * wIn * wOut;
    }
    double part2 = damp * sum;
    double PR = part1 + part2;
    return PR;
}


/* Calculates the new diff. */
double calculateDiffPR(PRNode currNode, Graph web)
{
    int i;
    double diff = 0;
    for (i = 0; i < web->numURLs; i++)
        diff = diff + fabs(currNode->currPR - currNode->prevPR);
    return diff;
}


// creating a new PageRank node and returning the pointer to it
PRNode newPageRankNode(char *URLName, int nURLs) {
    PRNode newPRNode = calloc(1, sizeof(struct pageRankNode));
    newPRNode->name = mystrdup(URLName);
    newPRNode->nOutLinks = 0;
    newPRNode->prevPR = DEFAULT_VAL/nURLs;
    newPRNode->currPR = INVALID_VAL;
    newPRNode->search_words = 0;
    return newPRNode;
}


/* Calculates pageranks of all URLs by DFS traversal. */
PRNode *PageRankW(Set URLList, double damp, double diffPR, int maxIterations, Graph web)  
{
    int i, j; // Generic counters.
    int nURLs = nElems(URLList);
    // Make a before and current PR array.
    PRNode *urlPRs = malloc(nURLs * sizeof(URL));

    // Initialise all prevPRs to 1/N and currPRs to -1.
    SetNode currLink = URLList->elems;
    for (i = 0; i < nURLs; ++i) {
        urlPRs[i] = newPageRankNode(currLink->val, nURLs);
        urlPRs[i]->nOutLinks = web->listOfUrls[i]->numOutLinks;
        urlPRs[i]->nInlinks = web->listOfUrls[i]->numInLinks;
        currLink = currLink->next;
    }

    i = 0;
    double diff = diffPR;
    // While less than max iterations or difference is not small enough.
    
    while (i < maxIterations && diff >= diffPR) {
        // For each URL, calculate the new pagerank.
        for (j = 0; j < web->numURLs; j++) {
            urlPRs[j]->currPR = calculateCurrPR(urlPRs[j], urlPRs, web, damp, nURLs);
            diff = calculateDiffPR(urlPRs[j], web);
            urlPRs[j]->prevPR = urlPRs[j]->currPR;
        }
        i++;
    }
    return urlPRs;
}


// helper function for the Merge Sort
void PRmerge(PRNode *array, int start, int middle, int end)
{
    int leftLength = middle - start + SHIFT;
    int rightLength = end - middle;

    // split given array in half
    PRNode *left = malloc(sizeof(URL) * leftLength);
    PRNode *right = malloc(sizeof(URL) * rightLength);
    for (int i = 0; i < leftLength; i++) left[i] = array[start + i];
    for (int i = 0; i < rightLength; i++) right[i] = array[middle + 1 + i];

    // merging back in order
    int i = 0, j = 0, k = start;
    while (i < leftLength && j < rightLength) {
        if (left[i]->currPR <= right[j]->currPR) {
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
void PRmergeSort(PRNode *array, int start, int end)
{
    if (start < end) {
        // same as (start + end)/2, but avoids overflow for large 
        // numbers
        int middle = start + (end - start)/2;
        // sort the two halves of the array
        PRmergeSort(array, start, middle);
        PRmergeSort(array, middle + SHIFT, end);
        // merge these sorted halves
        PRmerge(array, start, middle, end);

    }
}


// frees the Pagerank ADT
void dumpPR(PRNode *array, int nElems)
{
    int i;
    for (i = 0; i < nElems; i++) {
        free(array[i]->name);
        free(array[i]);
    }
    free(array);
}


int main(int argc, char **argv)
{
    if (argc != REQUIRED_ARGS) {
        printf("Usage: ./pagerank damping diffPR maxIterations\n");
        exit(EXIT_FAILURE);
    } 
    // Get args.
    double damp = atof(argv[DAMPING]);
    double diffPR = atof(argv[DIFFPR]);
    int maxIterations = atoi(argv[MAX_ITER]);
    // Creates a set of URLs and creates an adjacency list graph.
    Set URLList = getCollection();
    Graph web = getGraph(URLList);
    int nURLs = nElems(URLList);

    // Calculates pageranks and sorts them in order.
    PRNode *urlPRs = PageRankW(URLList, damp, diffPR, maxIterations, web);
    PRmergeSort(urlPRs, 0, web->numURLs-SHIFT);

    // Opens file and prints to it.
    FILE *PRList = fopen("pagerankList.txt", "w");
    if (PRList == NULL) { perror("fopen failed"); exit(EXIT_FAILURE); }
    int i;
    for(i = nURLs - 1; i >= 0; i--)
        fprintf(PRList, "%s, %d, %.7f\n", urlPRs[i]->name, urlPRs[i]->nOutLinks, urlPRs[i]->currPR);
    fclose(PRList);
    // free allocated memory
    dumpPR(urlPRs, nURLs);
    disposeSet(URLList);
    freeGraph(web);
    return 0;
}