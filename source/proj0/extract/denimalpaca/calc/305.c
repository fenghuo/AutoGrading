#include <stdio.h>

int power(int a, int b) {
	if(b==0) return 1;
	else if (b==1) return a;
	else return(a * power(a,b-1));
 }

int main() {
	int m[101];
	m[ 1 ] = 5 / 2 * 3 ;
	m[ 2 ] = power(2,-1) 2 ;
	m[ 3 ] = 11 * 3 - power(5,-1) 2 + power(3,-1) m[ 4 ] = 2 + m[ 2 ] ;
	m[ 5 ] = 5 - m[ 2 ] - 6 ;
	printf("%d\n", m[ 84848492 ]);
	printf("%d\n", m[ 5 ]);
	printf("%d\n", m[ 3 ]);
	 goto L1;
	m[ 5 ] = 5 - m[ 2 ] - 6 ;
	printf("%d\n", m[ 124324 ]);
	printf("%d\n", m[ 5 ]);
	L1:
	m[ 6 ] = 5 - ( m[ 5 ] - 6 ) ;
	m[ 7 ] = 3 * 11 - 4 / power(2,-1) m[ 8 ] = 2 + m[ 7 ] ;
	m[ 9 ] = power(3,-1) power(2,-1) 2 - 5 ;
	m[ 11 ] = 5 - m[ 11 - 6 ] ;
	m[ 12 ] = 5 - ( 4 - 6 ) ;
	m[ 13 ] = 5 - ( power(6,-1) m[ 11 ] ) ;
	m[ 14 ] = 5 / 2 * 3 ;
	printf("%d\n", m[ 13 ]);
	printf("%d\n", m[ 14 ]);
	m[ power(15,-1) = 2 + m[ 13 ] ;
	m[ 16 ] = 5 - m[ 14 - 2 ] - 6 ;
	L2:
	m[ 17 ] = 5 - ( m[ 16 ] - 6 ) ;
	if(m [ 16 ] ) goto L4;
	 goto L3;
	m[ 15 ] = 2 + m[ 7 ] ;
	m[ 16 ] = 5 - m[ 4 * 2 - 2 + 7 ] - 6 ;
	L3:
	m[ 15 ] = 5 - ( m[ 15 ] + m[ 7 ] - 6 ) ;
	m[ 16 ] = 5 / 2 * 3 ;
	 goto L5;
	L4:
	m[ 25 ] = 5 - ( m[ 4 ] - 6 ) ;
	printf("%d\n", m[ 25 ]);
	L5:
	printf("%d\n", m[ 25 ]);
	printf("%d\n", m[ 15 ]);
	printf("%d\n", m[ 16 ]);
	
	return 0;
}