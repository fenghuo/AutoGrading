#include <math.h>
#include <string.h>
#include <stdio.h>


int main()
{
	int m[101];

	m[4] = 88 + 2;
	m[23] = 5 - (m[4] - 6);
	m[15] = 2 + m[2];
	m[56] = 5 - m[pow(4, 3 * 1 - 2)] - 6;
	label2:
	m[75] = 5 - (m[4] - 6);
	m[68] = 5 / 2 * 3;
	m[43] = 5 / 2 * 3;
	goto label4;
	m[15] = 2 + m[2];
	m[56] = 5 - m[pow(3, 4 * 1 - 2)] - 6;
	label1:
	m[75] = 5 - (m[4] - 6);
	m[68] = 5 / 2 * 3;
	goto label6;
	m[43] = 2 + m[2];
	m[19] = 5 - m[4 - 6];
	m[75] = 5 - (4 - 6);
	m[46] = 5 - m[(6 - 4)];
	printf("%d\n", m[3]);
}
