#include <stdio.h>
int e(int b, int ex){return ex==0?1:b*e(b,--ex);}
int main(){
	int m[101];
	m[43]=5/2*3;
	m[15]=2+m[m[43]-m[43]+43];
	m[56]=5-m[e(2,2)-4+43]-6;
	printf("%i",m[15]);
	printf("%i",m[56]);
	printf("%i",m[43]);
	return 0;
}
