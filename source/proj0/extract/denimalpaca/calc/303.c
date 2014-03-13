#include <stdio.h>

int power(int a, int b) {
	if(b==0) return 1;
	else if (b==1) return a;
	else return(a * power(a,b-1));
 }

int main() {
	int m[101];
	m[ 0 ] = 5 / 2 * 3 ;
	m[ 2 ] = 5 / 2 * 3 ;
	printf("%d\n", m[ 2 ]);
	if(m [ 0 ] ) goto L1;
	 goto L2;
	L1:
	m[ 75 ] = 5 - ( m[ 2 ] - power(6,-1) ;
	L2:
	m[ 75 ] = 5 - ( m[ 0 ] - 6 ) ;
	printf("%d\n", m[ 0 ]);
	printf("%d\n", m[ 2 ]);
	printf("%d\n", m[ 75 ]);
	
	return 0;
}