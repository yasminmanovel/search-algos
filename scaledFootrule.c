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
#define SHIFT       1

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
    newFP->finalPos = -1;
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


/* Counts min # of lines needed to cover all 0s. 
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
*/

/* Row reduces all uncovered elements by min. */
void rowReduceUncovered(int **coverMatrix, double min, double **cost, int numURLs)
{
    int i, j;
    // Subtract min from all uncovered elements.
    for (i = 0; i < numURLs; i++) {
        for (j = 0; j < numURLs; j++) {
            // If not covered by a line.
            if (coverMatrix[i][j] == 0)
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
            if (coverMatrix[i][j] == 2)
                cost[i][j] += min;
        }
    }
}


// finds the minimum index that is uncovered in the matrix
double findUncoveredAreaMin(double **matrix, int **coverMatrix, int size)
{
    int row, col;
    double min = INT_MAX;
    for (row = 0; row < size; row++) {
        for (col = 0; col < size; col++) {
            if (coverMatrix[row][col] == 0.0) 
                min = MIN(min, matrix[row][col]);
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
        // same as (start + end)/2, but apparently avoids overflow for large 
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

int numZeroes(double *array, int numURLs)
{
    int i;
    int numZeroes = 0;
    for (i = 0; i < numURLs; i++) {
        if (array[i] != 0.0) continue;
        numZeroes++;
    }
    return numZeroes;
}

int positionsFilled(rankFP *files, int numURLs)
{
    int i;
    for (i = 0; i < numURLs; i++) {
        if (files[i]->finalPos == -1) return FALSE;
    }
    return TRUE;
}

/* TO DO */
void getPosition(double **cost, rankFP *files, int numURLs, double *sum, double **ogCost)
{
    int zeroes, row, col;
    for (zeroes = 1; zeroes <= numURLs; zeroes++) {
        printf("Rows with %d zeroes\n", zeroes);
        for (row = 0; row < numURLs; row++) {
            printf("row %d has %d zeroes\n", row, numZeroes(cost[row], numURLs));
            printf("%d vs %d\n", zeroes, numZeroes(cost[row], numURLs));
            if (zeroes != numZeroes(cost[row], numURLs)) continue;
            for (col = 0; col < numURLs; col++) {
                if (cost[row][col] != 0.0) continue;
                if (posAlreadyTaken(files, col+1, numURLs)) continue;
                files[row]->finalPos = col+1;
                printf("Adding %f\n", ogCost[row][col]);
                *sum = *sum + ogCost[row][col];
                break;
            }
        }
        if (positionsFilled(files, numURLs)) return;
    }
    // int i, j;
    // int posSet;
    // for (i = 0; i < numURLs; i++) {
    //     for (j = 0; j < numURLs; j++) {
    //         if (cost[i][j] != 0.0) continue;
    //         if (posAlreadyTaken(files, j+1, numURLs)) continue;
    //         files[i]->finalPos = j+1;
    //         *sum = *sum + ogCost[i][j];
    //         break;
    //     }
    //     if (posSet = numURLs) return;
    // }
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

/* Builds an adjMat based on cost */
int **buildAdjMat(double **cost, int numURLs)
{
    int i, j;
    // Creates an adjacency matrix based on cost.
    // 2 * numURLs for URLs and pos. +2 for source and sink.
    // Source and sink are the last 2 rows respectively.
    int size = (2 * numURLs) + 2;
    int sourceInd = size-2;
    int sinkInd = size-1;
    int **adjMat = calloc(size, sizeof(int *));
    for (i = 0; i < size; i++) 
        adjMat[i] = calloc(size, sizeof(int));
    
    // showMatrix(cost, numURLs);
    // Builds matrix based on cost.
    for (i = 0; i < numURLs; i++) {
        for (j = 0; j < numURLs; j++) {
            // Edge i -> j has been found.
            if (cost[i][j] == 0.0)
                adjMat[i][numURLs+j] = 1;
        }
    }
    // Makes all positions point to the sink.
    // (makes all last columns 1 for pos).
    // Makes source point to all URLs.
    for (i = 0; i < numURLs; i++) {
        adjMat[numURLs+i][sinkInd] = 1; 
        adjMat[sourceInd][i] = 1;
    }
    return adjMat;
}

/* Recursive dfs to find path from src to dst */
int dfs(int **adjMat, int size, int *visited, int src, int dst)
{
    int y;
    for (y = 0; y < size; y++) {
        if (visited[y] == -1 && adjMat[src][y] == 1) {
            visited[y] = src;
            if (y == dst) return 1;
            else if (dfs(adjMat, size, visited, y, dst))
                return 1;
        }
    }
    return 0;
}

/* Reverses the edges in the path to become -1 in adjMat. */
void reversePath(int *visited, int **adjMat, int size)
{
    int i;
    // Builds path from visited array.
    int *path = malloc(size * sizeof(int));
    for (i = 0; i < size; i++) path[i] = -1;
    int sourceInd = size-2;
    int sinkInd = size-1; // Sink is always dest.
    // int sourceInd = size-2;
    int curr = sinkInd;
    int temp = -2;
    i = 0;
    while (curr != sourceInd) {
        path[i] = curr;
        temp = curr;
        curr = visited[curr];
        visited[temp] = -1;
        i++;
    }
    path[i] = curr;

    // Reverse the edges.
    for (i = 0; i < size-1; i++) {
        if (path[i+1] == -1) break;
        adjMat[path[i+1]][path[i]] = -1;
        adjMat[path[i]][path[i+1]] = 1;
    }
}

/* Counts min # of lines needed to cover all 0s using network flow solution
   to find maximum bipartite matching between URL and pos. */
int numLinesToCoverZeroes(double **cost, int numURLs, int **adjMat)
{
    int i;
    int adjSize = (2 * numURLs) + 2;
    int sourceInd = adjSize-2, sinkInd = adjSize-1;

    // Network flow solution.
    // Keeps finding flow from source to sink. Once path is found, 
    // edges are reversed so that they cannot be re-traversed.
    // When no more paths are found, reversed edges are max matching.
    int pathFound = TRUE;
    int *visited;
    int lines = 0;
    while (pathFound) {
        // Finding path from source to sink.
        visited = malloc(adjSize * sizeof(int));
        for (i = 0; i < adjSize; i++) visited[i] = -1;
        pathFound = dfs(adjMat, adjSize, visited, sourceInd, sinkInd);
        // Reverses the edges in path.
        if (pathFound)
            reversePath(visited, adjMat, adjSize);
        lines++;
    }
    
    return lines-1;
}

int **coverLines(double **cost, int **adjMat, int numURLs)
{
    int i, j;
    int **coverMatrix = malloc(numURLs * sizeof(int *));
    for (i = 0; i < numURLs; i++) 
        coverMatrix[i] = calloc(numURLs, sizeof(int));
    
    int **ticked = malloc(2 * sizeof(int *));
    for (i = 0; i < 2; i++)
        ticked[i] = calloc(numURLs, sizeof(int));
    // Set all rows to be true.
    for (i = 0; i < 1; i++) {
        for (j = 0; j < numURLs; j++)
            ticked[i][j] = TRUE;
    }

    // Ticking all unassigned rows.
    for (i = 0; i < numURLs; i++) {
        for (j = 0; j < numURLs; j++) {
            // Untick rows already assigned.
            if (adjMat[i][numURLs+j] == -1)
                ticked[0][i] = FALSE;
        }
    }

    // Tick all unticked cols that have zero in ticked rows.
    int row, col;
    for (row = 0; row < numURLs; row++) {
        // for every ticked row.
        if (ticked[0][row]) {
            // for every element in that row.
            for (col = 0; col < numURLs; col++) {
                // Tick cols with a 0 in a ticked row.
                if (cost[row][col] == 0.0)
                    ticked[1][col] = TRUE;
            }
        }
    }

    // Tick all unticked rows that have zeros in ticked cols.
    for (col = 0; col < numURLs; col++) {
        if (ticked[1][col]) {
            for (row = 0; row < numURLs; row++) {
                if (cost[row][col] == 0.0)
                    ticked[0][row] = TRUE;
            }
        }
    }

    // Draw line through ticked cols and unticked rows.
    for (i = 0; i < numURLs; i++) {
        for (j = 0; j < numURLs; j++) {
            if (ticked[1][j] && !ticked[0][i])
                coverMatrix[i][j] = 2;
            else if (ticked[1][j] || !ticked[0][i])
                coverMatrix[i][j] = 1;
        }
    }

    return coverMatrix;
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

    // cost[0] = {82 83 69 92};
    // cost[1] = {77 37 49 92};
    // cost[2] = {11 69 5 86};
    // cost[3] = {8 9 98 23};
    // int j;
    // double num;
    // numURLs = 4;
    // for (i = 0; i < numURLs; i++) {
    //     for (j = 0; j < numURLs; j++) {
    //         printf("scanning: ");
    //         scanf("%lf", &num);
    //         cost[i][j] = num;
    //         ogCost[i][j] = num;
    //         printf("\n");
    //     }
    // }

    showMatrix(cost, numURLs);
    /* 3. Subtract row minima */
    rowReduce(cost, numURLs);
    /* 4. Subtract col minima */
    colReduce(cost, numURLs);

    // /* 5. Count number of lines L required to cover all the 0s */
    int **adjMat = buildAdjMat(cost, numURLs);
    while (numLinesToCoverZeroes(cost, numURLs, adjMat) != numURLs) {
        int **coverMatrix = coverLines(cost, adjMat, numURLs);
        /* 6. Find smallest number from uncovered area */
        double min = findUncoveredAreaMin(cost, coverMatrix, numURLs);
        /* Subtract this number from all UNCOVERED ROWS */
        rowReduceUncovered(coverMatrix, min, cost, numURLs);
        /* Add new smallest number to COVERED COLS */
        colAddCovered(coverMatrix, min, cost, numURLs);
        /* Go back to step 5 and repeat */
        adjMat = buildAdjMat(cost, numURLs);
    }
    // showMatrix(cost, numURLs);
    showMatrix(cost, numURLs);
    showMatrix(ogCost, numURLs);
    double sum = getURLOrder(cost, files, numURLs - 1, ogCost); // sort normally
    printf("%f\n", sum);
    for (i = 0; i < numURLs; i++) {
        printf("%s\n", files[i]->fileName);
    }

    // Cleaning up.
    disposeSet(unionURL);
    freeDoubleMatrix(cost, numURLs);
    freeDoubleMatrix(ogCost, numURLs);
    freeFiles(files, numURLs);

    return 0;
}


