#include <stdio.h>
int e(int b, int ex){return ex==0?1:b*e(b,--ex);}
int main(){
	int m[101];
	m[0]=5/2*3;
	m[43]=5/2*3;
	goto L4;
	m[15]=2+m[2];
	m[56]=5-m[e(4,1)*2-2]-6;
	L111