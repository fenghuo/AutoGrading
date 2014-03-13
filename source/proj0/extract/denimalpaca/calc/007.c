#include <stdio.h>

int power(int a, int b) {
	if(b==0) return 1;
	else if (b==1) return a;
	else return(a * power(a,b-1));
 }

int main() {
	int m[101];
	m[ 0 ] = 5 / 2 * 3 ;
	m[ 4 ] = 88 + 2 ;
	m[ 0 ] = 5 - ( m[ 4 ] - 6 ) ;
	m[ 0 ] = 5 / 2 * 3 ;
	m[ 0 ] = 2 + m[ 2 ] ;
	m[ 0 ] = 5 - m[ power(4,-1) 1 - 2 ] - 6 ;
	L1:
	m[ 0 ] = 5 - ( m[ 4 ] - 6 ) ;
	m[ 0 ] = 5 / power(2,3) ;
	m[ 0 ] = 2 + m[ 2 ] ;
	m[ 0 ] = 5 - m[ 4 - 6 ] ;
	m[ 3 ] = 5 - ( 4 - 6 ) ;
	m[ 2 ] = 5 - m[ ( 6 - 4 ) ] ;
	printf("%d\n", m[ 3 ]);
	if(m [ 60 ] ) goto L1;
	printf("%d\n", m[ 2 ]);
	 goto L2;
	printf("%d\n", m[ 0 ]);
	L2:
	printf("%d\n", m[ 0 ]);
	m[ 0 ] = 5 / 2 * 3 ;
	m[ 4 ] = 2 + 2 ;
	m[ 0 ] = 5 - ( m[ 4 ] - 6 ) ;
	m[ 0 ] = 5 / 2 * 3 ;
	m[ 0 ] = 2 + m[ 2 ] ;
	m[ 0 ] = 5 - m[ 4 - 6 ] 
	return 0;
}