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
#include "mystrdup.h"

#define SEEN_ONCE       1
#define SEEN_TWICE      2
#define URL_LENGTH      55
#define MAX_LINE        1001
#define NULL_TERM_SPACE 1
#define START_TAG_LEN   16
#define END_TAG_LEN     14
#define CHAR_LEN        1

typedef struct _rankFile {
    char  *fileName;
    char **URLs;
    int    nURLs;
} rankFile;

typedef rankFile *rankFP;

rankFP newRankFile();
void insertURLs(Set unionURL, char *fileName);
void showRankFile(rankFP file);

int main(int argc, char **argv) 
{
    int nFiles = argc - 1; // Number of files given.
    int i;

    // Get union of all URLs.
    Set unionURL = newSet();
    // Creates a 2D array of [file][url].
    rankFP *files = calloc(nFiles, sizeof(rankFP));
    for (i = 0; i < nFiles; i++) {
        files[i] = newRankFile(argv[i+1]);
        insertURLs(unionURL, argv[i+1]);
        showRankFile(files[i]);
    }
    return 0;
}

/* Calculates the scaled foot rule distance. */
// double calcSFRDist()
// {

// }

/* Creates a new rankFile struct. */
rankFP newRankFile(char *file)
{
    char line[URL_LENGTH] = {0};

    rankFile *new = calloc(1, sizeof(rankFile));
    new->fileName = mystrdup(file);
    new->nURLs = 0;
    
    FILE *fp = fopen(file, "r");
    if (!fp) { perror("fopen failed"); exit(EXIT_FAILURE); }
    // Counts number of URLs in file.
    while (fgets(line, URL_LENGTH, fp) != NULL)
        new->nURLs++;
    // Goes back to beginning of the file.
    rewind(fp);

    // Creates a string of URLs separated by spaces.
    char *URLs = calloc(new->nURLs, URL_LENGTH);
    while (fgets(line, URL_LENGTH, fp) != NULL) {
        char *word = normalise(line);
        strcat(URLs, " ");
        strcat(URLs, word);
        free(word);
    }
    new->URLs = tokenise(URLs, " ");
    fclose(fp);

    return new;
}

void showRankFile(rankFP file) 
{
    printf("%s\n", file->fileName);
    printf("    nURLS %d\n", file->nURLs);
    printf("    URLS:");
    int i;
    for (i = 0; file->URLs[i] != NULL; i++)
        printf(" %s", file->URLs[i]);
    printf("\n");
}

/* Gets URLs in file and place it into set. */
void insertURLs(Set unionURL, char *fileName)
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