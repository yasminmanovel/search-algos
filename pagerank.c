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

typedef struct _URL {
    float prevPageRank;
    float currPageRank;
    char *name;
} URL;

/* Calculates pageranks of all URLs by DFS traversal. */
float calculatePageRank(Graph web, int damp, int diffPR, int maxIterations)  
{

    return 0;
}

/* Sorts URLs by decreasing page rank order. */
void order()
{
    
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
    int i = 0;

    // Make a before and current PR array.
    URL* urlPRs = malloc(nURLs * sizeof(URL));

    // Initialise all prevPRs to 1/N and currPrs to -1.
    Link currLink = URLList;
    for (i = 0; i < nURLs; ++i) {
        urlPRs[i].prevPageRank = 1.0/nURLs;
        urlPRs[i].currPageRank = -1;
        urlPRs[i].name = currLink->val;
        currLink = currLink->next;
    }

    i = 0;
    int diff = diffPR;
    // While less than max iterations or difference is not small enough.
    while (i < maxIterations && diff >= diffPR) {
        calculatePageRank(web, damp, diffPR, maxIterations);
        i++;
    }

    /* PR[maxIterations][maxURLs]
        [ [1/3, 1/3, 1/3],
          []

        ]
    */

    


    return 0;
}





