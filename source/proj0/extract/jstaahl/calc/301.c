#include <stdio.h>
#include <math.h>

int main()
{
	int m[101];

    m[(int)pow(0, 1)] = (int)pow(5, 1)/(int)pow(2, 1)*(int)pow(3, 1);
    m[(int)pow(43, 1)] = (int)pow(100, 1)*(int)pow(5, 1) - (int)pow(2, 1) - (int)pow(5, 1) - (int)pow(9, 1)/(int)pow(3, 1);
    printf("%d", (int)pow(m[(int)pow(0, 1)], 1) );
    printf("%d", (int)pow(m[(int)pow(43, 1)], 1) );
    printf("%d", (int)pow(m[(int)pow(0, 1)], 1) );
    printf("%d", (int)pow(m[(int)pow(43, 1)], 1) );

    return 0;
}
