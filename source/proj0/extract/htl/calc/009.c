#include <stdio.h>
inline int intPow(int num, int e) {
	int res = 1;
	if(e>=0){
		for(;e>0;e--)res = res*num;
		return res;
	}
	if(num == 1) return 1;
	return 0;
}
int main(int argc, char *argv[]){
	int m[101];
	m[intPow(0, 1)] = intPow(5, 1) / intPow(2, 1) * intPow(3, 1);
	m[intPow(4, 1)] = intPow(88, 1) + intPow(2, 1);
	m[intPow(0, 1)] = intPow(5, 1) - intPow((intPow(m[intPow(4, 1)], 1) - intPow(6, 1)), 1);
	m[intPow(0, 1)] = intPow(5, 1) / intPow(2, 1) * intPow(3, 1);
	m[intPow(0, 1)] = intPow(2, 1) + intPow(m[intPow(2, 1)], 1);
	m[intPow(0, 1)] = intPow(5, 1) - intPow(m[intPow(4, intPow(3, 1)) * intPow(1, 1) - intPow(2, 1)], 1) - intPow(6, 1);
	my_label1:
	m[intPow(0, 1)] = intPow(5, 1) - intPow((intPow(m[intPow(4, 1)], 1) - intPow(6, 1)), 1);
	m[intPow(0, 1)] = intPow(5, 1) / intPow(2, 1) * intPow(3, 1);
	m[intPow(0, 1)] = intPow(2, 1) + intPow(m[intPow(2, 1)], 1);
	m[intPow(0, 1)] = intPow(5, 1) - intPow(m[intPow(4, 1) - intPow(6, 1)], 1);
	m[intPow(3, 1)] = intPow(5, 1) - intPow((intPow(4, 1) - intPow(6, 1)), 1);
	m[intPow(2, 1)] = intPow(5, 1) - intPow(m[intPow((intPow(6, 1) - intPow(4, 1)), 1)], 1);
	printf("%d\n", intPow(m[intPow(3, 1)], 1));
	if(intPow(m[intPow(60, 1)], 1))
		goto my_label1;
	printf("%d\n", intPow(m[intPow(2, 1)], 1));
	goto my_label2;
	printf("%d\n", intPow(m[intPow(0, 1)], 1));
	my_label2:
	printf("%d\n", intPow(m[intPow(0, 1)], 1));
	m[intPow(0, 1)] = intPow(5, 1) / intPow(2, 1) * intPow(3, 1);
	m[intPow(4, 1)] = intPow(2, 1) + intPow(2, 1);
	m[intPow(0, 1)] = intPow(5, 1) - intPow((intPow(m[intPow(4, 1)], 1) - intPow(6, 1)), 1);
	m[intPow(0, 1)] = intPow(5, 1) / intPow(2, 1) * intPow(3, 1);
	m[intPow(0, 1)] = intPow(2, 1) + intPow(m[intPow(2, 1)], 1);
	m[intPow(0, 1)] = intPow(5, 1) - intPow(m[intPow(4, 1) - intPow(6, 1)], 1);
	return 0;
}
