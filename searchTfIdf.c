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

#define MAX_LINE 1000

Set getURLs(char *word);
float tf(FILE *URLFile, char *word);
static char **tokenise(char *str, char *sep);

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
Set getURLs(char *word) 
{
    FILE *invIndex = fopen("invertedIndex.txt", "r");
    if (!invIndex) { perror("fopen failed"); exit(EXIT_FAILURE); }

    char line[MAX_LINE] = {0};
    char *urls = NULL;
    while (fgets(line, MAX_LINE, invIndex) != NULL) {
        // Finds the wanted word.
        if (strstr(line, word) != NULL) {
            urls = line + strlen(word); // Moves pointer to urls part.
            printf("%s\n", urls);
        }
    }
    
    return NULL;
}

/* Calculates how frequently a term appears in a url. */
float tf(FILE *URLFile, char *word) 
{
    return 1.0;
}

/* Tokenises a string based on a delimiter. 
 * Places tokens into an array of strings.
 * Written by jas for 1521 mymysh.c
 */
static char **tokenise(char *str, char *sep)
{
	// temp copy of string, because strtok() mangles it
	char *tmp;
	// count tokens
	tmp = strdup(str);
	int n = 0;
	strtok(tmp, sep); n++;
	while (strtok(NULL, sep) != NULL) n++;
	free(tmp);
	// allocate array for argv strings
	char **strings = malloc((n+1)*sizeof(char *));
	assert(strings != NULL);
	// now tokenise and fill array
	tmp = strdup(str);
	char *next; int i = 0;
	next = strtok(tmp, sep);
	strings[i++] = strdup(next);
	while ((next = strtok(NULL,sep)) != NULL)
	strings[i++] = strdup(next);
	strings[i] = NULL;
	free(tmp);
	return strings;
}