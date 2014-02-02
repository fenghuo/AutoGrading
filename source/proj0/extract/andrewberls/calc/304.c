#include <stdio.h>
#include <math.h>
int main(void) {
int m[101] = {};
m[0]=5/2*3;
m[2]=5/2*3;
printf("%d\n", m[2]);
if (m[0])
  goto L1;
  goto L2;
L1:
m[75]=5-(m[2]-pow(6,2));
m[13]=m[2];
