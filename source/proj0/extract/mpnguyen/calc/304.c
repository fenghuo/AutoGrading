#include <math.h>
#include <string.h>
#include <stdio.h>


int main()
{
	int m[101];

	m[0] = 5 / 2 * 3;
	m[2] = 5 / 2 * 3;
	printf("%d\n", m[2]);
	if (m[0]) goto label1;
	goto label2;
	label1:
	m[75] = 5 - (m[2] - pow(6, 2));
	m[13] = m[2] * m[0];
	label2:
	m[75] = 5 - (m[0] - 6);
	m[13] = m[2] * m[0];
	goto label3;
	printf("%d\n", 5 * 5);
	label3:
	m[10] = 1334;
	printf("%d\n", m[0]);
	printf("%d\n", m[2]);
	printf("%d\n", m[13]);
	printf("%d\n", m[10]);
	printf("%d\n", m[75]);
	printf("%d\n", 666444);
}
