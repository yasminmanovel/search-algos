#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "BSTree.h"

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
		free(temp);
	}
}

// make a new list node containing url.
static listNode *newListNode(char *url) 
{
	listNode *new = malloc(sizeof(struct listNode));
	assert(new != NULL);
	new->url = strdup(url);
	new->next = NULL;
	return new;
}

// make a new node containing a value
static BSTLink newBSTNode(char *str, char *url)
{
	BSTLink new = malloc(sizeof(BSTNode));
	assert(new != NULL);
	new->value = strdup(str);
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
	free(t);
}

// count #nodes in BSTree
int BSTreeNumNodes(BSTree t)
{
	if (t == NULL)
		return 0;
	else
		return 1 + BSTreeNumNodes(t->left)
	             + BSTreeNumNodes(t->right);
}

// display BSTree root node
void showBSTreeNode(BSTree t)
{
	if (t == NULL) return;
	printf("%s  ", t->value);
	listNode *curr = t->urlList;
	while (curr != NULL) {
		printf("%s ", curr->url);
		curr = curr->next;
	}
	printf("\n");
}

// print values in infix order
void BSTreeInfix(BSTree t)
{
	if (t == NULL) return;
	BSTreeInfix(t->left);
	showBSTreeNode(t);
	BSTreeInfix(t->right);
}

// print values in prefix order
void BSTreePrefix(BSTree t)
{
	if (t == NULL) return;
	showBSTreeNode(t);
	BSTreePrefix(t->left);
	BSTreePrefix(t->right);
}

// print values in postfix order
void BSTreePostfix(BSTree t)
{
	if (t == NULL) return;
	BSTreePostfix(t->left);
	BSTreePostfix(t->right);
	showBSTreeNode(t);
}

// count #leaves in BSTree
int BSTreeNumLeaves(BSTree t)
{
    if (!t) return 0;
    if (t->left == NULL && t->right == NULL) return 1;
    return BSTreeNumLeaves(t->left) + BSTreeNumLeaves(t->right);
}

// Inserts a url into urlList given BSTree node and url name.
static void urlListInsert(BSTree t, char *url) 
{
	assert(t != NULL);
	listNode *new = newListNode(url);
	// Get to the end of current urlList.
	listNode *curr = t->urlList;
	while (curr->next != NULL) curr = curr->next;
	curr->next = new;
}

// Inserts anew string into a BSTree.
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

// check whether a value is in a BSTree
int BSTreeFind(BSTree t, char *str)
{
	if (t == NULL)
		return 0;
	
	int v = strcmp(str, t->value);
	if (v < 0)
		return BSTreeFind(t->left, str);
	else if (v > 0)
		return BSTreeFind(t->right, str);
	else // (v == t->value)
		return TRUE;
}

// delete root of tree
static BSTree deleteRoot(BSTree t)
{
	// if no subtrees, tree empty after delete
	if (t->left == NULL && t->right == NULL) {
		freeurlList(t);
		free(t);
		return NULL;
	}
	// if only right subtree, make it the new root
	else if (t->left == NULL && t->right != NULL) {
		BSTree hold = t->right;
		freeurlList(t);
        free(t);
		return hold;
	}
	// if only left subtree, make it the new root
	else if (t->left != NULL && t->right == NULL) {
        BSTree hold = t->left;
		freeurlList(t);
		free(t);
		return hold;
	}
	// else (t->left != NULL && t->right != NULL)
	// so has two subtrees
	// - find inorder successor
	// - move its value to root
	// - delete inorder successor node
	BSTLink parent = t;
	BSTLink succ = t->right; // not null!
	while (succ->left != NULL) {
		parent = succ;
		succ = succ->left;
	}
	t->value = succ->value;
	freeurlList(succ);
	free(succ);
	if (parent == t)
		parent->right = succ->right;
	else
		parent->left = succ->right;
	return t;
}

// delete a value from a BSTree
BSTree BSTreeDelete(BSTree t, char *str)
{
	if (t == NULL)
		return NULL;
	int v = strcmp(str, t->value);
	if (v < 0)
		t->left = BSTreeDelete(t->left, str);
	else if (v > 0)
		t->right = BSTreeDelete(t->right, str);
	else // (v == t->value)
		t = deleteRoot(t);
	return t;
}

