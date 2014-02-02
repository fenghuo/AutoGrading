#include <stdio.h>
#include <math.h>

int main() {
  int m[101] {0};
  m[0] = 5 / 2 * 3;
  m[34] = 5 / 2 * 3;
  goto L4;
  m[51] = 2 + m[2];
  m[65] = 5 - m[41 * 2 - 2] - 6;
  L111