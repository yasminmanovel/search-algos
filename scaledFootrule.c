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
double calcSFRDist(char *url, int newPos);
void showRankFile(rankFP file);
void showMatrix(double **matrix, int n);

/* THE HUNGARIAN ALGORITHM:
 * 1. Represent cost matrix in an n x n 2d array
 *    - Rows: urls in set
 *    - Cols: possible positions
 * 2. Calculate the footrule distance for each [row][col]
 * 3. Subtract row minima
 *    - Subtract minima from all elements in row.
 *    - This gives at least 1 0 on each row.
 * 4. Subtract col minima
 *    - Subtract minima from all elements in col.
 *    - Gives at least 1 0 on each col.
 * 5. Count number of lines L required to cover all the 0s.
 *    - If L == n, optimal sol found.
 *    - else, move to next step.
 * 6. Find smallest number from uncovered area.
 * 7. Subtract this number from all UNCOVERED ROWS.
 * 8. Add new smallest number to COVERED COLS.
 * 9. Go back to step 5 and repeat.
 */

int main(int argc, char **argv) 
{
    int i;
    int nFiles = argc - 1; // Number of files given.
    if (nFiles == 0) 
        printf("Usage: ./scaledFootrule fileName ...\n");

    // Creates a 2D array of [file][url] and gets union of all URLs.
    Set unionURL = newSet();
    rankFP *files = calloc(nFiles, sizeof(rankFP));
    for (i = 0; i < nFiles; i++) {
        files[i] = newRankFile(argv[i+1]);
        insertURLs(unionURL, argv[i+1]);
    }

    /* 1. Represent a cost matrix with an n x n 2d array.
          cost[url][pos] */
    int nURLs = nElems(unionURL);
    double **cost = malloc(nURLs * sizeof(double));
    for (i = 0; i < nURLs; i++)
        cost[i] = malloc(nURLs * sizeof(double));
    
    // Calculate cost for each url in each position.
    int row, col;
    for (row = 0; row < nURLs; row++) {
        SetNode curr = unionURL->elems; // To get URL name.
        for (col = 0; col < nURLs; col++)
            cost[row][col] = calcSFRDist(curr->val, col+1);
        curr = curr->next;
    }
    showMatrix(cost, nURLs);

    return 0;
}

void showMatrix(double **matrix, int n)
{
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            printf("%f ", matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}

/* Calculates the scaled foot rule distance. */
double calcSFRDist(char *url, int newPos)
{
    return 1.0;
}

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