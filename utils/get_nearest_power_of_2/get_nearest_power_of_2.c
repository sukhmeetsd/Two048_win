#include<math.h>
int getNearestPowerOf2(int c)
{
    if(c==1) return 2;
    int pwr=10;
    while(c>pow(2,pwr)) pwr+=5;
    int i=pwr>10?(pwr-5):1;
    while(pow(2,i++)<=c);
    i-=2;
    if(pow(2,i)==c) return c;
    else return((c-pow(2,i))<(pow(2,(i+1))-c)? pow(2,i):pow(2,(i+1)));
}