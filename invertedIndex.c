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
#include "invertedIndex.h"


int main(int argc, char **argv) 
{
    Set URLSet = getCollection();
    BSTree invList = getInvertedList(URLSet);

    FILE *invtxt = fopen("invertedIndex.txt", "w");
    BSTreeInfix(invtxt, invList);
    fclose(invtxt);

    disposeSet(URLSet);
    dropBSTree(invList);

    return 0;
}

