#include <stdio.h>
#include <math.h>

int main() {
  int m[101] {0};
  m[34] = 5 / 2 * 3;
  goto L4;
  m[51] = 2 + m[2];
  m[65] = 5 - m[24 * 2 - 2] - 6;
  L1:
  m[57] = 5 - (m[4] - 6);
  m[86] = 5 / 2 * 3;
  goto L6;
  m[34] = 2 + m[2];
  m[32] = 5 - (m[4] - 6);
  m[51] = 2 + m[2];
  m[65] = 5 - m[33 * 2 - 2] - 6;
  L2:
  m[57] = 5 - (m[4] - 6);
  m[86] = 5 / 2 * 3;
  goto L6;
  m[34] = 2 + m[2];
  m[91] = 5 - m[4 - 6];
  m[57] = 5 - (4 - 6);
  m[64] = 5 - m[(6 - 4)];
  printf("%d",m[3]);
  return 0;
}
