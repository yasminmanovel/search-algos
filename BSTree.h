/* BSTree.h ... interface to binary search tree ADT
 * Taken from COMP2521 lab10.
 * Modified by Selina and Yasmin for COMP2521 ass2.
 * Group name: duckduckgo
 */

#ifndef BSTREE_H
#define BSTREE_H

typedef struct BSTNode *BSTree;

// create an empty BSTree
BSTree newBSTree();
// free memory associated with BSTree
void dropBSTree(BSTree);
// display BSTree root node
void showBSTreeNode(FILE *, BSTree);
// print values in infix order
void BSTreeInfix(FILE *, BSTree);
// insert a new value into a BSTree
BSTree BSTreeInsert(BSTree, char *, char *);

#endif
