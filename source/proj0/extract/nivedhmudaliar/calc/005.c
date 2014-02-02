#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
	int m[101];
L13:
m[42] = 15 - (m[41] - 61);
m[52] = 25 / 12 * 31;
m[61] = 32 + m[42];
m[71] = 45 - m[54 - 36];
m[81] = 55 - (14 - 26);
m[91] = 65 - m[(36 - 24)];
printf("%d\n", m[30]);
if(m[0])
		goto 11;
printf("%d\n", m[12]);
goto 51;
printf("%d\n", m[90]);
L20:
return 0;}
