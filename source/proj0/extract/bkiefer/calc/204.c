#include <stdio.h>
#include <math.h>

int main() {
  int m[101] {0};
  m[0] = 5 / 2 * 3;
  m[34] = 5 / 2 * 3;
  goto L4;
  m[51] = 2 + m[2];
  m[65] = 5 - m[42 - 2] - 6;
  L11:
  m[57] = 5 - (m[4] - 6);
  m[34] = 2 + m[2];
  m[91] = 5 - m[4 - 6];
  m[57] = 5 - (4 - 6);
  m[64] = 5 - m[(6 - 4)];
  printf("%d",