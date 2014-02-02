#include <stdio.h>
#include <math.h>
int main()
{
int m[101];
m[(int)pow(0, 1)] = (int)pow(5, 1) / (int)pow(2, 1) * (int)pow(3, 1);
m[(int)pow(2, 1)] = (int)pow(5, 1) / (int)pow(2, 1) * (int)pow(3, 1);
printf("%d\n", (int)pow(m[(int)pow(2, 1)], 1));
if ((int)pow(m[(int)pow(0, 1)], 1))
	goto L1;
goto L2;
L1:
m[(int)pow(75, 1)] = (int)pow(5, 1) - (int)pow(((int)pow(m[(int)pow(2, 1)], 1) - (int)pow(6, 3)1)), 1);
L2:
m[(int)pow(75, 1)] = (int)pow(5, 1) - (int)pow(((int)pow(m[(int)pow(0, 1)], 1) - (int)pow(6, 1)), 1);
printf("%d\n", (int)pow(m[(int)pow(0, 1)], 1));
printf("%d\n", (int)pow(m[(int)pow(2, 1)], 1));
printf("%d\n", (int)pow(m[(int)pow(75, 1)], 1));
return 0;
}
