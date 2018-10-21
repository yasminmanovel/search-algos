/* searchTfIdf.c
 *
 * Written by Selina (z5208109) & Yasmin (z5207093)
 * Group: duckduckgo
 * Start Date: 10/10/18
 * 
 * Description:
 * Content based search engine that uses tf-idf values.
 * Rank pages based on summation of tf-idf values for all query terms. 
 * 
 * tf = term frequency = (# of appearance) / (total num of words in doc)
 * idf = accounts for ratio of documents that include word
 *     = log( (total docs)/(# of docs word appears in) )
 * tf-idf = tf * idf
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include "set.h"
#include "graph.h"
#include "BSTree.h"
#include "readData.h"

#define MAX_LINE 1000
#define URL_LENGTH      55
#define SHIFT 1
#define NULL_TERM 1
#define MAX_OUTPUT 30

typedef struct TFIDFNode *TFNode;

struct TFIDFNode {
    char *name;
    double tfIdf;
};

char **getURLs(char *word);
double calcTf(char *URLName, char *word);
double calcIdf(int nURLs, int totalURLs);
void TFMerge(TFNode *array, int start, int middle, int end);
void TFmergeSort(TFNode *array, int start, int end);
TFNode newTFIDFNode(char *URLName);
void printTfIdf(TFNode *array, int size);
int numURLs(char **URLs);


int main(int argc, char **argv) 
{
    double tf, idf, tfIdf;
    char **URLs; // Array of URL names.
    char *search;
    int  nURLs;
    TFNode *URLTfIdf;
    int i, j;
    int index;

    int nSearchwords = argc - 1;
    Set URLList = getCollection();
    int totalURLs = nElems(URLList);

    // Inserts all search words into a set.
    Set searchWords;
    for (i = 0; i < nSearchwords; i++)
        insertInto(searchWords, argv[i+1]);

    // Array of size nURLs to keep track of tf-idf of each URL.
    URLTfIdf = malloc(nURLs * sizeof(TFNode));
    
    // For each URL, calcualte tf-idf.
    Link currURL = URLList->elems;
    for (i = 0; i < totalURLs; i++) {
        



        // URLTfIdf[i] = newTFIDFNode(currURL->URLName);

        currURL = currURL->next;
    }























    return 0;
}

/*
    int nSearchwords = argc - 1;
    Set URLList = getCollection();
    int totalURLs = nElems(URLList);
    // For each search word in command line argument.
    for(i = 0; i < nSearchwords; i++) {
        search = argv[i+1];
        // Gets URLs containing search word.
        URLs = getURLs(search);
        if (!URLs) continue; // If not found, look at next searchword.
        nURLs = numURLs(URLs);
        URLTfIdf = malloc(nURLs * sizeof(TFNode));
        if (!URLTfIdf) { perror("malloc failed"); exit(EXIT_FAILURE); }
        // For each URL containing word, calc tf-idf and insert into array.
        index = 0;
        for(j = 0; URLs[j] != NULL; j++) {
            tf = calcTf(URLs[j], search);
            idf = calcIdf(nURLs, totalURLs);
            tfIdf = tf * idf;
            // Inserts into array.
            URLTfIdf[index] = newTFIDFNode(URLs[j]);
            URLTfIdf[index]->tfIdf = tfIdf;
            index++;
        }
        // Sorts the array of TFNodes and prints.
        TFmergeSort(URLTfIdf, 0, nURLs-1);
        printTfIdf(URLTfIdf, nURLs-1);
    }

    return 0;
}
*/

/* Prints the tfidf to stdout */
void printTfIdf(TFNode *array, int size)
{
    int i;
    // Outputs only top 30.
    for(i = size; i >= 0 && i >= (size - MAX_OUTPUT); i--)
        printf("%s %.6f\n", array[i]->name, array[i]->tfIdf);
}

/* Gets number of URLs containing the word. */
int numURLs(char **URLs)
{
    int i, URLcount = 0;
    for(i = 0; URLs[i] != NULL; i++)
        URLcount++;
    return URLcount;
}

/* Gets the URLs that contain word. */
char **getURLs(char *word) 
{
    FILE *invIndex = fopen("invertedIndex.txt", "r");
    if (!invIndex) { perror("fopen failed"); exit(EXIT_FAILURE); }

    char line[MAX_LINE] = {0};
    char lineWord[MAX_LINE] = {0};
    char *urlString = NULL;
    char **urls = NULL;
    while (fgets(line, MAX_LINE, invIndex) != NULL) {
        sscanf(line, "%s", lineWord);
        // Finds the wanted word.
        if (strcmp(lineWord, word) == 0) {
            urlString = line + strlen(word); // Moves pointer to urls part.
            trim(urlString);
            // Get an array of url names.
            urls = tokenise(urlString, " ");
            break;
        }
    }
    return urls;
}


/* Calculates how frequently a term appears in a url. */
double calcTf(char *URLName, char *word) 
{
    // Opening URL.txt file.
    char fileName[URL_LENGTH] = {0};
    sprintf(fileName, "%s.txt", URLName);

    // Gets information from txt file.
    int url_size; int text_size;
    spaceRequired(fileName, &url_size, &text_size);
    char *urls = calloc(url_size, sizeof(char));
    char *text = calloc(text_size, sizeof(char));
    readPage(urls, text, fileName);

    char *dump = text; // For freeing.
    char *found;
    double wordCount = 0, searchCount = 0;
    char *wanted = normalise(word);
    // Counts total words & num of wanted word in file.
    while ((found = strsep(&text, " ")) != NULL) {
        char *str = normalise(found);
        if (strcmp(str, "") != 0) {
            if (strcmp(str, wanted) == 0) searchCount++;
            wordCount++;
        }
        free(str);
    }
    free(wanted);
    free(dump);
    // printf("%s\t search %f\t word %f\n", fileName, searchCount, wordCount);
    // printf("Tf = %f\n", searchCount/wordCount);

    return searchCount/wordCount;
}

/* Calculates the idf for a term in a file. */
double calcIdf(int nURLs, int totalURLs)
{
    return log10( (double)totalURLs / nURLs);
}

// creating a new tfidf node and returning the pointer to it
TFNode newTFIDFNode(char *URLName) 
{
    TFNode newTFNode = calloc(1, sizeof(struct TFIDFNode));
    newTFNode->name = malloc(strlen(URLName)+NULL_TERM);
    newTFNode->name = strdup(URLName);
    newTFNode->tfIdf = 0;
    return newTFNode;
}

// helper function for the Merge Sort
void TFMerge(TFNode *array, int start, int middle, int end)
{
    int i, j, k;
    
    int leftLength = middle - start + SHIFT;
    int rightLength = end - middle;

    // split given array in half
    TFNode *left = malloc(sizeof(URL) * leftLength);
    TFNode *right = malloc(sizeof(URL) * rightLength);
    for (i = 0; i < leftLength; i++) left[i] = array[start + i];
    for (i = 0; i < rightLength; i++) right[i] = array[middle + 1 + i];

    // merging back in order
    i = 0, j = 0, k = start;
    while (i < leftLength && j < rightLength) {
        if (left[i]->tfIdf <= right[j]->tfIdf) {
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
void TFmergeSort(TFNode *array, int start, int end)
{
    if (start < end) {
        // same as (start + end)/2, but apparently avoids overflow for large 
        // numbers
        int middle = start + (end - start)/2;
        // sort the two halves of the array
        TFmergeSort(array, start, middle);
        TFmergeSort(array, middle + SHIFT, end);
        // merge these sorted halves
        TFMerge(array, start, middle, end);

    }
}

