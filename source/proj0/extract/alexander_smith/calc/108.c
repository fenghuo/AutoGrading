#include <stdlib.h> 
#include <stdio.h> 
#include <math.h>
int main(){ 
 	int m[101]; 
	m[0] = 5 / 2 * 3;
	m[43] = 2+m[2];
	m[19] = 5 - m[4 - 6];
	m[75] = 5 - (4 - 6);
	m[46] = 5 - m[(6 - 4)];
	printf("%d",m[3]);
	m[43] = 5 / 2 * 3;
