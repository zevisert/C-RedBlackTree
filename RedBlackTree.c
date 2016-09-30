#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "RedBlackTree.h"
#include "RedBlackTreeNode.h"

#define MAX(a,b) (a > b) ? a : b

//#define ASSERTS

#ifdef ASSERTS
#include <assert.h>
#else
#define assert(X) {/* Asserts are unused unless defined */}
#endif // ASSERTS

void KL_forEach(RedBlackBST* self, KeyList* list, void(* func)(RedBlackBST*, Node*))
{
	Node* node;
	while (list && list->node)
	{
		node = list->node;
		func(self, node);
		KeyList* to_release = list;
		list = list->next;
		free(to_release);
	}
	free(list);
}

void applyKeyVal(Node* node, Key key, Value val)
{
	// Integer can simply be applied
	node->key = key;

	// Must save room for string
	char* nodeBuffer = (char*)calloc(1, strlen(val));
	node->val = nodeBuffer;
	sprintf(node->val, "%s", val);
}

Node* CreateNode(Key _key, Value _val, bool _color, int _size)
{
	Node* node = calloc(1, sizeof(Node));
	memset(node, 0, sizeof(Node));
	Node stackNode = { .key = _key, .val = _val, .color = _color, .size = _size, .left = NULL, .right = NULL };

	applyKeyVal(&stackNode, _key, _val);

	memcpy(node, &stackNode, sizeof(Node));
	return (Node*)node;
}

/***************************************************************************
*  Standard BST search.
***************************************************************************/

Value* RBT_get(const RedBlackBST* self, Key key)
{
	if (key == NULL) { printf("argument to get() is NULL"); exit(EXIT_FAILURE); }
	return NODE_get(self->root, key);
}

/* Returns the number of key-value pairs in this symbol table. */
int RBT_size(const RedBlackBST* self)
{
	return NODE_size(self->root);
}

/* Is this symbol table empty? */
bool RBT_isEmpty(const RedBlackBST* self)
{
	return self->root == NULL;
}

bool RBT_contains(const RedBlackBST* self, Key key)
{
	return RBT_get(self, key) != NULL;
}

/* Returns the smallest key in the symbol table. */
const Node* RBT_min_bykey(const RedBlackBST* self)
{
	if (RBT_isEmpty(self)) { printf("called min() with empty symbol table"); exit(EXIT_FAILURE); }
	return NODE_min_bykey(self->root);
}

/* Returns the largest key in the symbol table. */
const Node* RBT_max_bykey(const RedBlackBST* self)
{
	if (RBT_isEmpty(self)) { printf("called max() with empty symbol table"); exit(EXIT_FAILURE); }
	return NODE_max_bykey(self->root);
}

/***************************************************************************
*  Red-black tree deletion.
***************************************************************************/

/* Removes the largest key and associated value from the symbol table. */
void RBT_deleteMax(RedBlackBST* self)
{
	if (RBT_isEmpty(self)) { printf("BST underflow"); exit(EXIT_FAILURE); }

	// if both children of root are black, set root to red
	if (!NODE_isRed(self->root->left) && !NODE_isRed(self->root->right))
	{
		self->root->color = RED;
	}

	self->root = NODE_deleteMax(self->root);
	if (!RBT_isEmpty(self)) self->root->color = BLACK;
	assert(RBT_self_check(self));
}

/* Removes the specified key and its associated value from this symbol table
   (if the key is in this symbol table). */
void RBT_remove(RedBlackBST* self, Key key)
{
	if (key == NULL) { printf("argument to remove() is NULL"); exit(EXIT_FAILURE); }
	if (!RBT_contains(self, key)) return;

	/* if both children of root are black, set root to red */
	if (!NODE_isRed(self->root->left) && !NODE_isRed(self->root->right))
	{
		self->root->color = RED;
	}

	self->root = NODE_remove(self->root, key);
	if (!RBT_isEmpty(self)) self->root->color = BLACK;
	assert(RBT_self_check(self));
}


/***************************************************************************
*  Red-black tree insertion.
***************************************************************************/

/* Inserts the specified key-value pair into the symbol table, overwriting the old
 value with the new value if the symbol table already contains the specified key.
 Deletes the specified key (and its associated value) from this symbol table
 if the specified value is {NULL}. */
void RBT_put(RedBlackBST* self, Key key, Value val)
{
	if (key == NULL)
	{
		printf("first argument to put() is NULL"); exit(EXIT_FAILURE);
	}
	if (val == NULL)
	{
		RBT_remove(self, key);
		return;
	}

	self->root = NODE_put(self->root, key, val);
	self->root->color = BLACK;
	assert(RBT_self_check(self));
}


/***************************************************************************
*  Utility functions.
***************************************************************************/

/* Returns the height of the BST (for debugging). */
int RBT_height(const RedBlackBST* self)
{
	return NODE_height(self->root);
}


/***************************************************************************
*  Ordered symbol table functions.
***************************************************************************/

/* Returns the largest key in the symbol table less than or equal to {key}. */
Node* RBT_floor(const RedBlackBST* self, Key key)
{
	if (key == NULL) { printf("argument to floor() is NULL"); exit(EXIT_FAILURE); }
	if (RBT_isEmpty(self)) { printf("called floor() with empty symbol table"); exit(EXIT_FAILURE); }

	Node* x = NODE_floor(self->root, key);
	if (x == NULL) return NULL;
	else return x;
}

