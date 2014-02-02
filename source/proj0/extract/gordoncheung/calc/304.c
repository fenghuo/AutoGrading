#include <stdio.h>
#include <math.h>
int main(){
int m[101];
m[0]=5/2*3;
m[2]=5/2*3;
printf("%i",m[2]);
if(m[0])goto L1;
goto L2;
L1:
m[75]=5-(m[2]-pow(6,2));
m[13]=m[2]*m[0];
L2:
m[75]=5-(m[0]-6);
m[13]=m[2]*m[0];
goto L3;
printf("%i",5*5);
L3:
m[10]=1334;
printf("%i",m[0]);
printf("%i",m[2]);
printf("%i",m[13]);
printf("%i",m[10]);
printf("%i",m[75]);
printf("%i",666444);
}
