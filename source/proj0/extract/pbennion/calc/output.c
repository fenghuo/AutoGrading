#include <stdio.h>
#include <stdlib.h>

int main()
{
	int addr;
	int* m;
	m[ addr = ( 1 ) ] = 5 / 2 * 3 ;
	m[ addr = ( 2 ) ] = 2 ^ ( 3 ^ ( 2 ) ) ;
	m[ addr = ( 3 ) ] = 11 * 3 - 5 ^ ( 4 * 2 + 3 ^ ( 4 ) ) ;
	m[ addr = ( 4 ) ] = 2 + m[ addr = ( 2 ) ] ;
	m[ addr = ( 5 ) ] = 5 - m[ addr = ( 2 ) ] - 6 ;
	printf("%i\n", ( 84848492 ));
	printf("%i\n", ( m[ addr = ( 5 ) ] ));
	printf("%i\n", ( m[ addr = ( 3 ) ] ));
	goto l1;
	m[ addr = ( 5 ) ] = 5 - m[ addr = ( 2 ) ] - 6 ;
	printf("%i\n", ( 124324 ));
	printf("%i\n", ( m[ addr = ( 5 ) ] ));
	l1:
	m[ addr = ( 6 ) ] = 5 - ( m[ addr = ( 5 ) ] - 6 ) ;
	m[ addr = ( 7 ) ] = 3 * 11 - 4 / 2 ^ ( 2 ) ;
	m[ addr = ( 8 ) ] = 2 + m[ addr = ( 7 ) ] ;
	m[ addr = ( 9 ) ] = 3 ^ ( 2 - 2 ^ ( 2 ^ ( 2 - 5 ) ) ) ;
	m[ addr = ( 11 ) ] = 5 - m[ addr = ( 11 - 6 ) ] ;
	m[ addr = ( 12 ) ] = 5 - ( 4 - 6 ) ;
	m[ addr = ( 13 ) ] = 5 - ( 6 ^ ( 8 - m[ addr = ( 11 ) ] ) ) ;
	m[ addr = ( 14 ) ] = 5 / 2 * 3 ;
	printf("%i\n", ( m[ addr = ( 13 ) ] ));
	printf("%i\n", ( m[ addr = ( 14 ) ] ));
	m[ addr = ( 15 ^ ( 1 ) ) ] = 2 + m[ addr = ( 13 ) ] ;
	m[ addr = ( 16 ) ] = 5 - m[ addr = ( 14 - 2 ) ] - 6 ;
	l2:
	m[ addr = ( 17 ) ] = 5 - ( m[ addr = ( 16 ) ] - 6 ) ;
	if(m[ addr = ( 16 ) ]  != 0)		goto l4;
	goto l3;
	m[ addr = ( 15 ) ] = 2 + m[ addr = ( 7 ) ] ;
	m[ addr = ( 16 ) ] = 5 - m[ addr = ( 4 * 2 - 2 + 7 ) ] - 6 ;
	l3:
	m[ addr = ( 15 ) ] = 5 - ( m[ addr = ( 15 ) ] + m[ addr = ( 7 ) ] - 6 ) ;
	m[ addr = ( 16 ) ] = 5 / 2 * 3 ;
	goto l5;
	l4:
	m[ addr = ( 25 ) ] = 5 - ( m[ addr = ( 4 ) ] - 6 ) ;
	printf("%i\n", ( m[ addr = ( 25 ) ] ));
	l5:
	printf("%i\n", ( m[ addr = ( 25 ) ] ));
	printf("%i\n", ( m[ addr = ( 15 ) ] ));
	printf("%i\n", ( m[ addr = ( 16 ) ] ));

	return 0;
}
