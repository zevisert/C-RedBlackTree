#include "RedBlackTreeNode.h"
#include <stdlib.h>
#include <string.h>

#define MAX(a,b) (a > b) ? a : b

//#define ASSERTS

#ifdef ASSERTS
#include <assert.h>
#else
#define assert(X) {/* Asserts are unused unless defined */}
#endif // ASSERTS

#pragma region Private NODE_* functions

// Release any memory associated with this node
void NODE_free(Node** x)
{
	free((*x)->val);
	(*x)->val = NULL;

	free((*x));
	(*x) = NULL;
}

bool NODE_isRed(const Node* x)
{
	if (x == NULL) return false;
	return x->color == RED;
}

/* the smallest key in subtree rooted at x; NULL if no such key */
Node* NODE_min_bykey(Node* x)
{
	assert(x != NULL);
	if (x->left == NULL) return x;
	return NODE_min_bykey(x->left);
}

/* value associated with the given key in subtree rooted at x; NULL if no such key */
Value* NODE_get(Node* x, Key key)
{
	while (x != NULL)
	{
		if (key < x->key) x = x->left;
		else if (key > x->key) x = x->right;
		else return &x->val;
	}
	return NULL;
}

/* number of node in subtree rooted at x; 0 if x is NULL */
int NODE_size(const Node* x)
{
	if (x == NULL) return 0;
	return x->size;
}

/* the largest key in the subtree rooted at x; NULL if no such key */
Node* NODE_max_bykey(Node* x)
{
	assert(x != NULL);
	if (x->right == NULL) return x;
	else return NODE_max_bykey(x->right);
}

/* make a left-leaning link lean to the right */
Node* NODE_rotateRight(Node* h)
{
	assert( (h != NULL) && NODE_isRed(h->left));
	Node* x = h->left;
	h->left = x->right;
	x->right = h;
	x->color = x->right->color;
	x->right->color = RED;
	x->size = h->size;
	h->size = NODE_size(h->left) + NODE_size(h->right) + 1;
	return x;
}

// make a right-leaning link lean to the left
Node* NODE_rotateLeft(Node* h)
{
	assert( (h != NULL) && NODE_isRed(h->right));
	Node* x = h->right;
	h->right = x->left;
	x->left = h;
	x->color = x->left->color;
	x->left->color = RED;
	x->size = h->size;
	h->size = NODE_size(h->left) + NODE_size(h->right) + 1;
	return x;
}

// flip the colors of a node and its two children
void NODE_flipColors(Node* h)
{
	// h must have opposite color of its two children
	assert((h != NULL) && (h->left != NULL) && (h->right != NULL));
	assert((!NODE_isRed(h) &&  NODE_isRed(h->left) &&  NODE_isRed(h->right)) || (NODE_isRed(h)  && !NODE_isRed(h->left) && !NODE_isRed(h->right)));

	h->color = !h->color;
	h->left->color = !h->left->color;
	h->right->color = !h->right->color;
}

// Assuming that h is red and both h.left and h.left.left
// are black, make h.left or one of its children red.
Node* NODE_moveRedLeft(Node* h)
{
	assert(h != NULL);
	assert(NODE_isRed(h) && !NODE_isRed(h->left) && !NODE_isRed(h->left->left));

	NODE_flipColors(h);
	if (NODE_isRed(h->right->left))
	{
		h->right = NODE_rotateRight(h->right);
		h = NODE_rotateLeft(h);
		NODE_flipColors(h);
	}
	return h;
}

// Assuming that h is red and both h.right and h.right.left
// are black, make h.right or one of its children red.
Node* NODE_moveRedRight(Node* h)
{
	assert(h != NULL);
	assert(NODE_isRed(h) && !NODE_isRed(h->right) && !NODE_isRed(h->right->left));

	NODE_flipColors(h);
	if (NODE_isRed(h->left->left))
	{
		h = NODE_rotateRight(h);
		NODE_flipColors(h);
	}
	return h;
}

// restore red-black tree invariant
Node* NODE_balance(Node* h)
{
	assert(h != NULL);

	if (NODE_isRed(h->right))
	{
		h = NODE_rotateLeft(h);
	}
	if (NODE_isRed(h->left) && NODE_isRed(h->left->left))
	{
		h = NODE_rotateRight(h);
	}

	if (NODE_isRed(h->left) && NODE_isRed(h->right))
	{
		NODE_flipColors(h);
	}

	h->size = NODE_size(h->left) + NODE_size(h->right) + 1;
	return h;
}

// delete the key-value pair with the minimum key rooted at h
Node* NODE_deleteMin(Node* h)
{
	if (h->left == NULL)
	{
		NODE_free(&h);
		return h;
	}

	if (!NODE_isRed(h->left) && !NODE_isRed(h->left->left))
	{
		h = NODE_moveRedLeft(h);
	}

	h->left = NODE_deleteMin(h->left);
	return NODE_balance(h);
}

// delete the key-value pair with the maximum key rooted at h
Node* NODE_deleteMax(Node* h)
{
	if (NODE_isRed(h->left))
		h = NODE_rotateRight(h);

	if (h->right == NULL)
	{
		NODE_free(&h);
		return NULL;
	}

	if (!NODE_isRed(h->right) && !NODE_isRed(h->right->left))
		h = NODE_moveRedRight(h);

	h->right = NODE_deleteMax(h->right);

	return NODE_balance(h);
}

