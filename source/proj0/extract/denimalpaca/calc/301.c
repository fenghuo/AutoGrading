#include <stdio.h>

int power(int a, int b) {
	if(b==0) return 1;
	else if (b==1) return a;
	else return(a * power(a,b-1));
 }

int main() {
	int m[101];
	m[ 0 ] = 5 / 2 * 3 ;
	m[ 43 ] = 100 * 5 - 2 - 5 - 9 / 3 ;
	printf("%d\n", m[ 0 ]);
	printf("%d\n", m[ 43 ]);
	printf("%d\n", m[ 0 ]);
	printf("%d\n", m[ 43 ]);
	
	return 0;
}