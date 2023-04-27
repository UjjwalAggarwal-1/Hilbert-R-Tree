#include "hilbert.h"
#include <math.h>


// rotate/flip a quadrant appropriately
void rotate(int n,int *x,int *y, int x0, int y0)
{
    if(y0==0)
    {
        if(x0==1)
        {
            *y=n-1-*y;
            *x=n-1-*x;
        }
        //Swap x and y
        int t=*x;
        *x=*y;
        *y=t;
    }
}

// convert (x,y) to d
int hilbertValue(POINT p)
{
    int n=pow(2,N);
    int x=p->x,y=p->y;
    int x0,y0,s,d=0;
    for(s=n/2;s>0;s/=2)
    {
        y0=(y&s)>0;
        x0=(x&s)>0;
        d=d+((3*x0)^y0)*s*s;
        rotate(n,&x,&y,x0,y0);
    }
    return d;
}
