#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "RedBlackTree.h"
void printNode(RedBlackBST* self, Node* node);
void testBST(RedBlackBST* self, int test_size);

int main()
{
	RedBlackBST st = { .root = NULL }; 

	testBST(&st, 20);

	printf("Press enter to exit...");
	getc(stdin);

	RBT_free(&st);
}

void printNode(RedBlackBST* self, Node* node)
{
	printf("%d %s at %p => (%p)\n", node->key, *RBT_get(self, node->key), node, node->val);
}

void testBST(RedBlackBST* self, int test_size)
{
	KeyList *q;
	int i;

	// Add test_size nodes to the tree
	for (i = 1; i <= test_size; i++)
	{
		char val[2];
		sprintf(val, "%c", i % ((int)'Z' - 'A' - 1) + 'A' - 1);

		RBT_put(self, i, val);
	}

	// Print the in order traversal
	q = RBT_keys(self);
	KL_forEach(self, q, printNode);

	// Remove in order
	for (i = 1; i <= test_size; i++)
	{
		RBT_remove(self, i);
		KeyList *list = RBT_keys(self);
		printf("---\n");
		KL_forEach(self, list, printNode);
	}

	// Fill the tree back up for another test
	for (i = 1; i <= test_size; i++)
	{
		char val[2];
		sprintf(val, "%c", i % ((int)'Z' - 'A' - 1) + 'A' - 1);

		RBT_put(self, i, val);
	}

	// Print the in order traversal
	q = RBT_keys(self);
	KL_forEach(self, q, printNode);
	
	// Remove the nodes by removing the root one at a time
	while (!RBT_isEmpty(self))
	{
		RBT_remove(self, self->root->key);
		KeyList *list = RBT_keys(self);
		printf("---\n");
		KL_forEach(self, list, printNode);
	}
}