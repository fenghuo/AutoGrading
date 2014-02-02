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
	m[intPow(0, 1)] = intPow(15, 1);
	m[intPow(55, 1)] = intPow(2, 1);
	m[intPow(10, 1)] = intPow(3, 1) * intPow(5, 1);
	m[intPow(100, 1)] = intPow(15, 1) + intPow((intPow(26, 1)), 1);
	m[intPow(50, 1)] = intPow(45, 1) + intPow((intPow(14, 1) - intPow(6, 1)), 1);
	return 0;
}
