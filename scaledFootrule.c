/* scaledFootrule.c
 *
 * Written by Selina (z5208109) & Yasmin (z5207093)
 * Group: duckduckgo
 * Start Date: 10/10/18
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "readData.h"

#define SEEN_ONCE       1
#define SEEN_TWICE      2
#define URL_LENGTH      55
#define MAX_LINE        1001
#define NULL_TERM_SPACE 1
#define START_TAG_LEN   16
#define END_TAG_LEN     14
#define CHAR_LEN        1

typedef rankFile *rankFP;

typedef struct _rankFile {
    char  *fileName;
    int    fileSize;
    char **urls;
} rankFile;

void insertURLs(Set unionURL, char *fileName);

int main(int argc, char **argv) 
{
    int nFiles = argc - 1; // Number of files given.
    int i;

    // Get union of all URLs.
    Set unionURL = newSet();
    // Creates a 2D array of [file][url].
    rankFile *files = malloc(nFiles * sizeof(rankFP));
    for (i = 0; i < nFiles; i++) {
        URLs[i] = insertURLs(unionURL, argv[i+1]);
    }

}

/* Calculates the scaled foot rule distance. */
double calcSFRDist()
{

}

/* Gets URLs in file and place it into set. */
char **insertURLs(Set unionURL, char *fileName)
{
    // Opens the file.
    FILE *fp = fopen(fileName, "r");
    if (!fp) { perror("fopen failed"); exit(EXIT_FAILURE); }

    // Read every line and insert url into set.
    char line[URL_LENGTH] = {0};
    while (fgets(line, URL_LENGTH, fp) != NULL) {
        char *url = normalise(line);
        insertInto(unionURL, url);
        free(url);
    }
    fclose(fp);
}