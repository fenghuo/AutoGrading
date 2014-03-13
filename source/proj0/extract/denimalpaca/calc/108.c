#include <stdio.h>

int power(int a, int b) {
	if(b==0) return 1;
	else if (b==1) return a;
	else return(a * power(a,b-1));
 }

int main() {
	int m[101];
	m[ 0 ] = 5 / 2 * 3 ;
	m[ 43 ] = 2 + m[ 2 ] ;
	m[ 19 ] = 5 - m[ 4 - 6 ] ;
	m[ 75 ] = 5 - ( 4 - 6 ) ;
	m[ 46 ] = 5 - m[ ( 6 - 4 ) ] ;
	printf("%d\n", m[ 3 ]);
	m[ 43 ] = 5 / 2 * 3 ;
	 goto L4;
	m[ 15 ] = 2 + m[ 2 ] ;
	m[ 56 ] = 5 - m[ power(2,-1) 2 - 2 ] - 6 ;
	L1:
	m[ 75 ] = 5 - ( m[ 4 ] - 6 ) ;
	if(m [ 60 ] ) goto L1;
	 goto L4;
	m[ 15 ] = 2 + m[ 2 ] ;
	m[ 56 ] = 5 - m[ power(3,-1) 2 - 2 ] - 6 ;
	L1:
	m[ 75 ] = 5 - ( m[ 4 ] - 6 ) ;
	m[ 68 ] = 5 / 2 * 3 
	return 0;
}