// delete the key-value pair with the given key rooted at h
Node* NODE_remove(Node* h, Key key)
{
	 assert(NODE_get(h, key) != NULL);

	if (key < h->key)
	{
		if (!NODE_isRed(h->left) && !NODE_isRed(h->left->left))
			h = NODE_moveRedLeft(h);
		h->left = NODE_remove(h->left, key);
	}
	else
	{
		if (NODE_isRed(h->left))
		{
			h = NODE_rotateRight(h);
		}
		if (key == h->key && (h->right == NULL))
		{
			NODE_free(&h);
			return NULL;
		}
		if (!NODE_isRed(h->right) && !NODE_isRed(h->right->left))
		{
			h = NODE_moveRedRight(h);
		}
		if (key == h->key)
		{
			Node* x = NODE_min_bykey(h->right);
			h->key = x->key;

			// x will be freed in a moment: make sure it's data is freed
			Value temp = h->val;
			h->val = x->val;
			x->val = temp;

			h->right = NODE_deleteMin(h->right);
		}
		else h->right = NODE_remove(h->right, key);
	}
	return NODE_balance(h);
}

// insert the key-value pair in the subtree rooted at h
Node* NODE_put(Node* h, Key key, Value val)
{
	if (h == NULL) return CreateNode(key, val, RED, 1);

	if (key < h->key)
	{
		h->left = NODE_put(h->left, key, val);
	}
	else if (key > h->key)
	{
		h->right = NODE_put(h->right, key, val);
	}
	else
	{
		// Replace the key with a new value
		memcpy(&h->val, val, sizeof(Value));
	}

	// fix-up any right-leaning links
	if (NODE_isRed(h->right) && !NODE_isRed(h->left))
	{
		h = NODE_rotateLeft(h);
	}
	if (NODE_isRed(h->left) && NODE_isRed(h->left->left))
	{
		h = NODE_rotateRight(h);
	}
	if (NODE_isRed(h->left) && NODE_isRed(h->right))
	{
		NODE_flipColors(h);
	}

	h->size = NODE_size(h->left) + NODE_size(h->right) + 1;

	return h;
}

int NODE_height(Node* x)
{
	if (x == NULL) return -1;
	return 1 + (MAX(NODE_height(x->left), NODE_height(x->right)));
}

// the largest key in the subtree rooted at x less than or equal to the given key
Node* NODE_floor(Node* x, Key key)
{
	if (x == NULL) return NULL;
	if (key == x->key) return x;
	if (key < x->key)  return NODE_floor(x->left, key);

	Node* t = NODE_floor(x->right, key);
	if (t != NULL) return t;
	else return x;
}


// the smallest key in the subtree rooted at x greater than or equal to the given key
Node* NODE_ceiling(Node* x, Key key)
{
	if (x == NULL) return NULL;
	if (key == x->key) return x;
	if (key > x->key)  return NODE_ceiling(x->right, key);
	Node* t = NODE_ceiling(x->left, key);
	if (t != NULL) return t;
	else           return x;
}

// the key of rank k in the subtree rooted at x
Node* NODE_select(Node* x, int k)
{
	assert(x != NULL);
	assert(k >= 0 && k < NODE_size(x));
	int t = NODE_size(x->left);
	if (t > k) return NODE_select(x->left, k);
	if (t < k) return NODE_select(x->right, k - t - 1);
	return x;
}

// number of keys less than key in the subtree rooted at x
int NODE_rank(Key key, Node* x)
{
	if (x == NULL) return 0;
	if (key < x->key) return NODE_rank(key, x->left);
	else if (key > x->key) return 1 + NODE_size(x->left) + NODE_rank(key, x->right);
	else return NODE_size(x->left);
}

// add the keys between lo and hi in the subtree rooted at x to the queue
void NODE_keys(Node* x, KeyList** queue, const Key lo, const Key hi)
{
	if (x == NULL || queue == NULL) return;
	if (lo < x->key) NODE_keys(x->left, queue, lo, hi);
	if (lo <= x->key && hi >= x->key)
	{
		KeyList* next = (KeyList*)calloc(1, sizeof(KeyList));
		(*queue)->node = x;
		(*queue)->next = next;
		(*queue) = next;
	}
	if (hi > x->key) NODE_keys(x->right, queue, lo, hi);
}

#pragma region Node Tests

// is the tree rooted at x a BST with all keys strictly between min and max
// (if min or max is NULL, treat as empty constraint)
// Credit: Bob Dondero's elegant solution
bool NODE_test_isBST(const Node* x, const Key* min, const Key* max)
{
	if (x == NULL) return true;
	if (min != NULL && x->key <= *min) return false;
	if (max != NULL && x->key >= *max) return false;
	return NODE_test_isBST(x->left, min, &x->key) && NODE_test_isBST(x->right, &x->key, max);
}

bool NODE_test_isSizeConsistent(const Node* x)
{
	if (x == NULL) return true;
	if (x->size != NODE_size(x->left) + NODE_size(x->right) + 1) return false;
	return NODE_test_isSizeConsistent(x->left) && NODE_test_isSizeConsistent(x->right);
}

bool NODE_test_is23(const Node* x, const Node* root)
{
	if (x == NULL) return true;
	if (NODE_isRed(x->right)) return false;
	if (x != root && NODE_isRed(x) && NODE_isRed(x->left))
		return false;
	return NODE_test_is23(x->left, root) && NODE_test_is23(x->right, root);
}

// does every path from the root to a leaf have the given number of black links?
bool NODE_test_isBalanced(const Node* x, int black)
{
	if (x == NULL) return black == 0;
	if (!NODE_isRed(x)) black--;
	return NODE_test_isBalanced(x->left, black) && NODE_test_isBalanced(x->right, black);
}

#pragma endregion

#pragma endregion
