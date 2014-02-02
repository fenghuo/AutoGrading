#include <math.h>
#include <string.h>
#include <stdio.h>


int main()
{
	int m[101];

	m[1] = 5 / 2 * 3;
	m[2] = pow(2, pow(3, 2));
	m[3] = 11 * 3 - pow(5, 4 * 2 + pow(3, 4));
	m[4] = 2 + m[2];
	m[5] = 5 - m[2] - 6;
	printf("%d\n", 84848492);
	printf("%d\n", m[5]);
	printf("%d\n", m[3]);
