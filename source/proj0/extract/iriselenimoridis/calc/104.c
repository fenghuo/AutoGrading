#include <stdio.h>
#include <math.h>

int main()
{
int m[101]={0};
m[3]=5-(4-6);
m[2]=5-m[(6-4)];
printf( "%d\n", m[3]);
if (m[60])
	goto L1;
printf( "%d\n", m[2]);
goto L2;
printf( "%d\n", m[0]);
L2:
printf( "%d\n", m[0]);
m[10]=5/2*3;
m[44]=2+2;
m[50]=5-(m[4]-6);
m[70]=5/2*3;
return 0;

}
