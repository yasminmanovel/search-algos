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

/* Calculates pageranks of all URLs */
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

    Set URLList = getCollection();
    Graph web = getGraph(URLList);

    


    return 0;
}





