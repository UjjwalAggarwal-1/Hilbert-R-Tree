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
    NODE parent;
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
    newNode->parent = NULL;
    return newNode;
}

TREE newTree(){
    NODE newNode = (NODE) malloc(sizeof(Node));
    TREE newTree = (TREE) malloc(sizeof(Tree));
    newTree->root = newNode;
    return newTree;
}

NODE ChooseLeaf(NODE n, RECTANGLE r, float h){
    //n is the root node at which to start
    if(isLeaf(n)){
        return n;
    }
    for(int i=0; i<n->size; i++){
        if(n->I[i]->lhv > h){
            return ChooseLeaf(n->I[i]->child, r, h);
        }
    }
    return ChooseLeaf(n->I[n->size-1]->child, r, h); // 
}

NODE GetCooperatingSibling(NODE n){
    NODE parent = n->parent;
    NODE sibling = NULL;
    // n can only have siblings if its parent exists and has a size of at least 2
    if(parent && (parent->size > 1)){ 
        int i=0; 
        while(parent->I[i]->child != n) i++; // get the index of the MBR of n in its parent

        if(i==0) sibling = parent->I[i+1]->child; // if it is the first in line, we take the node to its right
        else if(i==parent->size-1) sibling = parent->I[i-1]->child; // if it is the last in line, we take the immediate left
        else{ // otherwise we check for both left and right siblings
            if(parent->I[i-1]->child->size < M) sibling = parent->I[i-1]->child; // incase the left sibling is not full
            else sibling = parent->I[i+1]->child; // in case left is full, we take the right one regardless it is full or not
        }
    }
    return sibling;
}

