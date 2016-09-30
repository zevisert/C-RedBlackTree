# C-RedBlackTree
A red black tree written in C, algorithms are ported from Sedgewick and Wayne: Algs4

`Key` and `Value` types are defined in RedBlackTree.h, these can be changed - though one should be mindful about comparisons between these data types. Furthermore, if one of the two types is not primitive you must allocate room for the data and perform a copy explicitly. The `applyKeyVal(...)` function is called from tree node creation to streamline this. Similarily, when deleting nodes the memory needs to be freed, if any. Use `NODE_free(...)` for this.

### Note:
The `Key` is set to `int` in this repo. For safety's sake if one wanted to strings as keys for example, there are checks in many functions that fire if `key == NULL`, so that NULL pointers do not arrive at node creation. This has the sideaffect of not allowing `0` as an integer key because `0 == NULL` is true in C. Keys should be > 0 if using integers.
