#include <math.h>
#include <stdio.h>
int main (){
int m[101] = {0};
m[0]=5/2*3;
m[43]=2+m[2];
m[19]=5-m[4-6];
m[75]=5-(4-6);
m[46]=5-m[(6-4)];
printf("%i\n", m[3]);
m[43]=5/2*3;
goto L4;
m[15]=2+m[2];
m[56]=5-m[pow(2,3)*2-2]-6;
L1:
m[75]=5-(m[4]-6);
if(m[60]) goto L1;
goto L4;
m[15]=2+m[2];
m[56]=5-m[pow(3,2)*2-2]-6;
L1:
m[75]=5-(m[4]-6);
m[68]=5/2*3;
return 0;
}
