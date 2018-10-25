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
#include "mystring.h"

#define MAX_LINE 1001
#define URL_LENGTH      55
#define SHIFT 1
#define NULL_TERM 1
#define MAX_OUTPUT 30
#define TFIDF 0
#define SEARCHTERMS 1

typedef struct TFIDFNode *TFNode;

struct TFIDFNode {
    char *name;
    double tfIdf;
    int searchTerms;
};


void disposeTfIdf(TFNode *URLTfIdf, int totalURLs)
{
    int i;
    for (i = 0; i < totalURLs; i++) {
        free(URLTfIdf[i]->name);
        free(URLTfIdf[i]);
    }
    free(URLTfIdf);
}


/* Prints the tfidf to stdout */
void printTfIdf(TFNode *array, int size)
{
    int i;
    // Outputs only top 30.
    for(i = 0; i < size && i < MAX_OUTPUT; i++) {
        if (array[i]->tfIdf != 0)
            printf("%s %.6f\n", array[i]->name, array[i]->tfIdf);
            // printf("%s %.6f %d\n", array[i]->name, array[i]->tfIdf, array[i]->searchTerms);
    }
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
    fclose(invIndex);
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
        if (strcmp(found, "") != 0) {
            char *str = normalise(found);
            if (strcmp(str, wanted) == 0) searchCount++;
            wordCount++;
            free(str);
        }
    }
    free(wanted); free(dump); free(urls);

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
    newTFNode->name  = mystrdup(URLName);
    newTFNode->tfIdf = 0;
    newTFNode->searchTerms = 0;
    return newTFNode;
}

// helper function for the Merge Sort
void TFmerge(int searchBy, TFNode *array, int start, int middle, int end)
{
    int leftLength = middle - start + SHIFT;
    int rightLength = end - middle;

    // split given array in half
    TFNode *left = malloc(sizeof(TFNode) * leftLength);
    TFNode *right = malloc(sizeof(TFNode) * rightLength);
    for (int i = 0; i < leftLength; i++) left[i] = array[start + i];
    for (int i = 0; i < rightLength; i++) right[i] = array[middle + 1 + i];

    // merging back in order
    int i = 0, j = 0, k = start;
    while (i < leftLength && j < rightLength) {
        if (left[i]->tfIdf >= right[j]->tfIdf && searchBy == TFIDF) {
            array[k] = left[i];
            i++;
		} else if (left[i]->searchTerms >= right[j]->searchTerms && searchBy == SEARCHTERMS) {
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


void countOccurences(char **URLs, TFNode *URLTfIdf, int elems)
{
	int i, j;
	for (i = 0; i < elems; i++) {
		for (j = 0; URLs[j] != NULL; j++) {
			if (strcmp(URLTfIdf[i]->name, URLs[j]) == 0) URLTfIdf[i]->searchTerms++;
		}
	}
}


void countSearchTerms(int argc, char **argv, TFNode *URLTfIdf, int elems)
{
    int i;
	for (i = 1; i < argc; i++) {
		char **URLs = getURLs(argv[i]);
		if (URLs == NULL) continue;
		countOccurences(URLs, URLTfIdf, elems);
		freeTokens(URLs);
	}
}


// Merge Sort that is used to order the URLS by tfidf and search terms
void TFmergeSort(int searchBy, TFNode *array, int start, int end)
{
    if (start < end) {
        // same as (start + end)/2, but avoids overflow for large 
        // numbers
        int middle = start + (end - start)/2;
        // sort the two halves of the array
        TFmergeSort(searchBy, array, start, middle);
        TFmergeSort(searchBy, array, middle + SHIFT, end);
        // merge these sorted halves
        TFmerge(searchBy, array, start, middle, end);

    }
}

int main(int argc, char **argv) 
{
    double tf, idf, tfIdf;
    char **URLs; // Array of URL names.
    // char *search;
    // int  nURLs;
    TFNode *URLTfIdf;
    int i;
    // int index;

    int nSearchwords = argc - 1;
    Set URLList = getCollection();
    int totalURLs = nElems(URLList);

    // Inserts all search words into a set.
    Set searchWords = newSet();
    for (i = 0; i < nSearchwords; i++)
        insertInto(searchWords, argv[i+1]);

    // Array of size nURLs to keep track of tf-idf of each URL.
    URLTfIdf = malloc(totalURLs * sizeof(TFNode));
    
    // For each URL, calcualte tf-idf.
    SetNode word, currURL = URLList->elems;
    for (i = 0; i < totalURLs; i++) {
        tfIdf = 0;
        // For each search word wanted, sum up tf-idf for each search word.
        for (word = searchWords->elems; word != NULL; word = word->next) {
            URLs = getURLs(word->val);
            if (!URLs) continue;
            tf = calcTf(currURL->val, word->val);
            idf = calcIdf(numURLs(URLs), totalURLs);
            tfIdf += tf * idf;
            freeTokens(URLs);
        }
        // Set a new tfidf struct for a URL.
        URLTfIdf[i] = newTFIDFNode(currURL->val);
        URLTfIdf[i]->tfIdf = tfIdf;

        currURL = currURL->next;
    }
    countSearchTerms(argc, argv, URLTfIdf, totalURLs);
    // sort the ADT by tfidf
	TFmergeSort(TFIDF, URLTfIdf, 0, totalURLs-SHIFT);
    // sort the ADT again by number of search terms each URL contains
	TFmergeSort(SEARCHTERMS, URLTfIdf, 0, totalURLs-SHIFT);
    // sort URLS by Tfidf
    printTfIdf(URLTfIdf, totalURLs);

    // free memory
    disposeSet(searchWords);
    disposeSet(URLList);
    disposeTfIdf(URLTfIdf, totalURLs);

    return 0;
}