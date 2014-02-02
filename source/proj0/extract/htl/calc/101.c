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
	return 0;
}
