#include <stdio.h>
int e(int b, int ex){return ex==0?1:b*e(b,--ex);}
int main(){
	int m[101];
	m[60]=m[60]-1;
	m[0]=5/2*3;
	m[4]=