NODE HandleOverflow(TREE t, NODE n, INDEX ind){
    NODE root = t->root;

    // get the sibling node 
    NODE sibling = GetCooperatingSibling(n);
    // now we calculate the size of the set of indices 
    // present in n, its sibling and the new index
    int setSize = n->size + 1; 
    if(sibling) setSize += sibling->size;

    INDEX set[setSize];
    // add indices of n and sibling in sorted order
    int j = 0;

    if(sibling){
        if(n->I[0]->lhv < sibling->I[0]->lhv){    // if indices in n are smaller, append them first
            for(int i = 0; i<n->size; i++)
                set[j++] = n->I[i];
            for(int i = 0; i<sibling->size; i++)
                set[j++] = sibling->I[i];
        }
        else{                                     // otherwise, append the indices of the sibling first
            for(int i = 0; i<sibling->size; i++)
                set[j++] = sibling->I[i];
            for(int i = 0; i<n->size; i++)
                set[j++] = n->I[i];
        }
    }
    else{
        for(int i = 0; i<n->size; i++)           // if there is no sibling, just append the indices of n
                set[j++] = n->I[i];
    }
    // insert the new rectangle in sorted order
    for(int i = 0; i<setSize; i++){
        if (set[i]> ind){
            j = i;            // got the index
            break;
        }
    }
    for (int i = setSize-2; i>=j; i--) //last index of set array is setSize-1
        set[i+1] = set[i];    // shift the elements(rectangles)
    set[j] = ind;             // insert the new rectangle
    
    // now we have our sorted set of rectangles.
    // Now, if there is space in the sibling, distribute all rectangles evenly in both
    if(sibling && sibling->size < M){ // now total elements can be adjusted in the two siblings
        int k = setSize/2;
        
        if(n->I[0]->lhv < sibling->I[0]->lhv){ // sibling is greater in terms of Hilbert Value
            int i1 = 0, i2 = 0, i3 = 0;
            while(i1 < k) 
                n->I[i1++] = set[i2++];  // set the node first
            while(i1 < n->size)
                n->I[i1++] = NULL;
            while(i2 < setSize)    // then set the sibling
                sibling->I[i3++] = set[i2++];
            while(i3 < sibling->size)
                sibling->I[i3++] = NULL;
            n->size = k; 
            sibling->size = setSize - k;
        }
        else{
            int i1 = 0, i2 = 0, i3 = 0;
            while(i3 < k)
                sibling->I[i3++] = set[i2++];  // set the sibling first
            while(i3 < sibling->size)
                sibling->I[i3++] = NULL;
            while(i2 < setSize)
                n->I[i1++] = set[i2++];     // then set the node 
            while(i1<n->size)
                n->I[i1++] = NULL;
            sibling->size = k;
            n->size = setSize - k;
        }

        return NULL;
    }
    else if(sibling){ // if sibling is present and it is full then we will distribute all indeices in 3 nodes
        NODE NN = newNode(); // new node
        int k = setSize/3;
        if(n->I[0]->lhv < sibling->I[0]->lhv){ // so sibling was on right 
            for(int i = 0; i < n->size; i++) // removing all indices from n
                n->I[i] = NULL;
            for(int i = 0; i < sibling->size; i++) // removing all indices from siblings
                sibling->I[i] = NULL;
            int j = 0;
            for(int i = 0; i < k ; i++) // putting first k indices in n (lowest) 
                n->I[i] = set[j++];
            for(int i = 0; i < k ; i++) // putting next k indices in siblings
                sibling->I[i] = set[j++];
            for(int i = 0; i < setSize - 2*k; i++) // putting next k in new n
                NN->I[i] = set[j++];
        }
        else{
            for(int i = 0; i < n->size; i++) // removing all indices from n
                n->I[i] = NULL;
            for(int i = 0; i < sibling->size; i++) // removing all indices from siblings
                sibling->I[i] = NULL;
            int j = 0;
            for(int i = 0; i < k ; i++) // putting next k indices in siblings (lowest)
                sibling->I[i] = set[j++];
            for(int i = 0; i < k ; i++)// putting first k indices in n
                n->I[i] = set[j++];
            for(int i = 0; i < setSize - 2*k ; i++) // putting next k in new n
                NN->I[i] = set[j++];
        }
        // updating all the sizes
        n->size = k;
        sibling->size=k;
        NN->size=setSize - 2*k;
        
        return NN;    
    }
    else{ // when there is no sibling. This case should only arise when n is the root node
        NODE NN = newNode(); // create a new node to distribute indices evenly
        int k = setSize/2;   // k is the number of indices we will put in n, the original node
        
        for(int i = 0; i < n->size; i++) 
            n->I[i] = NULL;              // make all indices in n NULL
        n->size = 0;
        
        int j = 0;
        for(int i = 0; i < k; i++){
            n->I[i] = set[j++];     // add the first k indices of the set to n
            n->size++;
        }
        for(int i = 0; i < setSize - k; i++){
            NN->I[i] = set[j++];           // add the rest of them to new node NN
            NN->size++;
        }
        
        return NN;
    }
}

INDEX MBR(NODE n){
    POINT min, max;
    float lhv = n->I[0]->lhv;
    
    min->x = n->I[0]->rect->min->x;
    min->y = n->I[0]->rect->min->y;
    max->x = n->I[0]->rect->max->x;
    max->y = n->I[0]->rect->max->y;

    // calculate the min and max points and lhv of the MBR of n
    for(int i = 1; i < n->size; i++){
        if(n->I[i]->rect->min->x < min->x)
            min->x = n->I[i]->rect->min->x; // x value for min will be the minimum x for all rectangles in n
        if(n->I[i]->rect->min->y < min->y)
            min->y = n->I[i]->rect->min->y; // y value for min will be the minimum y for all rectangles in n
        if(n->I[i]->rect->max->x > max->x)
            max->x = n->I[i]->rect->max->x; // x value for max will be the maximum x for all rectangles in n
        if(n->I[i]->rect->max->y > max->y)
            max->y = n->I[i]->rect->max->y; // y value for max will be the maximum y for all rectangles in n
        if(n->I[i]->lhv > lhv)
            lhv = n->I[i]->lhv;             // lhv will be the maximum lhv for all rectangles in n
    }

    RECTANGLE mbr = newRectangle(min, max); // creating the MBR
    INDEX ind = newIndex(mbr, lhv, n); // creating the index that stores the MBR

    return ind;
}

