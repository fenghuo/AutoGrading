#include<iostream>
#include<cmath>
using namespace std;
int main() {
int m[101];
m[0]=5;
m[1]=2;
m[10]=3*5;
m[100]=5+(6);
m[50]=5+(4-6);
m[51]=4-6;
m[0]=5-(4-6);
m[60]=4;
L1:
m[60]=m[60]-1;
m[0]=5/2*3;
m[4]=2+2;
m[0]=5-(m[4]-6);
m[0]=5/2*3;
m[0]=2+m[2];
m[0]=5-m[4-6];
m[3]=5-(4-6);
m[2]=5-m[(6-4)];
m[2]=m[m[3]];
cout << m[3] << endl;
if(m[60])
goto L1;
cout << m[2] << endl;
goto L2;
cout << m[0] << endl;
L2:
cout << m[0] << endl;
return 0;
}