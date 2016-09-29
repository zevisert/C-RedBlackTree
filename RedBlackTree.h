#pragma once

#include "stdbool.h"

typedef char* Value;
typedef int Key;

const static bool RED = 1;
const static bool BLACK = 0;

typedef struct _Node
{
	Value val;					// associated data
	struct _Node* left, *right; // links to left and right subtrees
	bool color;				    // color of parent link
	Key key;					// key
	int size;					// subtree count

} Node;

typedef struct _KeyList
{
	Node* node;
	struct _KeyList* next;
} KeyList;

#define INIT_KeyList(X) KeyList X = { .node = NULL,	.next = NULL }

typedef struct _RedBlackBST
{
	Node* root;
} RedBlackBST;

void applyKeyVal(Node* node, Key key, Value val);
void KL_forEach(RedBlackBST* self, KeyList* list, void(* func)(RedBlackBST*, Node*));
Node* CreateNode(Key _key, Value _val, bool _color, int _size);

void RBT_deleteMax(RedBlackBST* self);
void RBT_remove(RedBlackBST* self, Key key);
void RBT_put(RedBlackBST* self, Key key, Value val);

Value* RBT_get(const RedBlackBST* self, Key key);
int RBT_size(const RedBlackBST* self);
bool RBT_isEmpty(const RedBlackBST* self);
bool RBT_contains(const RedBlackBST* self, Key key);

const Node* RBT_min_bykey(const RedBlackBST* self);
const Node* RBT_max_bykey(const RedBlackBST* self);

int RBT_height(const RedBlackBST* self);
Node* RBT_floor(const RedBlackBST* self, Key key);
Node* RBT_ceiling(const RedBlackBST* self, Key key);
Key RBT_select(const RedBlackBST* self, int k);

int RBT_rank(const RedBlackBST* self, Key key);

KeyList* RBT_keys_range(const RedBlackBST* self, const Key lo, const Key hi);
KeyList* RBT_keys(const RedBlackBST* self);
int RBT_range_size(const RedBlackBST* self, Key lo, Key hi);

bool RBT_self_check(const RedBlackBST* self);
bool RBT_free(RedBlackBST* self);