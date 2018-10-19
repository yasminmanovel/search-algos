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
#include "set.h"
#include "graph.h"
#include "BSTree.h"
#include "readData.h"

#define MAX_LINE 1000

char **getURLs(char *word);
float calcTf(char *fileName, char *word);
float calcIdf();

int main(int argc, char **argv) 
{
    float tf, idf, tfIdf;
    char **URLs; // Array of URL names.
    char *search;
    int nSearchwords = argc - 1;
    // For each search word in command line argument.
    int i, j;
    for(i = 0; i < nSearchwords; i++) {
        search = argv[i+1];
        URLs = getURLs(search);
        // For each URL containing the word, calculate tf-idf
        // and then insert into a sorted list.
        for(j = 0; URLs[j] != NULL; j++) {
            tf = calcTf(URLs[j], search);
            idf = calcIdf();
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
float calcTf(char *fileName, char *word) 
{
    
    return 1.0;
}

/* Calculates the idf for a term. */
float calcIdf()
{

}


