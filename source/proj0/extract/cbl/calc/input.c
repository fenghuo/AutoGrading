#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main(){
int m[101];
m[0]=5;
m[1]=2;
m[100]=5+(6);
m[50]=5+(4-6);
m[51]=4-6;
m[0]=5-(4-6);
m[60]=214748362;
L1:
m[60]=m[60]-1;
m[0]=5/2*3;
m[4]=2+2;
m[0]=5-(m[4]-6);
m[0]=5/2*3;
m[0]=2+m[2];
m[0]=5-m[4-6];
m[3]=5-(4-6);
m[2]=5-m[(6-4)];
m[2]=m[m[3]3];
printf("%d",m[2]);
if(m[60])
goto L1;
goto L2;
printf("%d",m[0]);
L2:
printf("%d",m[0]);
return 0;
 }
