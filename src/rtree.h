#include <stdlib.h>
#define M 4 // the maximum number of indices a node can contain
#define m 2 // the minimum number of indices a node can contain

// Point
typedef struct point{ 
    float x;
    float y;
} Point;
// Point pointer
typedef Point* POINT;

// Constructor for Point struct
POINT newPoint(float x, float y);

// Rectangle - min , max should be pointer or change it to simple struct
typedef struct rectangle{
    POINT min; // bottom left vertex
    POINT max; // upper right vertex
} Rectangle;
// Rectangle pointer
typedef Rectangle* RECTANGLE;

// Constructor for Rectangle struct
RECTANGLE newRectangle(POINT min, POINT max);

typedef struct index* INDEX;
// Node
typedef struct node{
    INDEX I[M];
    int size;
} Node;
// Node Pointer
typedef struct node* NODE;

// Function for Node struct
NODE newNode();

// Function to check if a node is a leaf node
int isLeaf(NODE n);

// Index 
typedef struct index{
    RECTANGLE rect; // the MBR of all rectangles present in the child node of this index
    float lhv; // the Largest Hilbert Value among child indices
    NODE child; // a pointer to the child node of this index
} Index;
// Index Pointer


// Function for Index struct
INDEX newIndex(RECTANGLE rect, float lhv, NODE child);

// Tree
typedef struct tree{
    NODE root;
} Tree;
// Tree Pointer
typedef struct tree* TREE;

// Constructor for Tree struct
TREE newTree();
