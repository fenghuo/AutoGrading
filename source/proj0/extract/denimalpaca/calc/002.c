#include <stdio.h>

int power(int a, int b) {
	if(b==0) return 1;
	else if (b==1) return a;
	else return(a * power(a,b-1));
 }

int main() {
	int m[101];
	m[ 68 ] = m[ 11 ] - 1 ;
	m[ 3 ] = 5 / 2 * 3 ;
	m[ 4 ] = 2 + 2 ;
	m[ 2 ] = 5 - ( m[ 4 ] - 6 ) ;
	m[ 1 ] = 5 / 2 * 3 ;
	m[ 7 ] = 2 + m[ 2 ] ;
	m[ 8 ] = 5 - m[ 4 - 6 ] 
	return 0;
}