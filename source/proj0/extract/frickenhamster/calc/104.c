#include <stdio.h>
#include <math.h>
int main(void)
{
int m[101];
m[3] = 5-(4-6);
m[2] = 5-m[(6-4)];
printf("%d",m[3]);
if(m[60])goto label1;
printf("%d",m[2]);
goto label2;
printf("%d",m[0]);
label2:printf("%d",m[0]);
m[10] = 5/2*3;
m[44] = 2+2;
m[50] = 5-(m[4]-6);
m[70] = 5/2*3;
}
