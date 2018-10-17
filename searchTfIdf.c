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
#include "set.h"
#include "readData.h"

int main(int argc, char **argv) 
{
    float tf, idf, tfIdf;
    // For each search word in command line argument.
    int i;
    for(i = 0; i < argc; i++) {
        getURLs(argv[i+1]); // Gets the URLs for search word.
    }

    return 0;
}

/* Gets the URLs that contain word. */
Set getURLs(char *word) {
    FILE *invIndex = fopen("invertedIndex.txt", "r");
    if (!invIndex) { perror("fopen failed"); exit(EXIT_FAILURE); }
    // Finds the wanted word.


}

/* Calculates how frequently a term appears in a url. */
float tf(FILE *URLFile, char *word) 
{
    
}