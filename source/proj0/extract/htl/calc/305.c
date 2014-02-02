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
	m[intPow(1, 1)] = intPow(5, 1) / intPow(2, 1) * intPow(3, 1);
	m[intPow(2, 1)] = intPow(2, intPow(3, intPow(2, 1)));
	m[intPow(3, 1)] = intPow(11, 1) * intPow(3, 1) - intPow(5, intPow(4, 1)) * intPow(2, 1) + intPow(3, intPow(4, 1));
	m[intPow(4, 1)] = intPow(2, 1) + intPow(m[intPow(2, 1)], 1);
	m[intPow(5, 1)] = intPow(5, 1) - intPow(m[intPow(2, 1)], 1) - intPow(6, 1);
	printf("%d\n", intPow(84848492, 1));
	printf("%d\n", intPow(m[intPow(5, 1)], 1));
	printf("%d\n", intPow(m[intPow(3, 1)], 1));
	goto my_label1;
	m[intPow(5, 1)] = intPow(5, 1) - intPow(m[intPow(2, 1)], 1) - intPow(6, 1);
	printf("%d\n", intPow(124324, 1));
	printf("%d\n", intPow(m[intPow(5, 1)], 1));
	my_label1:
	m[intPow(6, 1)] = intPow(5, 1) - intPow((intPow(m[intPow(5, 1)], 1) - intPow(6, 1)), 1);
	m[intPow(7, 1)] = intPow(3, 1) * intPow(11, 1) - intPow(4, 1) / intPow(2, intPow(2, 1));
	m[intPow(8, 1)] = intPow(2, 1) + intPow(m[intPow(7, 1)], 1);
	m[intPow(9, 1)] = intPow(3, intPow(2, 1)) - intPow(2, intPow(2, intPow(2, 1))) - intPow(5, 1);
	m[intPow(11, 1)] = intPow(5, 1) - intPow(m[intPow(11, 1) - intPow(6, 1)], 1);
	m[intPow(12, 1)] = intPow(5, 1) - intPow((intPow(4, 1) - intPow(6, 1)), 1);
	m[intPow(13, 1)] = intPow(5, 1) - intPow((intPow(6, intPow(8, 1)) - intPow(m[intPow(11, 1)], 1)), 1);
	m[intPow(14, 1)] = intPow(5, 1) / intPow(2, 1) * intPow(3, 1);
	printf("%d\n", intPow(m[intPow(13, 1)], 1));
	printf("%d\n", intPow(m[intPow(14, 1)], 1));
	m[intPow(15, intPow(1, 1))] = intPow(2, 1) + intPow(m[intPow(13, 1)], 1);
	m[intPow(16, 1)] = intPow(5, 1) - intPow(m[intPow(14, 1) - intPow(2, 1)], 1) - intPow(6, 1);
	my_label2:
	m[intPow(17, 1)] = intPow(5, 1) - intPow((intPow(m[intPow(16, 1)], 1) - intPow(6, 1)), 1);
	if(intPow(m[intPow(16, 1)], 1))
		goto my_label4;
	goto my_label3;
	m[intPow(15, 1)] = intPow(2, 1) + intPow(m[intPow(7, 1)], 1);
	m[intPow(16, 1)] = intPow(5, 1) - intPow(m[intPow(4, 1) * intPow(2, 1) - intPow(2, 1) + intPow(7, 1)], 1) - intPow(6, 1);
	my_label3:
	m[intPow(15, 1)] = intPow(5, 1) - intPow((intPow(m[intPow(15, 1)], 1) + intPow(m[intPow(7, 1)], 1) - intPow(6, 1)), 1);
	m[intPow(16, 1)] = intPow(5, 1) / intPow(2, 1) * intPow(3, 1);
	goto my_label5;
	my_label4:
	m[intPow(25, 1)] = intPow(5, 1) - intPow((intPow(m[intPow(4, 1)], 1) - intPow(6, 1)), 1);
	printf("%d\n", intPow(m[intPow(25, 1)], 1));
	my_label5:
	printf("%d\n", intPow(m[intPow(25, 1)], 1));
	printf("%d\n", intPow(m[intPow(15, 1)], 1));
	printf("%d\n", intPow(m[intPow(16, 1)], 1));
	return 0;
}
