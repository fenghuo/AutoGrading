#include<iostream>
#include<cmath>
using namespace std;
int main() {
int m[101];
m[0]=5/2*3;
m[2]=5/2*3;
cout << m[2] << endl;
if(m[0])
goto L1;
goto L2;
L1:
m[75]=5-(m[2]-6**3);
L2:
m[75]=5-(m[0]-6);
cout << m[0] << endl;
cout << m[2] << endl;
cout << m[75] << endl;
return 0;
}