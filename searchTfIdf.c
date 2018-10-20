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

char **getURLs(char *word);
double calcTf(char *URLName, char *word);
double calcIdf(char **URLs, int totalURLs);

int main(int argc, char **argv) 
{
    double tf, idf, tfIdf;
    char **URLs; // Array of URL names.
    char *search;

    int nSearchwords = argc - 1;
    Set URLList = getCollection();
    int totalURLs = nElems(URLList);
    // For each search word in command line argument.
    int i, j;
    for(i = 0; i < nSearchwords; i++) {
        search = argv[i+1];
        URLs = getURLs(search);
        // For each URL containing the word, calculate tf-idf
        // and then insert into a sorted list.
        for(j = 0; URLs[j] != NULL; j++) {
            tf = calcTf(URLs[j], search);
            idf = calcIdf(URLs, totalURLs);
            tfIdf = tf * idf;
            // insertTfIdf(sortedTfIdf, tfIdf);
        }
    }

    return 0;
}

/* Gets the URLs that contain word. */
char **getURLs(char *word) 
{
    FILE *invIndex = fopen("invertedIndex.txt", "r");
    if (!invIndex) { perror("fopen failed"); exit(EXIT_FAILURE); }

    char line[MAX_LINE] = {0};
    char *urlString = NULL;
    char **urls = NULL;
    while (fgets(line, MAX_LINE, invIndex) != NULL) {
        // Finds the wanted word.
        if (strstr(line, word) != NULL) {
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
double calcIdf(char **URLs, int totalURLs)
{
    // Count num of URLs containing the word.
    int i;
    double count = 0;
    for(i = 0; URLs[i] != NULL; i++) count++;

    return log10(count/totalURLs);
}


