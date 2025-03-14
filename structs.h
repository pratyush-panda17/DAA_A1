#ifndef TREE_H
#define TREE_H

typedef struct {
    int x;
    int y;
} Point;

typedef struct Node {
    int height;
    int layer_index;
    int y_val;
    int x_val;
    Point *layer;        // Array of tuples
    int array_size;      // Size of the array
    struct Node *left;   // Fixed struct reference (not TreeNode)
    struct Node *right;
} Node;

#endif
