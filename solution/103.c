#include <math.h>
#include <stdio.h>

int main() {
  int m[101];

  // -- BEGIN CODE GEN --

  m[4] = 88 + 2;
  m[23] = 5 - (m[4] - 6);
  m[43] = 5 / 2 * 3;
  m[15] = 2 + m[2];
  m[56] = 5 - m[((int) pow(4, 4)) * 2 - 2] - 6;
  L1:
  m[75] = 5 - (m[4] - 6);
  m[68] = 5 / 2 * 3;
  m[43] = 2 + m[2];
  m[19] = 5 - m[4 - 6];
  m[75] = 5 - (4 - 6);
  m[46] = 5 - m[(6 - 4)];
  printf("%d\n", m[3]);
  
  // -- END CODE GEN --

  return 0;
}
