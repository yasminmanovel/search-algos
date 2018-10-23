/* scaledFootrule.c
 *
 * Written by Selina (z5208109) & Yasmin (z5207093)
 * Group: duckduckgo
 * Start Date: 10/10/18
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "set.h"
#include "readData.h"
#include "mystrdup.h"

#define URL_LENGTH 55
#define MIN(X, Y)  ( (X < Y) ? X : Y)  
#define MAX(X, Y)  ( (X > Y) ? X : Y)  
#define TRUE 1
#define FALSE 0

struct URLRank {
    char  *fileName;
    double posData[2][10];
};

typedef struct URLRank *rankFP;


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
/* Finds minimum from each row and subtracts from all elements. */
void rowReduce(double **matrix, int size)
{
    int row, col;
    for (row = 0; row < size; row++) {
        double min = INT_MAX;
        // Find min element in row.
        for (col = 0; col < size; col++)
            min = MIN(min, matrix[row][col]);
        // Subtracts min from all elements.
        for (col = 0; col < size; col++)
            matrix[row][col] -= min;
    }
    showMatrix(matrix, size);
}

/* Finds minimum from each col and subtracts from all elements. */
void colReduce(double **matrix, int size)
{
    int row, col;
    for (col = 0; col < size; col++) {
        int min = 0;
        // Find min element in row.
        for (row = 0; row < size; row++)
            min = MIN(min, matrix[row][col]);
        // Subtracts min from all elements.
        for (row = 0; row < size; row++)
            matrix[row][col] -= min;
    }
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
double calcSFRDist(int **pos, int newPos, int unionSize)
{
    double sum = 0;
    int i = 0;
    for (i = 0; pos[1][i] != -1 && pos[2][i] != -1; i++) {
        sum = sum + (pos[1][i]/pos[2][i] - newPos/unionSize);
    }
    return sum;
}


Set GetAllUrls(int nFiles, char **fileNames)
{
    Set URLs = newSet();
    int i = 0;
    for (i = 0; i < nFiles; i++) {
        FILE *file = fopen(fileNames[i], "r");
        char line[URL_LENGTH] = {0};
        while (fgets(line, URL_LENGTH, file) != NULL) {
            trim(line);
            insertInto(URLs, line);
            free(line);
        }
        fclose(file);
    }
    return URLs;
}

rankFP found(rankFP *array, int nURLs, char *URLName)
{
    int i = 0;
    for (i = 0; i < nURLs; i++) {
        if (strcmp(array[i]->fileName, URLName) == 0) return array[i];
    }
    return NULL;
}

double linesInFile(char *fileName)
{
    FILE *file = fopen(fileName, "r");
    char line[URL_LENGTH] = {0};
    double i = 0.0;
    while (fgets(line, URL_LENGTH, file) != NULL) i++;
    fclose(file);
    return i;
}

void insertRankData(rankFP URL, int pos, double nURLs)
{
    int i = 0;
    for (i = 0; URL->posData[1][i] != -1 && URL->posData[2][i] != -1; i++);
    URL->posData[1][i] = pos;
    URL->posData[2][i] = nURLs;
}

void buildRankADT(rankFP *array, char **fileNames, int nFiles)
{
    int i;
    for (i = 0; i < nFiles; i++) {
        double nURLs = linesInFile(fileNames[i]);
        FILE *file = fopen(fileNames[i], "r");
        char line[URL_LENGTH] = {0};
        int pos = 1;
        rankFP curr = NULL;
        while (fgets(line, URL_LENGTH, file) != NULL) {
            trim(line);
            if ((curr = found(array, nURLs, line)) != NULL) {
                insertRankData(curr, pos, nURLs);
            } else {
                newUrlRank();
            }
            free(line);
            pos++;
        }
        fclose(file);
    }
}

// get set of URLS
// for each rank file, read into info into URL array
// set the matrix values based on the info in the URL array

int main(int argc, char **argv) 
{
    int i;
    int nFiles = argc - 1; // Number of files given.
    if (nFiles == 0) 
        printf("Usage: ./scaledFootrule fileName ...\n");

    Set unionURL = GetAllUrls(nFiles, argv);
    int nURLS = nElems(unionURL);
    double numURLs = 0.0 + nURLS;
    rankFP *files = calloc(numURLs, sizeof(rankFP));
    buildRankADT(files, argv, numURLs);

    for (i = 0; i < nFiles; i++) {
        files[i] = newRankFile(argv[i+1]);
        insertURLs(unionURL, argv[i+1]);
    }

    /* 1. Represent a cost matrix with an n x n 2d array.
          cost[url][pos] */
    double **cost = malloc(numURLs * sizeof(double));
    for (i = 0; i < numURLs; i++)
        cost[i] = malloc(numURLs * sizeof(double));
    
    // Calculate cost for each url in each position.
    int row, col;
    for (row = 0; row < numURLs; row++) {
        SetNode curr = unionURL->elems; // To get URL name.
        for (col = 0; col < numURLs; col++)
            cost[row][col] = calcSFRDist(curr->val, col+1, numURLs);
        curr = curr->next;
    }
    showMatrix(cost, numURLs);

    return 0;
}


/* Creates a new rankFile struct. 
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
*/
/* Gets URLs in file and place it into set. 
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
*/

