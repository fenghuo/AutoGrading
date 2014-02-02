#include <stdio.h>
#include <math.h>

int main()
{
int m[101];
m[0]=5 / 2 * 3;
m[4]=88 + 2;
m[0]=5 - (m[4] - 6);
m[0]=5 / 2 * 3;
m[0]=2 + m[2];
m[0]=5 - m[pow(4,3) * 1 - 2] - 6;
L1:
m[0]=5 - (m[4] - 6);
m[0]=5 / 2 * 3;
m[0]=2 + m[2];
m[0]=5 - m[4 - 6];
m[3]=5 - (4 - 6);
m[2]=5 - m[(6 - 4)];
printf( " %i\n", m[3]);
if (m[60])
	goto L1;
printf( " %i\n", m[2]);
goto L2;
printf( " %i\n", m[0]);
L2:
printf( " %i\n", m[0]);
m[0]=5 / 2 * 3;
m[4]=2 + 2;
m[0]=5 - (m[4] - 6);
m[0]=5 / 2 * 3;
m[0]=2 + m[2];
m[0]=5 - m[4 - 6];
return 0;
}