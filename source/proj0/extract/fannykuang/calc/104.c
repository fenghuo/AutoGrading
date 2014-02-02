#include<iostream>
#include<cmath>
using namespace std;
int main() {
int m[101];
m[3]=5-(4-6);
m[2]=5-m[(6-4)];
cout << m[3] << endl;
if(m[60])
goto L1;
cout << m[2] << endl;
goto L2;
cout << m[0] << endl;
L2:
cout << m[0] << endl;
m[10]=5/2*3;
m[44]=2+2;
m[50]=5-(m[4]-6);
m[70]=5/2*3;
return 0;
}