#include <stdio.h>

int power(int a, int b) {
	if(b==0) return 1;
	else if (b==1) return a;
	else return(a * power(a,b-1));
 }

int main() {
	int m[101];
	m[ 60 ] = m[ 60 ] - 1 ;
	m[ 0 ] = 5 / 2 * 3 ;
	m[ 4 ] = 1874919416 + 2 ;
	m[ 0 ] = 5 - ( m[ 4 ] - 6 ) ;
	m[ 0 ] = m[ 5 ] / 2 * 3 ;
	m[ 0 ] = 2 + m[ 2 ] ;
	m[ 0 ] = 5 - m[ 4 - 6 ] 
	return 0;
}