#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "RedBlackTree.h"
void printNode(RedBlackBST* self, Node* node);

int main()
{
	RedBlackBST st = { .root = NULL }; 

	int i;
	for (i = 1; i <= 5; i++)
	{
		char val[2];
		sprintf(val, "%c", i%((int)'Z'-'A') + 'A'- 1);

		RBT_put(&st, i, val);
	}

	if (!RBT_self_check(&st))
	{
		exit(EXIT_FAILURE);
	}

	for (i = 1; i <= 5; i++)
	{
		RBT_remove(&st, i);
	}

	KeyList *q = RBT_keys(&st);
	KL_forEach(&st, q, printNode);

	printf("Press enter to exit...");
	getc(stdin);

	RBT_free(&st);
}

void printNode(RedBlackBST* self, Node* node)
{
	printf("%d %s\n", node->key, *RBT_get(self, node->key));
}