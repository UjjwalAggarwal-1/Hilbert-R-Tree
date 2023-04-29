#include "hilbert.h"
#include <stdio.h>
#include <math.h>

char newQuadType(char oldType, float quad)
{
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

float quadValue(char oldType, float quad)
{
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

void hilbert(float n, float x, float y, float* res, char quadType)
{
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

float hilbertValue(POINT p)
{
    float res=0;
    hilbert(pow(2, N), p->x, p->y, &res, 'A');
    return res;
}

int main(){
    POINT p = newPoint(1,9);
    float test = hilbertValue(p);
    printf("%f\n", test);

    return 0;
}