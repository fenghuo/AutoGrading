#include<stdio.h>
#include<math.h>

main()
{

int m[101];

m[ 43 ]  = 5 / 2 * 3; 
m[ 15 ]  = 2 + m[ m[ 43 ] - m[ 43 ] + 43 ]; 
m[ 56 ]  = 5 - m[ 22 - 4 + 43 ] - 6; 
printf("%d ",m[ 15 ]);

printf("%d ",m[ 56 ]);

printf("%d ",m[ 43 ]);

}
