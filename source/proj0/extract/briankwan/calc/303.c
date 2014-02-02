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
m[5] = 5-(m[2]-6pow(3));
L2:
m[5] = 5-(m[0]-6);
printf("%i",m[0]);
printf("%i",m[2]);
printf("%i",m[5]);
return 0;
}
