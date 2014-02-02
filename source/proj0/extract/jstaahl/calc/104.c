#include <stdio.h>
#include <math.h>

int main()
{
	int m[101];

    m[(int)pow(3, 1)] = (int)pow(5, 1) - (int)pow(((int)pow(4, 1) - (int)pow(6, 1)), 1);
    m[(int)pow(2, 1)] = (int)pow(5, 1) - (int)pow(m[(int)pow(((int)pow(6, 1) - (int)pow(4, 1)), 1)], 1);
    printf("%d", (int)pow(m[(int)pow(3, 1)], 1) );
    if ( (int)pow(m[(int)pow(60, 1)], 1) ) goto line1;
    printf("%d", (int)pow(m[(int)pow(2, 1)], 1) );
    goto line2;
    printf("%d", (int)pow(m[(int)pow(0, 1)], 1) );
line2: ;
    printf("%d", (int)pow(m[(int)pow(0, 1)], 1) );
    m[(int)pow(10, 1)] = (int)pow(5, 1)/(int)pow(2, 1)*(int)pow(3, 1);
    m[(int)pow(44, 1)] = (int)pow(2, 1) + (int)pow(2, 1);
    m[(int)pow(50, 1)] = (int)pow(5, 1) - (int)pow(((int)pow(m[(int)pow(4, 1)], 1) - (int)pow(6, 1)), 1);
    m[(int)pow(70, 1)] = (int)pow(5, 1)/(int)pow(2, 1)*(int)pow(3, 1);

    return 0;
}
