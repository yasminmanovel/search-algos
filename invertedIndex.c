/* invertedIndex.c
 *
 * Written by Selina (z5208109) & Yasmin (z5207093)
 * Group: duckduckgo
 * Start Date: 10/10/18
 * 
 * NOTES:
 * You must use BST where 
 *      key = string
 *      value = list 
 */

/* PSEUDOCODE:
 * listOfURLs = getCollection();
 * InvertedIndex invertedIdx = getInvertedList(listOfURLs);
 * 
 * Output -> invertedIdx to "invertedIndex.txt"
 */

#include <stdio.h>
#include <stdlib.h>
#include "readData.h"
#include "set.h"
#include "BSTree.h"
#include "mystring.h"


int main(int argc, char **argv) 
{
    // get Set of URLs
    Set URLSet = getCollection();
    /* Creates a list of url for each word found in urls. */
    // Create a list of urls for each word found in URL
    BSTree invList = getInvertedList(URLSet);

    // print to file
    FILE *invtxt = fopen("invertedIndex.txt", "w");
    BSTreeInfix(invtxt, invList);
    fclose(invtxt);
    // free memory
    disposeSet(URLSet);
    dropBSTree(invList);

    return 0;
}

