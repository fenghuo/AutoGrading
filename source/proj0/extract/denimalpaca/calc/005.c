#include <stdio.h>

int power(int a, int b) {
	if(b==0) return 1;
	else if (b==1) return a;
	else return(a * power(a,b-1));
 }

int main() {
	int m[101];
	;
	L1:
	m[ 4 ] = 15 - ( m[ 4 ] - 6 ) ;
	m[ 5 ] = 25 / 12 * 3 ;
	m[ 6 ] = 32 + m[ 42 ] ;
	m[ 7 ] = 45 - m[ 54 - 36 ] ;
	m[ 8 ] = 55 - ( 14 - 26 ) ;
	m[ 9 ] = 65 - m[ ( 36 - 24 ) ] ;
	printf("%d\n", m[ 3 ]);
	if(m [ 0 ] ) goto L1;
	printf("%d\n", m[ 12 ]);
	 goto L5;
	printf("%d\n", m[ 90 ]);
	L2:
	printf("%d