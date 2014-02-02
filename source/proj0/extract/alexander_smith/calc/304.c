#include <stdlib.h> 
#include <stdio.h> 
#include <math.h>
int main(){ 
 	int m[101]; 
	m[0] = 5 / 2 * 3;
	m[2] = 5 / 2 * 3;
	printf("%d",m[2]);
	if ( m[0] ) goto label1;
	goto label2;
	label1:
	m[75] = 5 - (m[2] - pow(6, 2));
