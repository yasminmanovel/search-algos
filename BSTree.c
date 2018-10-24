/* BSTree.c
 * Taken from COMP2521 lab10.
 * Modified by Selina and Yasmin for COMP2521 ass2.
 * Group name: duckduckgo
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "BSTree.h"
#include "mystrdup.h"

#define TRUE  1
#define FALSE 0

typedef struct BSTNode *BSTLink;

typedef struct BSTNode {
	char *value;
	listNode *urlList;
	BSTLink left, right;
} BSTNode;

// frees memory associated with urlList in a BSTree node.
static void freeurlList(BSTree t) 
{
	listNode *curr = t->urlList;
	while (curr != NULL) {
		listNode *temp = curr;
		curr = curr->next;
		free(temp->url);
		free(temp);
	}
}

// make a new list node containing url.
static listNode *newListNode(char *url) 
{
	listNode *new = malloc(sizeof(struct listNode));
	assert(new != NULL);
	new->url = mystrdup(url);
	new->next = NULL;
	return new;
}

// make a new node containing a value
static BSTLink newBSTNode(char *str, char *url)
{
	BSTLink new = malloc(sizeof(BSTNode));
	assert(new != NULL);
	new->value = mystrdup(str);
	new->urlList = newListNode(url);
	new->left = new->right = NULL;
	return new;
}

// create a new empty BSTree
BSTree newBSTree()
{
	return NULL;
}

// free memory associated with BSTree
void dropBSTree(BSTree t)
{
	if (t == NULL) return;
	dropBSTree(t->left);
	dropBSTree(t->right);
	freeurlList(t);
	free(t->value);
	free(t);
}

// display BSTree root node
void showBSTreeNode(FILE * out, BSTree t)
{
	if (t == NULL) return;
	fprintf(out, "%s  ", t->value);
	listNode *curr = t->urlList;
	while (curr != NULL) {
		fprintf(out, "%s ", curr->url);
		curr = curr->next;
	}
	fprintf(out, "\n");
}

// print values in infix order
void BSTreeInfix(FILE *out, BSTree t)
{
	if (t == NULL) return;
	BSTreeInfix(out, t->left);
	showBSTreeNode(out, t);
	BSTreeInfix(out, t->right);
}

// Inserts a url into urlList given BSTree node and url name.
static void urlListInsert(BSTree t, char *url) 
{
	assert(t != NULL);
	int exists = FALSE; // So that we don't add duplicates.
	listNode *new = newListNode(url);
	// Get to the end of current urlList.
	listNode *curr;
	// Checks if the word already exists.
	curr = t->urlList;
	while (curr != NULL) {
		if (strcmp(curr->url, url) == 0) exists = TRUE;
		curr = curr->next;
	}
	// Iterates to the last node.
	curr = t->urlList;
	while (curr->next != NULL) curr = curr->next;
	// Only adds to list if not already in it.
	if (!exists) 
		curr->next = new;
	else {
		free(new->url);
		free(new);
	}
}

// Inserts a new string into a BSTree.
// If string is already in BSTree, it inserts it into its LL.
BSTree BSTreeInsert(BSTree t, char *str, char *url)
{
	if (t == NULL)
		return newBSTNode(str, url);

	int v = strcmp(str, t->value);
	if (v < 0)
		t->left = BSTreeInsert(t->left, str, url);
	else if (v > 0)
		t->right = BSTreeInsert(t->right, str, url);
	else // (v == t->value)
		urlListInsert(t, url);
	return t;
}

