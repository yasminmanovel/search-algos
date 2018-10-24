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
#include <math.h>

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

// print matrix out
void showMatrix(double **matrix, int n)
{
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            printf("%.2f ", matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}

/* Finds minimum from each row and subtracts from all elements. */
void rowReduce(double **matrix, int size, int **zeroCount)
{
    int row, col;
    for (row = 0; row < size; row++) {
        double min = INT_MAX;
        // Find min element in row.
        for (col = 0; col < size; col++)
            min = MIN(min, matrix[row][col]);
        // Subtracts min from all elements.
        for (col = 0; col < size; col++) {
            matrix[row][col] -= min;
            // Counts number of zeros in row.
            if (matrix[row][col] == 0) zeroCount[0][row]++;
        }
    }
}

/* Finds minimum from each col and subtracts from all elements. */
void colReduce(double **matrix, int size, int **zeroCount)
{
    int row, col;
    for (col = 0; col < size; col++) {
        double min = INT_MAX;
        // Find min element in row.
        for (row = 0; row < size; row++)
            min = MIN(min, matrix[row][col]);
        // Subtracts min from all elements.
        for (row = 0; row < size; row++) {
            matrix[row][col] -= min;
            // Counts # of zeros in col.
            if (matrix[row][col] == 0) zeroCount[1][col]++;
        }
    }
}


/* Calculates the scaled foot rule distance. */
double calcSFRDist(double pos[2][10], int newPos, double unionSize)
{
    double sum = 0;
    int i = 0;
    for (i = 0; pos[0][i] != -1.0 && pos[1][i] != -1.0; i++) {
        sum = sum + fabs((pos[0][i]/pos[1][i]) - (newPos/unionSize));
    }
    return fabs(sum);
}

// read all URLS into a set
Set GetAllUrls(int nFiles, char **fileNames)
{
    Set URLs = newSet();
    int i = 0;
    for (i = 1; i < nFiles; i++) {
        FILE *file = fopen(fileNames[i], "r");
        char line[URL_LENGTH] = {0};
        while (fgets(line, URL_LENGTH, file) != NULL) {
            trim(line);
            insertInto(URLs, line);
        }
        fclose(file);
    }
    return URLs;
}

// Check if the URL already exists in the array
rankFP found(rankFP *array, int nURLs, char *URLName)
{
    int i;
    for (i = 0; i < nURLs; i++) {
        if (strcmp(array[i]->fileName, URLName) == 0) return array[i];
    }
    return NULL;
}

// gets the number of lines in a file - the number of URLs in each ordered list
double linesInFile(char *fileName)
{
    FILE *file = fopen(fileName, "r");
    char line[URL_LENGTH] = {0};
    double i = 0.0;
    while (fgets(line, URL_LENGTH, file) != NULL) i++;
    fclose(file);
    return i;
}

// insert new positional data into URL
void insertRankData(rankFP URL, int pos, double nURLs)
{
    int i;
    for (i = 0; URL->posData[0][i] != -1.0 && URL->posData[1][i] != -1.0; i++);
    URL->posData[0][i] = pos;
    URL->posData[1][i] = nURLs;
}

// create new URL node
rankFP newUrlRank(char *name, int pos, double fileSize)
{
    rankFP newFP = calloc(1, sizeof(struct URLRank));
    int i = 0;
    for (i = 0; i < 10; i++) {
        newFP->posData[0][i] = -1.0;
        newFP->posData[1][i] = -1.0;
    }
    insertRankData(newFP, pos, fileSize);
    newFP->fileName = mystrdup(name);
    return newFP;
}


void buildRankADT(rankFP *array, char **fileNames, int nFiles)
{
    int i;
    int nURLs = 0;
    for (i = 1; i < nFiles; i++) {
        double nLines = linesInFile(fileNames[i]);
        FILE *file = fopen(fileNames[i], "r");
        char line[URL_LENGTH] = {0};
        int pos = 1;
        rankFP curr = NULL;
        while (fgets(line, URL_LENGTH, file) != NULL) {
            trim(line);
            curr = found(array, nURLs, line);
            if (curr != NULL) {
                insertRankData(curr, pos, nLines);
            } else {
                array[nURLs] = newUrlRank(line, pos, nLines);
                nURLs++;
            }
            pos++;
        }
        fclose(file);
    }
}

/* Sets the zeroCount[row][col] to all 1s. */
void setZero(int **coverMatrix, int numURLs, int row, int col)
{
    int i;
    // If want to set a column to 1.
    if (row == -1) {
        for (i = 0; i < numURLs; i++)
            coverMatrix[i][col]++;
    // If want to set a row to 1.
    } else if (col == -1) {
        for (i = 0; i < numURLs; i++)
            coverMatrix[row][i]++;
    }
}

/* Checks if matrix is all 0s. */
int allZero(int **matrix, int nRows, int nCols)
{
    int i, j;
    for (i = 0; i < nRows; i++) {
        for (j = 0; j < nCols; j++) {
            if (matrix[i][j] != 0)
                return FALSE;
        }
    }
    return TRUE;
}

/* Gets max num in a 2d array */
int getMax(int **matrix, int nRows, int nCols, int *index)
{
    int i, j;
    int max = 0;
    // Goes through rows & cols to get row/col that has the most 0s.
    for (i = 0; i < nRows; i++) {
        for (j = 0; j < nCols; j++) {
            if (matrix[i][j] > max) {
                max = matrix[i][j];
                *index = (i * nCols) + j;
            }
        }
    }
    return max;
}

/* Counts min # of lines needed to cover all 0s. */
int numLinesToCoverZeroes(int **zeroCount, double **cost, int numURLs)
{
    int i;
    // 2d array that reflects cost matrix.
    // 0 means not covered, and > 0 means covered.
    int **coverMatrix = calloc(numURLs, sizeof(int *));
    for (i = 0; i < numURLs; i++) 
        coverMatrix[i] = calloc(numURLs, sizeof(int));

    int max = 0;
    int index, zeroRow, zeroCol;
    int lines = 0;
    // While not all zeroes have been covered.
    while (!allZero(zeroCount, 2, numURLs)) {
        max = getMax(zeroCount, 2, numURLs, &index);
        // Gets row and col of zeroCount.
        zeroRow = index / numURLs;
        zeroCol = index % numURLs;
        // Subtract # of zeroes crossed out from covered rows & cols.
        // Max is in row.
        zeroCount[zeroRow][zeroCol] -= max;
        if (zeroRow == 0) {
            for (i = 0; i < numURLs; i++) {
                if (cost[zeroCol][i] == 0.0)
                    zeroCount[1][i]--;
            }
            setZero(coverMatrix, numURLs, zeroCol, -1);
        // Max is in col.
        } else if (zeroRow == 1) {
            for (i = 0; i < numURLs; i++) {
                if (cost[i][zeroCol] == 0.0)
                    zeroCount[1][i]--;
            }
            setZero(coverMatrix, numURLs, zeroCol, -1);
        }
        // printf("INSIDE LOOP\n");
        // for (i = 0; i < 2; i++) {
        //     for (j = 0; j < numURLs; j++) printf("%d ", zeroCount[i][j]);
        //     printf("\n");
        // }
        // printf("\n");
        lines++;
    }

    // printf("zeroCount:\n");
    // for (i = 0; i < 2; i++) {
    //     for (j = 0; j < numURLs; j++) printf("%d ", zeroCount[i][j]);
    //     printf("\n");
    // }
    // printf("\n");

    // printf("Cover Matrix:\n");
    // for (i = 0; i < numURLs; i++) {
    //     for (j = 0; j < numURLs; j++) {
    //         printf("%d ", coverMatrix[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    // printf("lines %d\n", lines);


    return lines;
}

// get set of URLS
// for each rank file, read into info into URL array
// set the matrix values based on the info in the URL array

int main(int argc, char **argv) 
{
    int i, j;
     // Number of files given.
    if (argc - 1 == 0) 
        printf("Usage: ./scaledFootrule fileName ...\n");

    Set unionURL = GetAllUrls(argc, argv);
    double numURLs = nElems(unionURL) + 0.0;
    /* building ADT that represents the URLs and their positions within the 
    rank files */
    rankFP *files = calloc(numURLs, sizeof(rankFP));
    buildRankADT(files, argv, argc);
    /* 1. Represent a cost matrix with an n x n 2d array.
          cost[url][pos] */
    double **cost = malloc(numURLs * sizeof(double));
    for (i = 0; i < numURLs; i++)
        cost[i] = malloc(numURLs * sizeof(double));
    
    /* 2. Calculate the footrule distance for each [row][col] */
    int row, col;
    i = 0;
    for (row = 0; row < numURLs; row++) {
        for (col = 0; col < numURLs; col++)
            cost[row][col] = calcSFRDist(files[i]->posData, col+1, numURLs);
        i++;
    }
    showMatrix(cost, numURLs);

    // Keeps track of the number of zeros in each row and col respectively.
    int **zeroCount = calloc(numURLs, sizeof(int *));
    for (i = 0; i < numURLs; i++)
        zeroCount[i] = calloc(numURLs, sizeof(int));
    /* 3. Subtract row minima */
    rowReduce(cost, numURLs, zeroCount);
    /* 4. Subtract col minima */
    colReduce(cost, numURLs, zeroCount);
    showMatrix(cost, numURLs);
    for (i = 0; i < 2; i++) {
        for (j = 0; j < numURLs; j++) printf("%d ", zeroCount[i][j]);
        printf("\n");
    }
    /* 5. Count number of lines L required to cover all the 0s */

    while (numLinesToCoverZeroes(zeroCount, cost, numURLs) != numURLs) { // Selina
        printf("finding\n");
    //     /* 6. Find smallest number from uncovered area */
         int min  = findUncoveredAreaMin(cost, numURLs); // Yasmin
    //     /* Subtract this number from all UNCOVERED ROWS */
    //     rowReduceUncovered(min, cost, numURLs); // Selina
    //     /* Add new smallest number to COVERED COLS */
    //     colAddCovered(min, cost, numURLs); // Selina
    //     /* Go back to step 5 and repeat */
    }
    printf("FOUND\n");
    // char **orderedURLs = getURLOrder(cost); // Yasmin

    return 0;
}

