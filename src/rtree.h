#include <stdlib.h>
#define M 4 // the maximum number of indices a node can contain
#define m 2 // the minimum number of indices a node can contain

// Point
typedef struct point
{
    int x;
    int y;
} Point;
typedef Point* POINT;

// Constructor for Point struct
POINT newPoint(int x, int y)
{
    POINT newPoint = (POINT) malloc(sizeof(Point));
    newPoint->x = x;
    newPoint->y = y;
    return newPoint;
}

// Rectangle - min , max should be pointer or change it to simple struct
typedef struct rectangle
{
    POINT min; // bottom left vertex
    POINT max; // upper right vertex
} Rectangle;
typedef Rectangle* RECTANGLE;

// Constructor for Rectangle struct
RECTANGLE newRectangle(POINT min, POINT max)
{
    RECTANGLE newRect = (RECTANGLE) malloc(sizeof(Rectangle));
    newRect->min = min;
    newRect->max = max;
    return newRect;
}

// Index Pointer
typedef struct index* INDEX;

// Node Pointer
typedef struct node* NODE;

// Node
typedef struct node
{
    INDEX I[4];
    int size;
} Node;

// Constructor for Node struct
NODE newNode()
{
    NODE newNode = (NODE) malloc(sizeof(Node));
    for (int i = 0; i < M; i++) newNode->I[i] = NULL;
    newNode->size = 0;
    return newNode;
}

// Index 
typedef struct index
{
    RECTANGLE rect; // the MBR of all rectangles present in the child node of this index
    int lhv; // the Largest Hilbert Value among child indices
    NODE child; // a pointer to the child node of this index
} Index;

// Constructor for Index struct
INDEX newIndex(RECTANGLE rect, int lhv, NODE child)
{
    INDEX newIndex = (INDEX) malloc(sizeof(Index));
    newIndex->rect = rect;
    newIndex->lhv = lhv;
    newIndex->child = child;
    return newIndex;
}

typedef struct tree
{
    NODE root;
} Tree;
typedef Tree* TREE;