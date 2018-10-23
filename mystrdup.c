/* 
 * DESCRIPTION: 
 * Writing own mystrdup function.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *mystrdup(char *word) 
{
    int len = strlen(word) + 1;
    char *dup = malloc(len);
    strcpy(dup, word);
    return dup;
}