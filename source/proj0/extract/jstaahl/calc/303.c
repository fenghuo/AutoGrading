#include <stdio.h>
#include <math.h>

int main()
{
	int m[101];

    m[(int)pow(0, 1)] = (int)pow(5, 1)/(int)pow(2, 1)*(int)pow(3, 1);
    m[(int)pow(2, 1)] = (int)pow(5, 1)/(int)pow(2, 1)*(int)pow(3, 1);
    printf("%d", (int)pow(m[(int)pow(2, 1)], 1) );
    if ( (int)pow(m[(int)pow(0, 1)], 1) ) goto line1;
    goto line2;
line1: ;
    m[(int)pow(75, 1)] = (int)pow(5, 1) - (int)pow(((int)pow(m[(int)pow(2, 1)], 1) - (int)pow(6, pow(3, 1))), 1);
line2: ;
    m[(int)pow(75, 1)] = (int)pow(5, 1) - (int)pow(((int)pow(m[(int)pow(0, 1)], 1) - (int)pow(6, 1)), 1);
    printf("%d", (int)pow(m[(int)pow(0, 1)], 1) );
    printf("%d", (int)pow(m[(int)pow(2, 1)], 1) );
    printf("%d", (int)pow(m[(int)pow(75, 1)], 1) );

    return 0;
}
