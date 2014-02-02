#include <stdio.h>
int e(int b, int ex){return ex==0?1:b*e(b,--ex);}
int main(){
	int m[101];
	m[0]=5/2*3;
	m[2]=5/2*3;
	printf("%i",m[2]);
	if(m[0]) goto L1;
	goto L2;
	L1:
	m[75]=5-(m[2]-e(6,2));
	m[13]=m[2]*