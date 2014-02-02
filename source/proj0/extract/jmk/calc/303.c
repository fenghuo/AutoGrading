#include<stdio.h>
#include<math.h>

main()
{

int m[101];

m[ 0 ]  = 5 / 2 * 3; 
m[ 2 ]  = 5 / 2 * 3; 
printf("%f ",m[ 2 ]);

 if( m[ 0 ] );
goto L1; 
goto L2; 
L1:
m[ 75 ]  = 5 -  ( m[ 2 ] - 63 ) ; 
L2:
m[ 75 ]  = 5 -  ( m[ 0 ] - 6 ) ; 
printf("%f ",m[ 0 ]);

printf("%f ",m[ 2 ]);

printf("%f ",m[ 75 ]);

}
