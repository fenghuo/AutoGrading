#include <stdio.h>
#include <math.h>

int main(){
int m[101];
m[0] = 5 / 2 * 3;
m[2] = 5 / 2 * 3;
printf("%d\n",m[2]);
 if( m[0]){
     goto L1;
    }
goto 2;
L1:
m[75] = 5 - ( m[2] - (int) pow(6,2) );
m[13] = m[2] * m[0];
L2:
m[75] = 5 - ( m[0] - 6 );
m[13] = m[2] * m[0];
goto 3;
printf("%d\n",5 * 5);
L3:
m[10] = 1334;
printf("%d\n",m[0]);
printf("%d\n",m[2]);
printf("%d\n",m[13]);
printf("%d\n",m[10]);
printf("%d\n",m[75]);
printf("%d\n",666444);

return 0;
  }