NODE AdjustTree(TREE t, NODE arr[]){
    NODE root = t->root;
    /*
        arr[0] -> node being updated
        arr[1] -> sibling node, if any (can be NULL)
        arr[2] -> new node created during split, if any (can be NULL)
    */
    if(!arr[0]->parent) return arr[2]; // if reached root level, stop and return the new node if root was split
    NODE Np = arr[0]->parent;

    NODE parents[3]; 
    // parents is  an array that contains the parent node 
    // of the node being updated, its cooperating sibling,
    // if any, and the new node that might be formed when 
    // we try to insert into the parent node
    parents[0] = Np;
    parents[1] = GetCooperatingSibling(Np);
    parents[2] = NULL;
    
    // if there was a split when trying to insert into 
    // the node, we try to insert the MBR of the newly
    // created node into the parent node Np
    if(arr[2]){
        INDEX NN = MBR(arr[2]);
        
        if(Np->size < M){ // if there is space in Np, we insert
            for(int i = Np->size++ ; i >= 0; i--){
                if(Np->I[i]->lhv > NN->lhv)
                    Np->I[i+1] = Np->I[i];
                else{
                    Np->I[i] = NN;
                    break;
                } 
            }
        }
        else{ // otherwise, call HandleOverflow
            parents[2] = HandleOverflow(t, Np, NN);
        }
    }

    for(int i = Np->size - 1 ; i >= 0; i--){  // fix the MBRs present in the parent node
        Np->I[i] = MBR(Np->I[i]->child);
    }

    return AdjustTree(t, parents);
}

void JoinRoot(TREE t, NODE left){
    NODE root = t->root; 
    NODE new = newNode();
    if(root->I[root->size - 1]->lhv < left->I[left->size - 1]->lhv){
        new->I[new->size++] = root;
        new->I[new->size++] = left;
    }
    else{
        new->I[new->size++] = left;
        new->I[new->size++] = root;
    }
    t->root = new;
}

void Insert(TREE t, RECTANGLE r){
    NODE root = t->root;
    
    // calculating the center point of the rectangle
    float xmid=(r->min->x+r->max->x)/2.0;
    float ymid=(r->min->x+r->max->y)/2.0;
    POINT pmid=newPoint(xmid, ymid); // creating a point with those coordinates

    INDEX ind = newIndex(r, hilbertValue(pmid), NULL); // creating an index record having the given rectangle
    
    // this selects the leaf node in which to place our rectangle
    NODE leaf = ChooseLeaf(root, r, hilbertValue(pmid)); 
    
    NODE arr[3];
    arr[0] = leaf;
    arr[1] = GetCooperatingSibling(leaf);
    arr[2] = NULL;

    if(leaf->size<M){ // if there is space in this node, insert the rectangle
        for(int i=leaf->size-1; i>=0; i--){
            if(leaf->I[i]->lhv > ind->lhv) leaf->I[i+1] = leaf->I[i];    // inserting the rectangle in the
            else{                                                        // node in a sorted order based on
                leaf->I[i+1]=ind;                                        // the hilbert values of all the 
                break;                                                   // rectangles, akin to the
            }                                                            // insertion sort algorithm 
        }
    }
    else              // else, call the HandleOverflow method
        arr[2] = HandleOverflow(t, leaf, ind);
    NODE Left = AdjustTree(t, arr);
    if(Left)
        JoinRoot(t, Left);
}

int checkIntersection(RECTANGLE r1, RECTANGLE r2){
    return (r2->min->x <= r1->max->x) && (r2->max->x >= r1->min->x) && (r2->min->y <= r1->max->y) && (r2->max->y >= r1->min->y);
}

void Search(TREE t,RECTANGLE r){
    NODE root = t->root;
    
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

int main(){
    POINT p = newPoint(2,10);
    float test = hilbertValue(p);
    printf("%f\n", test);
    return 0;
}