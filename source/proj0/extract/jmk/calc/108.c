#include<stdio.h>
#include<math.h>

main()
{

int m[101];

m[ 0 ]  = 5 / 2 * 3; 
m[ 43 ]  = 2 + m[ 2 ]; 
m[ 19 ]  = 5 - m[ 4 - 6 ]; 
m[ 75 ]  = 5 -  ( 4 - 6 ) ; 
m[ 46 ]  = 5 - m[  ( 6 - 4 )  ]; 
printf("%f ",m[ 3 ]);

m[ 43 ]  = 5 / 2 * 3; 
goto L4; 
m[ 15 ]  = 2 + m[ 2 ]; 
m[ 56 ]  = 5 - m[ 23 * 2