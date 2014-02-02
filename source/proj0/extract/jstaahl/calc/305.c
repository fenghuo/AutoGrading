#include <stdio.h>
#include <math.h>

int main()
{
	int m[101];

    m[(int)pow(1, 1)] = (int)pow(5, 1)/(int)pow(2, 1)*(int)pow(3, 1);
    m[(int)pow(2, 1)] = (int)pow(2, pow(3, pow(2, 1)));
    m[(int)pow(3, 1)] = (int)pow(11, 1)*(int)pow(3, 1) - (int)pow(5, pow(4, 1))*(int)pow(2, 1) + (int)pow(3, pow(4, 1));
    m[(int)pow(4, 1)] = (int)pow(2, 1) + (int)pow(m[(int)pow(2, 1)], 1);
    m[(int)pow(5, 1)] = (int)pow(5, 1) - (int)pow(m[(int)pow(2, 1)], 1) - (int)pow(6, 1);
    printf("%d", (int)pow(84848492, 1) );
    printf("%d", (int)pow(m[(int)pow(5, 1)], 1) );
    printf("%d", (int)pow(m[(int)pow(3, 1)], 1) );
    goto line1;
    m[(int)pow(5, 1)] = (int)pow(5, 1) - (int)pow(m[(int)pow(2, 1)], 1) - (int)pow(6, 1);
    printf("%d", (int)pow(124324, 1) );
    printf("%d", (int)pow(m[(int)pow(5, 1)], 1) );
line1: ;
    m[(int)pow(6, 1)] = (int)pow(5, 1) - (int)pow(((int)pow(m[(int)pow(5, 1)], 1) - (int)pow(6, 1)), 1);
    m[(int)pow(7, 1)] = (int)pow(3, 1)*(int)pow(11, 1) - (int)pow(4, 1)/(int)pow(2, pow(2, 1));
    m[(int)pow(8, 1)] = (int)pow(2, 1) + (int)pow(m[(int)pow(7, 1)], 1);
    m[(int)pow(9, 1)] = (int)pow(3, pow(2, 1)) - (int)pow(2, pow(2, pow(2, 1))) - (int)pow(5, 1);
    m[(int)pow(11, 1)] = (int)pow(5, 1) - (int)pow(m[(int)pow(11, 1) - (int)pow(6, 1)], 1);
    m[(int)pow(12, 1)] = (int)pow(5, 1) - (int)pow(((int)pow(4, 1) - (int)pow(6, 1)), 1);
    m[(int)pow(13, 1)] = (int)pow(5, 1) - (int)pow(((int)pow(6, pow(8, 1)) - (int)pow(m[(int)pow(11, 1)], 1)), 1);
    m[(int)pow(14, 1)] = (int)pow(5, 1)/(int)pow(2, 1)*(int)pow(3, 1);
    printf("%d", (int)pow(m[(int)pow(13, 1)], 1) );
    printf("%d", (int)pow(m[(int)pow(14, 1)], 1) );
    m[(int)pow(15, pow(1, 1))] = (int)pow(2, 1) + (int)pow(m[(int)pow(13, 1)], 1);
    m[(int)pow(16, 1)] = (int)pow(5, 1) - (int)pow(m[(int)pow(14, 1) - (int)pow(2, 1)], 1) - (int)pow(6, 1);
line2: ;
    m[(int)pow(17, 1)] = (int)pow(5, 1) - (int)pow(((int)pow(m[(int)pow(16, 1)], 1) - (int)pow(6, 1)), 1);
    if ( (int)pow(m[(int)pow(16, 1)], 1) ) goto line4;
    goto line3;
    m[(int)pow(15, 1)] = (int)pow(2, 1) + (int)pow(m[(int)pow(7, 1)], 1);
    m[(int)pow(16, 1)] = (int)pow(5, 1) - (int)pow(m[(int)pow(4, 1)*(int)pow(2, 1) - (int)pow(2, 1) + (int)pow(7, 1)], 1) - (int)pow(6, 1);
line3: ;
    m[(int)pow(15, 1)] = (int)pow(5, 1) - (int)pow(((int)pow(m[(int)pow(15, 1)], 1) + (int)pow(m[(int)pow(7, 1)], 1) - (int)pow(6, 1)), 1);
    m[(int)pow(16, 1)] = (int)pow(5, 1)/(int)pow(2, 1)*(int)pow(3, 1);
    goto line5;
line4: ;
    m[(int)pow(25, 1)] = (int)pow(5, 1) - (int)pow(((int)pow(m[(int)pow(4, 1)], 1) - (int)pow(6, 1)), 1);
    printf("%d", (int)pow(m[(int)pow(25, 1)], 1) );
line5: ;
    printf("%d", (int)pow(m[(int)pow(25, 1)], 1) );
    printf("%d", (int)pow(m[(int)pow(15, 1)], 1) );
    printf("%d", (int)pow(m[(int)pow(16, 1)], 1) );

    return 0;
}
