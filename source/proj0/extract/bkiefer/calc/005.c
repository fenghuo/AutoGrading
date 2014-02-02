#include <stdio.h>
#include <math.h>

int main() {
  int m[101] {0};
  L1:
  m[4] = 51 - (m[4] - 6);
  m[5] = 52 / 21 * 3;
  m[6] = 23 + m[24];
  m[7] = 54 - m[45 - 63];
  m[8] = 55 - (41 - 62);
  m[9] = 56 - m[(63 - 42)];
  printf("%d",m[3]);
  goto L1 if(m[0]);
  printf("%d",m[21]);
  goto L5;
  printf("%d",m[9]);
  L2:
  printf("%d",m[8]);
  return 0;
}
