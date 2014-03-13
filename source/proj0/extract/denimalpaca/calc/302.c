#include <stdio.h>

int power(int a, int b) {
	if(b==0) return 1;
	else if (b==1) return a;
	else return(a * power(a,b-1));
 }

int main() {
	int m[101];
	m[ 43 ] = 5 / 2 * 3 ;
	m[ 15 ] = 2 + m[ m[ 43 ] - m[ 43 ] + 43 ] ;
	m[ 56 ] = 5 - m[ power(2,-1) 4 + 43 ] - 6 ;
	printf("%d\n", m[ 15 ]);
	printf("%d\n", m[ 56 ]);
	printf("%d\n", m[ 43 ]);
	
	return 0;
}