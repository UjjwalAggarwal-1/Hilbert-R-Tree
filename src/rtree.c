#include "rtree.h"

// 
typedef struct point{
    int x;
    int y;
} Point;
typedef Point* POINT;

// 
typedef struct rectangle{
    POINT min; // bottom left vertex
    POINT max; // upper right vertex
} Rectangle;
typedef Rectangle* RECTANGLE;

// Index 
typedef struct index{
    RECTANGLE rect; // the MBR of all rectangles present in the child node of this index
    int lhv; // the Largest Hilbert Value among child indices
    NODE child; // a pointer to the child node of this index
} Index;
typedef Index* INDEX;

// 
typedef struct node{
    Index* I;
    int size;
} Node;
typedef Node* NODE;


// Constructor for Point struct
POINT newPoint(int x, int y) {
    POINT newPoint = (POINT) malloc(sizeof(Point));
    newPoint->x = x;
    newPoint->y = y;
    return newPoint;
}

// Constructor for Rectangle struct
RECTANGLE newRectangle(POINT min, POINT max) {
    RECTANGLE newRect = (RECTANGLE) malloc(sizeof(Rectangle));
    newRect->min = min;
    newRect->max = max;
    return newRect;
}

// Constructor for Index struct
INDEX newIndex(RECTANGLE rect, int lhv, NODE child) {
    INDEX newIndex = (INDEX) malloc(sizeof(Index));
    newIndex->rect = rect;
    newIndex->lhv = lhv;
    newIndex->child = child;
    return newIndex;
}

// Constructor for Node struct
NODE newNode() {
    NODE newNode = (NODE) malloc(sizeof(Node));
    INDEX i_ptr = (INDEX) malloc(sizeof(Index)*M);
    // Index I[4];
    newNode->size = 0;
    newNode->I = i_ptr;
    return newNode;
}
