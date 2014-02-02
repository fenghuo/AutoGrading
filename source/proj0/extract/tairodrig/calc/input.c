#include <math.h>
#include <stdio.h>
int main (){
int m[101] = {0};
m[0]=pow(1,pow(2,pow(3,pow(2,1))));
m[1]=2;
m[10]=3*5;
m[100]=5+(6);
m[50]=5+(4-6);
m[51]=4-6;
m[60]=4;
m[0]=5-(4-6);
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
m[2]=m[m[3]];
printf("%i\n", m[3]);
if(m[60]) goto L1;
printf("%i\n", m[2]);
goto L2;
printf("%i\n", m[0]);
L2:
printf("%i\n", m[0]);
return 0;
}
