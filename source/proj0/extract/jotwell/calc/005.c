#include <stdio.h>
#include <math.h>
int main()
{
int m[101];
L1:
m[(int)pow(4, 1)] = (int)pow(15, 1) - (int)pow(((int)pow(m[(int)pow(4, 1)], 1) - (int)pow(6, 1)), 1);
m[(int)pow(5, 1)] = (int)pow(25, 1) / (int)pow(12, 1) * (int)pow(3, 1);
m[(int)pow(6, 1)] = (int)pow(32, 1) + (int)pow(m[(int)pow(42, 1)], 1);
m[(int)pow(7, 1)] = (int)pow(45, 1) - (int)pow(m[(int)pow(54, 1) - (int)pow(36, 1)], 1);
m[(int)pow(8, 1)] = (int)pow(55, 1) - (int)pow(((int)pow(14, 1) - (int)pow(26, 1)), 1);
m[(int)pow(9, 1)] = (int)pow(65, 1) - (int)pow(m[(int)pow(((int)pow(36, 1) - (int)pow(24, 1)), 1)], 1);
printf("%d\n", (int)pow(m[(int)pow(3, 1)], 1));
if ((int)pow(m[(int)pow(0, 1)], 1))
	goto L1;
printf("%d\n", (int)pow(m[(int)pow(12, 1)], 1));
goto L5;
printf("%d\n", (int)pow(m[(int)pow(90, 1)], 1));
L2:
printf("%d\n", (int)pow(m[(int)pow(80, 1)], 1));
return 0;
}
