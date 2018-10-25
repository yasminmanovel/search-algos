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
#include "mystring.h"
#include <math.h>

#define URL_LENGTH 55
#define MIN(X, Y)  ( (X < Y) ? X : Y)  
#define MAX(X, Y)  ( (X > Y) ? X : Y)  
#define TRUE 1
#define FALSE 0
#define SHIFT       1
#define INTERSECTING_LINE 2
#define EMPTY -1.0


struct URLRank {
    char  *fileName;
    double posData[2][10];
    int finalPos;
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
            printf("%.20f ", matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}


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
}


/* Finds minimum from each col and subtracts from all elements. */
void colReduce(double **matrix, int size)
{
    int row, col;
    for (col = 0; col < size; col++) {
        double min = INT_MAX;
        // Find min element in row.
        for (row = 0; row < size; row++)
            min = MIN(min, matrix[row][col]);
        // Subtracts min from all elements.
        for (row = 0; row < size; row++)
            matrix[row][col] -= min;
    }
}


/* Calculates the scaled foot rule distance. */
double calcSFRDist(double pos[2][10], int newPos, double unionSize)
{
    double sum = 0;
    int i = 0;
    // scaled foot rule distance is the sum of the difference between their 
    // original position and the position the're moved to
    for (i = 0; pos[0][i] != EMPTY && pos[1][i] != EMPTY; i++) {
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
    for (i = 0; URL->posData[0][i] != EMPTY && URL->posData[1][i] != EMPTY; i++);
    URL->posData[0][i] = pos;
    URL->posData[1][i] = nURLs;
}


// create new URL node
rankFP newUrlRank(char *name, int pos, double fileSize)
{
    rankFP newFP = calloc(1, sizeof(struct URLRank));
    int i = 0;
    for (i = 0; i < 10; i++) {
        newFP->posData[0][i] = EMPTY;
        newFP->posData[1][i] = EMPTY;
    }
    insertRankData(newFP, pos, fileSize);
    newFP->fileName = mystrdup(name);
    newFP->finalPos = -1;
    return newFP;
}


// reads Rank Files in and constructs an ADT that holds URLs and their 
// positions in the rank files
void buildRankADT(rankFP *array, char **fileNames, int nFiles)
{
    int i;
    int nURLs = 0;
    for (i = 1; i < nFiles; i++) {
        double nLines = linesInFile(fileNames[i]);
        FILE *file = fopen(fileNames[i], "r");
        char line[URL_LENGTH] = {0};
        int pos = 1; // position is indexed starting at 1, not 0
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
void setLine(int **coverMatrix, int numURLs, int row, int col)
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


/* Creates a 2d array that count zeroes in each row and col. */
int **countZeroes(double **matrix, int numURLs)
{
    int row, col;
    // double zero = 0.0;
    int **zeroCount = calloc(2, sizeof(int *));
    for (row = 0; row < 2; row++)
        zeroCount[row] = calloc(numURLs, sizeof(int));
    // Count zeroes in the matrix.
    for (row = 0; row < numURLs; row++) {
        for (col = 0; col < numURLs; col++) {
            if (matrix[row][col] != 0) continue;
            zeroCount[0][row]++;
            zeroCount[1][col]++;
        }
    }
    return zeroCount;
}


/* Counts min # of lines needed to cover all 0s. */
int numLinesToCoverZeroes(int **coverMatrix, int **zeroCount, double **cost, int numURLs)
{
    int i;
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
        zeroCount[zeroRow][zeroCol] -= max;
        for (i = 0; i < numURLs; i++) {
            if (zeroRow == 0 && cost[zeroCol][i] == 0.0) {
                zeroCount[1][i]--;
                setLine(coverMatrix, numURLs, zeroCol, -1);
            }
            if (zeroRow == 1 && cost[i][zeroCol] == 0.0) {
                zeroCount[1][i]--;
                setLine(coverMatrix, numURLs, -1, zeroCol);
            }
        }
        lines++;
    }


    return lines;
}


/* Row reduces all uncovered elements by min. */
void rowReduceUncovered(int **coverMatrix, double min, double **cost, int numURLs)
{
    int i, j;
    // Subtract min from all uncovered elements.
    for (i = 0; i < numURLs; i++) {
        for (j = 0; j < numURLs; j++) {
            // If not covered by a line.
            if (!coverMatrix[i][j])
                cost[i][j] -= min;
        }
    }
}

/* Adds min to all covered columns */
void colAddCovered(int **coverMatrix, double min, double **cost, int numURLs) 
{
    int i, j;
    // Add min to all elements that have an intersecting line.
    for (i = 0; i < numURLs; i++) {
        for (j = 0; j < numURLs; j++) {
            // If covered by 2 lines.
            if (coverMatrix[i][j] == INTERSECTING_LINE)
                cost[i][j] += min;
        }
    }
}


// finds the minimum index that is uncovered in the matrix
double findUncoveredAreaMin(double **matrix, int **coverMatrix, int size)
{
    int row, col;
    double min = INT_MAX;
    for (col = 0; col < size; col++) {
        for (row = 0; row < size; row++) {
            if (!coverMatrix[row][col]) min = MIN(min, matrix[row][col]);
        }
    }
    return min;
}


// helper function for the Merge Sort
void rankMerge(rankFP *array, int start, int middle, int end)
{
    int leftLength = middle - start + SHIFT;
    int rightLength = end - middle;

    // split given array in half
    rankFP *left = malloc(sizeof(rankFP) * leftLength);
    rankFP *right = malloc(sizeof(rankFP) * rightLength);
    for (int i = 0; i < leftLength; i++) left[i] = array[start + i];
    for (int i = 0; i < rightLength; i++) right[i] = array[middle + 1 + i];

    // merging back in order
    int i = 0, j = 0, k = start;
    while (i < leftLength && j < rightLength) {
        if (left[i]->finalPos <= right[j]->finalPos) {
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
void rankMergeSort(rankFP *array, int start, int end)
{
    if (start < end) {
        // same as (start + end)/2, but avoids overflow for large 
        // numbers
        int middle = start + (end - start)/2;
        // sort the two halves of the array
        rankMergeSort(array, start, middle);
        rankMergeSort(array, middle + SHIFT, end);
        // merge these sorted halves
        rankMerge(array, start, middle, end);

    }
}

int posAlreadyTaken(rankFP *files, int pos, int numURLs) {
    int i;
    for (i = 0; i < numURLs; i++) {
        if (files[i]->finalPos == pos) return TRUE;
    }
    return FALSE;
}


/* TO DO */
void getPosition(double **cost, rankFP *files, int numURLs, double *sum, double **ogCost)
{
    int i, j;
    for (i = 0; i < numURLs; i++) {
        for (j = 0; j < numURLs; j++) {
            if (cost[i][j] != 0.0) continue;
            if (posAlreadyTaken(files, j+1, numURLs)) continue;
            files[i]->finalPos = j+1;
            *sum = *sum + ogCost[i][j];
            break;
        }
    }
}


double getURLOrder(double **cost, rankFP *files, int length, double **ogCost)
{
    /* Need to figure out how to get each URL's position from the matrix */
    double sum = 0;
    getPosition(cost, files, length + 1, &sum, ogCost);
    rankMergeSort(files, 0, length);
    return sum;
}

/* Frees a double 2d array */
void freeDoubleMatrix(double **matrix, int size) 
{
    int i;
    for (i = 0; i < size; i++) 
        free(matrix[i]);
    free(matrix);
}

/* Frees an int 2d array */
void freeIntMatrix(int **matrix, int size) 
{
    int i;
    for (i = 0; i < size; i++) 
        free(matrix[i]);
    free(matrix);
}

/* Free rankFP pointer */
void freeFiles(rankFP *files, int size)
{
    int i;
    for (i = 0; i < size; i++) {
        free(files[i]->fileName);
        free(files[i]);
    }
    free(files);
}

// get set of URLS
// for each rank file, read into info into URL array
// set the matrix values based on the info in the URL array

int main(int argc, char **argv) 
{
    int i;
     // Number of files given.
    if (argc - 1 == 0) {
        printf("Usage: ./scaledFootrule fileName ...\n");
        exit(1);
    }
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
    double **ogCost = malloc(numURLs * sizeof(double));
    for (i = 0; i < numURLs; i++) 
        ogCost[i] = malloc(numURLs * sizeof(double));
    
    /* 2. Calculate the footrule distance for each [row][col] */
    int row, col;
    i = 0;
    for (row = 0; row < numURLs; row++) {
        for (col = 0; col < numURLs; col++) {
            cost[row][col] = calcSFRDist(files[i]->posData, col+1, numURLs);
            ogCost[row][col] = calcSFRDist(files[i]->posData, col+1, numURLs);
        }
        i++;
    }
    /* 3. Subtract row minima */
    rowReduce(cost, numURLs);
    /* 4. Subtract col minima */
    colReduce(cost, numURLs);
    // Keeps track of the number of zeros in each row and col respectively.
    int **zeroCount = countZeroes(cost, numURLs);

    // 2d array that reflects cost matrix.
    // 0 means not covered, and > 0 means covered.
    int **coverMatrix = calloc(numURLs, sizeof(int *));
    for (i = 0; i < numURLs; i++) 
        coverMatrix[i] = calloc(numURLs, sizeof(int));
    /* 5. Count number of lines L required to cover all the 0s */
    while (numLinesToCoverZeroes(coverMatrix, zeroCount, cost, numURLs) != numURLs) {
        /* 6. Find smallest number from uncovered area */
        double min = findUncoveredAreaMin(cost, coverMatrix, numURLs);
        /* Subtract this number from all UNCOVERED ROWS */
        rowReduceUncovered(coverMatrix, min, cost, numURLs);
        /* Add new smallest number to COVERED COLS */
        // is this function right, shouldnt it be == 1 or 2??
        colAddCovered(coverMatrix, min, cost, numURLs);
        /* Go back to step 5 and repeat */
    }
    double sum = getURLOrder(cost, files, numURLs - 1, ogCost); // sort normally
    printf("%f\n", sum);
    for (i = 0; i < numURLs; i++) {
        printf("%s\n", files[i]->fileName);
    }

    // Cleaning up.
    disposeSet(unionURL);
    freeDoubleMatrix(cost, numURLs);
    freeDoubleMatrix(ogCost, numURLs);
    freeIntMatrix(zeroCount, 2);
    freeIntMatrix(coverMatrix, numURLs);
    freeFiles(files, numURLs);

    return 0;
}


