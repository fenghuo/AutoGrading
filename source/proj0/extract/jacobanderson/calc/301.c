#include <stdio.h>
int e(int b, int ex){return ex==0?1:b*e(b,--ex);}
int main(){
	int m[101];
	m[0]=5/2*3;
	m[43]=100*5-2-5-9/3;
	printf("%i",m[0]);
	printf("%i",m[43]);
	printf("%i",m[0]);
	printf("%i",m[43]);
	return 0;
}
