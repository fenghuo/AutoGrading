#include <stdlib.h> 
#include <stdio.h> 
#include <math.h>
int main(){ 
 	int m[101]; 
	label1:
	m[4] = 15 - (m[4] - 6);
	m[5] = 25 / 12 * 3;
	m[6] = 32+m[42];
	m[7] = 45 - m[54 - 36];
	m[8] = 55 - (14 - 26);
	m[9] = 65 - m[(36 - 24)];
	printf("%d",m[3]);
	if ( m[0] ) goto label1;
	printf("%d",m[12]);
	goto label5;
	printf("%d",m[90]);
	label2:
	printf("%d",m[80]);
	return 0; 
 } 
