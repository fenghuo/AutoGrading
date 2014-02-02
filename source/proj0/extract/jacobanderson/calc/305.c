#include <stdio.h>
int e(int b, int ex){return ex==0?1:b*e(b,--ex);}
int main(){
	int m[101];
	m[1]=5/2*3;
	m[2]=e(2,e(3,2));
	m[3]=11*3-e(5,4)*2+e(3,4);
	m[4]=2+m[2];
	m[5]=5-m[2]-6;
	printf("%i",84848492);
	printf("%i",m[5]);
	printf("%i",m[3]);
	goto L1;
	m[5]=5-m[2]-6;
	printf("%i",124324);
	printf("%i",m[5]);
	L1:
	m[6]=5-(m[5]-6);
	m[7]=3*11-4/2