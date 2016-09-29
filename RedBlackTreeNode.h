#pragma once

#include "RedBlackTree.h"

bool	NODE_isRed(const Node* x);
Node*	NODE_min_bykey(Node* x);
Value*	NODE_get(Node* x, Key key);
int		NODE_size(const Node* x);
Node*	NODE_max_bykey(Node* x);
Node*	NODE_rotateRight(Node* h);
Node*	NODE_rotateLeft(Node* h);
void	NODE_flipColors(Node* h);
Node*	NODE_moveRedLeft(Node* h);
Node*	NODE_moveRedRight(Node* h);
Node*	NODE_balance(Node* h);
Node*	NODE_deleteMin(Node* h);
Node*	NODE_deleteMax(Node* h);
Node*	NODE_remove(Node* h, Key key);
Node*	NODE_put(Node* h, Key key, Value val);
int		NODE_height(Node* x);
Node*	NODE_floor(Node* x, Key key);
Node*	NODE_ceiling(Node* x, Key key);
Node*	NODE_select(Node* x, int k);
int		NODE_rank(Key key, Node* x);
void	NODE_keys(Node* x, KeyList** queue, const Key lo, const Key hi);

bool	NODE_test_isBST(const Node* x, const Key* min, const Key* max);
bool	NODE_test_isSizeConsistent(const Node* x);
bool	NODE_test_is23(const Node* x, const Node* root);
bool	NODE_test_isBalanced(const Node* x, int black);
