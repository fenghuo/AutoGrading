#include <stdio.h>

int power(int a, int b) {
	if(b==0) return 1;
	else if (b==1) return a;
	else return(a * power(a,b-1));
 }

int main() {
	int m[101];
	m[ 0 ] = 2 + m[ 2 ] ;
	m[ 43 ] = 5 - m[ power(4,-1) 1 - 2 ] - 6 ;
	m[ 0 ] = 5 / 2 * 3 
	return 0;
}