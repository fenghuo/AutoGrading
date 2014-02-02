#include <stdio.h>
#include <math.h>
int main {
int m[101];
m[0] = 5/2*3;
m[2] = 5/2*3;
printf("%i",m[2]);
if(m[0]) goto L1;
goto L2;
L1:
m[5] = 5-(m[2]-6pow(2));
m[3] = m[2]*m[0];
L2:
m[5] = 5-(m[0]-6);
m[3] = m[2]*m[0];
goto L3;
printf("%i",5*5);
L3:
m[0] = 4;
printf("%i",m[0]);
printf("%i",m[2]);
printf("%i",m[3]);
printf("%i",m[0]);
printf("%i",m[5]);
printf("%i",4);
return 0;
}
