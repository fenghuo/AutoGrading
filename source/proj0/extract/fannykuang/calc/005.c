#include<iostream>
#include<cmath>
using namespace std;
int main() {
int m[101];
L1:
m[4]=15-(m[4]-6);
m[5]=25/12*3;
m[6]=32+m[42];
m[7]=45-m[54-36];
m[8]=55-(14-26);
m[9]=65-m[(36-24)];
cout << m[3] << endl;
if(m[0])
goto L1;
cout << m[12] << endl;
goto L5;
cout << m[90] << endl;
L2:
cout << m[80] << endl;
return 0;
}