/* Returns the smallest key in the symbol table greater than or equal to {key}. */
Node* RBT_ceiling(const RedBlackBST* self, Key key)
{
	if (key == NULL) { printf("argument to ceiling() is NULL"); exit(EXIT_FAILURE); }
	if (RBT_isEmpty(self)) { printf("called ceiling() with empty symbol table"); exit(EXIT_FAILURE); }

	Node* x = NODE_ceiling(self->root, key);
	if (x == NULL) return NULL;
	else return x;
}

/* Return the kth smallest key in the symbol table */
Key RBT_select(const RedBlackBST* self, int k)
{
	if (k < 0 || k >= RBT_size(self)) { printf("Illegal arguement"); exit(EXIT_FAILURE); }
	Node* x = NODE_select(self->root, k);
	return x->key;
}

/* Return the number of keys in the symbol table strictly less than {key}. */
int RBT_rank(const RedBlackBST* self, Key key)
{
	if (key == NULL) { printf("argument to rank() is NULL"); exit(EXIT_FAILURE); }
	return NODE_rank(key, self->root);
}


/***************************************************************************
*  Range count and range search.
***************************************************************************/

/* Returns all keys in the symbol table in the given range as a KeyList */
KeyList* RBT_keys_range(const RedBlackBST* self, const Key lo, const Key hi)
{
	if (lo == NULL) { printf("first argument to keys() is NULL"); exit(EXIT_FAILURE); }
	if (hi == NULL) { printf("second argument to keys() is NULL"); exit(EXIT_FAILURE); }

	KeyList* list = (KeyList*)calloc(1, sizeof(KeyList));
	KeyList* end = list;

	NODE_keys(self->root, &end, lo, hi);
	return list;
}

/* Returns all keys in the symbol table as a KeyList */
KeyList* RBT_keys(const RedBlackBST* self)
{
	if (RBT_isEmpty(self)) return NULL;
	return RBT_keys_range(self, RBT_min_bykey(self)->key, RBT_max_bykey(self)->key);
}

/* Returns the number of keys in the symbol table in the given range. */
int RBT_range_size(const RedBlackBST* self, Key lo, Key hi)
{
	if (lo == NULL) { printf("first argument to size() is NULL"); exit(EXIT_FAILURE); }
	if (hi == NULL) { printf("second argument to size() is NULL"); exit(EXIT_FAILURE); }

	if (lo < hi) return 0;
	if (RBT_contains(self, hi)) return RBT_rank(self, hi) - RBT_rank(self, lo) + 1;
	return RBT_rank(self, hi) - RBT_rank(self, lo);
}

/* Free the specified RBT */
bool RBT_free(RedBlackBST* self)
{
	// Free the BST
	KeyList* list = RBT_keys(self);
	while (list && list->node)
	{
		KeyList* release = list;
		list = list->next;

		free(release->node->val);
		release->node->val = NULL;

		free(release->node);
		release->node->left = NULL;
		release->node->right = NULL;
		release->node = NULL;

		free(release);
		release = NULL;
	}
	self->root = NULL;
	//free the null element in the keylist
	free(list);

	return true;
}

#pragma region RBT_Tests

/***************************************************************************
*  Check integrity of red-black tree data structure.
***************************************************************************/

/* does this binary tree satisfy symmetric order?
   Note: this test also ensures that data structure is a binary tree since order is strict */
bool RBT_test_isBST(const RedBlackBST* self)
{
	return NODE_test_isBST(self->root, NULL, NULL);
}

/* are the size fields correct? */
bool RBT_test_isSizeConsistent(const RedBlackBST* self)
{
	return NODE_test_isSizeConsistent(self->root);
}

/* check that ranks are consistent */
bool RBT_test_isRankConsistent(const RedBlackBST* self)
{
	int i = 0;
	for (; i < RBT_size(self); i++)
		if (i != RBT_rank(self, RBT_select(self, i))) return false;

	KeyList* list = RBT_keys(self);
	Node* n;

	while (list && list->node)
	{
		n = list->node;
		if (n->key != RBT_select(self, RBT_rank(self, n->key)))
		{
			/* Free the rest of the list then return false */
			while (list->next)
			{
				KeyList* release = list;
				list = list->next;
				free(release);
			}
			free(list);
			return false;
		}

		/* Move to the next item and free as we go */
		KeyList* release = list;
		list = list->next;
		free(release);
	}
	free(list);

	return true;
}

/* Does the tree have no red right links, and at most one (left)
   red links in a row on any path? */
bool RBT_test_is23(const RedBlackBST* self)
{
	return NODE_test_is23(self->root, self->root);
}

/* do all paths from root to leaf have same number of black edges? */
bool RBT_test_isBalanced(const RedBlackBST* self)
{
	/* number of black links on path from root to min */
	int black = 0;
	Node* x = self->root;
	while (x != NULL)
	{
		if (!NODE_isRed(x)) black++;
		x = x->left;
	}
	return NODE_test_isBalanced(self->root, black);
}

bool RBT_self_check(const RedBlackBST* self)
{
	bool t1, t2, t3, t4, t5;
	if (!(t1 = RBT_test_isBST(self)))            fprintf(stdout, "Not in symmetric order\n");
	if (!(t2 = RBT_test_isSizeConsistent(self))) fprintf(stdout, "Subtree counts not consistent\n");
	if (!(t3 = RBT_test_isRankConsistent(self))) fprintf(stdout, "Ranks not consistent\n");
	if (!(t4 = RBT_test_is23(self)))             fprintf(stdout, "Not a 2-3 tree\n");
	if (!(t5 = RBT_test_isBalanced(self)))       fprintf(stdout, "Not balanced\n");

	return t1 && t2 && t3 && t4 && t5;
}

#pragma endregion
