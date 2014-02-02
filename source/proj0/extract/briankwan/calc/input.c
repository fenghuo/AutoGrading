#include <stdio.h>
#include <math.h>
int main {
int m[101];
m[0] = 5;
m[1] = 2;
m[0] = 3*5;
m[0] = 5+(6);
m[0] = 5+(4-6);
m[1] = 4-6;
m[0] = 5-(4-6);
m[0] = 4;
L1:
m[0] = m[0]-1;
m[0] = 5/2*3;
m[4] = 2+2;
m[0] = 5-(m[4]-6);
m[0] = 5/2*3;
m[0] = 2+m[2];
m[0] = 5-m[4-6];
m[3] = 5-(4-6);
m[2] = 5-m[(6-4)];
m[2] = m[m[3]];
printf("%i",m[3]);
if(m[0]) goto L1;
printf("%i",m[2]);
goto L2;
printf("%i",m[0]);
L2:
printf("%i",m[0]);
m[3] = 5pow(3);
return 0;
}
