#include <stdio.h>
int e(int b, int ex){return ex==0?1:b*e(b,--ex);}
int main(){
	int m[101];
	m[0]=15;
	m[55]=2;
	m[10]=3*5;
	m[100]=15+(26);
	m[50]=45+(14-6);
	return 0;
}
