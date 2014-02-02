#include <stdio.h>
#include <math.h>
int main {
int m[101];
L1:
m[4] = 5-(m[4]-6);
m[5] = 5/2*3;
m[6] = 2+m[2];
m[7] = 5-m[4-6];
m[8] = 5-(4-6);
m[9] = 5-m[(6-4)];
printf("%i",m[3]);
if(m[0]) goto L1;
printf("%i",m[2]);
goto L5;
printf("%i",m[0]);
L2:
printf("%i",m[0]);
return 0;
}
