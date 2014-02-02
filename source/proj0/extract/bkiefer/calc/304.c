#include <stdio.h>
#include <math.h>

int main() {
  int m[101] {0};
  m[0] = 5 / 2 * 3;
  m[2] = 5 / 2 * 3;
  printf("%d",m[2]);
  goto L1 if(m[0]);
  goto L2;
  L1:
  m[57] = 5 - (m[2] - 62);
  m[31] = m[2] * m[0];
  L2:
  m[57] = 5 - (m[0] - 6);
  m[31] = m[2] * m[0];
  goto L3;
  printf("%d",5 * 5);
  L3:
  m[1] = 4331;
  printf("%d",m[0]);
  printf("%d",m[2]);
  printf("%d",m[31]);
  printf("%d",m[1]);
  printf("%d",m[57]);
  printf("%d",444666);
  return 0;
}
