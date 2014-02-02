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
	my_label1:
	m[intPow(4, 1)] = intPow(15, 1) - intPow((intPow(m[intPow(4, 1)], 1) - intPow(6, 1)), 1);
	m[intPow(5, 1)] = intPow(25, 1) / intPow(12, 1) * intPow(3, 1);
	m[intPow(6, 1)] = intPow(32, 1) + intPow(m[intPow(42, 1)], 1);
	m[intPow(7, 1)] = intPow(45, 1) - intPow(m[intPow(54, 1) - intPow(36, 1)], 1);
	m[intPow(8, 1)] = intPow(55, 1) - intPow((intPow(14, 1) - intPow(26, 1)), 1);
	m[intPow(9, 1)] = intPow(65, 1) - intPow(m[intPow((intPow(36, 1) - intPow(24, 1)), 1)], 1);
	printf("%d\n", intPow(m[intPow(3, 1)], 1));
	if(intPow(m[intPow(0, 1)], 1))
		goto my_label1;
	printf("%d\n", intPow(m[intPow(12, 1)], 1));
	goto my_label5;
	printf("%d\n", intPow(m[intPow(90, 1)], 1));
	my_label2:
	printf("%d\n", intPow(m[intPow(80, 1)], 1));
	return 0;
}
