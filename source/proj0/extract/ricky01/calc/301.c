#include <stdio.h>
#include<math.h>
int main(){
	int m[101];
	int i=0;
	for(i=0;i<=100;i++)
		m[i]=0;
	m[0]=5/2*3;
	m[43]=100*5-2-5-9/3;
	printf("%d\n",m[0]);
	printf("%d\n",m[43]);
	printf("%d\n",m[0]);
	printf("%d\n",m[43]);
	return 0;
}
