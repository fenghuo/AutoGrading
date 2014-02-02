#include <stdio.h>
#include <math.h>
int main() {
int m[101] = {};
m[0]=5/2*3;
m[2]=5/2*3;
printf("%d",m[2]);
if (m[0]) {
goto L1;
}
goto L2;
L1:
m[75]=5-pow((m[2]-6,3));
L2:
m[75]=5-(m[0]-6);
printf("%d",m[0]);
printf("%d",m[2]);
printf("%d",m[75]);
return 0;
}
