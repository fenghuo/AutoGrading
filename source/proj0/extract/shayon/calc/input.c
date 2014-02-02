#include <stdio.h> 
#include <math.h> 
int main() 
{
    int m[101];
    m[0]=5;
    m[1]=2;
    m[10]=3*5;
    printf("%d",m[0]);
    printf("%d",m[1]);
    printf("%d",m[10]);
    m[2]=pow(2 , pow(3 , (4-2) )  ) ;
    return 0;
}