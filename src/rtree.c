#include "hilbert.h"

POINT newPoint(float x, float y){
    POINT newPoint = (POINT) malloc(sizeof(Point));
    newPoint->x = x;
    newPoint->y = y;
    return newPoint;
}

int isLeaf(NODE n){
    return (n->I)[0]->child == NULL;
}

INDEX newIndex(RECTANGLE rect, float lhv, NODE child){
    INDEX newIndex = (INDEX) malloc(sizeof(Index));
    newIndex->rect = rect;
    newIndex->lhv = lhv;
    newIndex->child = child;
    return newIndex;
}

RECTANGLE newRectangle(POINT min, POINT max){
    RECTANGLE newRect = (RECTANGLE) malloc(sizeof(Rectangle));
    newRect->min = min;
    newRect->max = max;
    return newRect;
}

NODE newNode(){
    NODE newNode = (NODE) malloc(sizeof(Node));
    for (int i = 0; i < M; i++) newNode->I[i] = NULL;
    newNode->size = 0;
    return newNode;
}

TREE newTree(){
    NODE newNode = (NODE) malloc(sizeof(Node));
    TREE newTree = (TREE) malloc(sizeof(Tree));
    newTree->root = newNode;
    return newTree;
}

void Insert(NODE root, RECTANGLE r)
{
    float xmid=(r->min->x+r->max->x)/2.0;
    float ymid=(r->min->x+r->max->y)/2.0;

    POINT pmid=newPoint(xmid, ymid);
    NODE leaf = ChooseLeaf(root, r, hilbertValue(pmid));
    INDEX rect = newIndex(r, hilbertValue(pmid), NULL);
    
    if(leaf->size<M)
    {
        for(int i=leaf->size-1; i>=0; i--)
        {
            if(leaf->I[i]->lhv>rect->lhv) leaf->I[i+1] = leaf->I[i];
            else leaf->I[i]=rect;
        }
    }
    else
    {
        
    }
}
NODE ChooseLeaf(NODE root, RECTANGLE r, float h){
    NODE n = root;
    if(isLeaf(n))
        return n;
    
    for(int i=0; i<n->size; i++){
        if(n->I[i]->lhv > h){
            return ChooseLeaf(n->I[i]->child, r, h);
        }
    }
}

void HandleOverflow(NODE root, NODE n, RECTANGLE r){
    
}