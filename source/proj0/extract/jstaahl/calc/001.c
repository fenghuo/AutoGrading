#include <stdio.h>
#include <math.h>

int main()
{
	int m[101];

    m[(int)pow(0, 1)] = (int)pow(15, 1);
    m[(int)pow(55, 1)] = (int)pow(2, 1);
    m[(int)pow(10, 1)] = (int)pow(3, 1)*(int)pow(5, 1);
    m[(int)pow(100, 1)] = (int)pow(15, 1) + (int)pow(((int)pow(26, 1)), 1);
    m[(int)pow(50, 1)] = (int)pow(45, 1) + (int)pow(((int)pow(14, 1) - (int)pow(6, 1)), 1);

    return 0;
}
