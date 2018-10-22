/* scaledFootrule.c
 *
 * Written by Selina (z5208109) & Yasmin (z5207093)
 * Group: duckduckgo
 * Start Date: 10/10/18
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pagerank.h"


int main(int argc, char **argv) 
{
    int nFiles = argc - 1; // Number of files given.
    int i;
    char **URLs = NULL;
    // Get URLs from each file.
    for (i = 0; i < nFiles; i++) {
        URLs = getURLs(argv[i+1]);
    }
}

/* Gets the URLs written inside the file. */
char **getURLs(char *fileName)
{

}