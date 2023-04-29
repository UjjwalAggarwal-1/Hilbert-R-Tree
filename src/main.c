#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define M 4 // the maximum number of indices a node can contain
#define m 2 // the minimum number of indices a node can contain
#define N 20

// Point
typedef struct point{ 
    float x;
    float y;
} Point;
// Point pointer
typedef Point* POINT;

// Rectangle - min , max should be pointer or change it to simple struct
typedef struct rectangle{
    POINT min; // bottom left vertex
    POINT max; // upper right vertex
} Rectangle;
// Rectangle pointer
typedef Rectangle* RECTANGLE;

typedef struct index* INDEX;
typedef struct node* NODE;

// Node
typedef struct node{
    INDEX I[M];
    int size;
} Node;

// Index 
typedef struct index{
    RECTANGLE rect; // the MBR of all rectangles present in the child node of this index
    float lhv; // the Largest Hilbert Value among child indices
    NODE child; // a pointer to the child node of this index
} Index;
// Index Pointer
// Tree
typedef struct tree{
    NODE root;
} Tree;
// Tree Pointer
typedef struct tree* TREE;

char newQuadType(char oldType, float quad){
    if(oldType=='A')
    {
        if(quad==0) return 'D';
        if(quad==1) return 'A';
        if(quad==2) return 'A';
        if(quad==3) return 'B';
    }
    if(oldType=='B')
    {
        if(quad==0) return 'B';
        if(quad==1) return 'B';
        if(quad==2) return 'C';
        if(quad==3) return 'A';
    }
    if(oldType=='C')
    {
        if(quad==0) return 'C';
        if(quad==1) return 'D';
        if(quad==2) return 'B';
        if(quad==3) return 'C';
    }
    if(oldType=='D')
    {
        if(quad==0) return 'A';
        if(quad==1) return 'C';
        if(quad==2) return 'D';
        if(quad==3) return 'D';
    }
    return 'A';
}

float quadValue(char oldType, float quad){
    if(oldType=='A')
    {
        if(quad==0) return 0;
        if(quad==1) return 1;
        if(quad==2) return 2;
        if(quad==3) return 3;
    }
    if(oldType=='B')
    {
        if(quad==0) return 2;
        if(quad==1) return 1;
        if(quad==2) return 0;
        if(quad==3) return 3;
    }
    if(oldType=='C')
    {
        if(quad==0) return 2;
        if(quad==1) return 3;
        if(quad==2) return 0;
        if(quad==3) return 1;
    }
    if(oldType=='D')
    {
        if(quad==0) return 0;
        if(quad==1) return 3;
        if(quad==2) return 2;
        if(quad==3) return 1;
    }
    return 0;
}

void hilbert(float n, float x, float y, float* res, char quadType){
    if(n==1) return;
    float p=n/2;
    float quad;
    if(x<p && y<p)
    {
        quad=0;
    }
    else if(x<p && y>=p)
    {
        quad=1;
        y-=p;
    }
    else if(x>=p && y>=p)
    {
        quad=2;
        x-=p;
        y-=p;
    }
    else
    {
        quad=3;
        x-=p;
    }
    float quadVal=quadValue(quadType,quad);
    *res=((*res)*pow(2, 2))+(quadVal);
    char newQuad=newQuadType(quadType, quad);
    hilbert(n/2,x,y,res, newQuad);
}

float hilbertValue(POINT p){
    float res=0;
    hilbert(pow(2, N), p->x, p->y, &res, 'A');
    return res;
}

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

void Insert(NODE root, RECTANGLE r){
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

void HandleOverflow(NODE root, NODE n, RECTANGLE r){
    
}

int checkIntersection(RECTANGLE r1, RECTANGLE r2){
    return (r2->min->x <= r1->max->x) && (r2->max->x >= r1->min->x) && (r2->min->y <= r1->max->y) && (r2->max->y >= r1->min->y);
}

void Search(NODE root,RECTANGLE r){
    for(int i=0; i<root->size; i++){
        if(checkIntersection(root->I[i]->rect, r)){
            if(isLeaf(root)){
                printf("%f-->", root->I[i]->lhv);
            }
            else{
                Search(root->I[i]->child, r);
            }
        }
    }
    printf("\n");
    return;
}




// main.c 


int main()
{
    POINT p = newPoint(2,10);
    float test = hilbertValue(p);
    printf("%f\n", test);
    return 0;
}