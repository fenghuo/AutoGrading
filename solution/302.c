#include <math.h>
#include <stdio.h>

int main() {
  int m[101];

  // -- BEGIN CODE GEN --

  m[43] = 5 / 2 * 3;
  m[15] = 2 + m[m[43] - m[43] + 43];
  m[56] = 5 - m[((int) pow(2, 2)) - 4 + 43] - 6;
  printf("%d\n", m[15]);
  printf("%d\n", m[56]);
  printf("%d\n", m[43]);
  
  // -- END CODE GEN --

  return 0;
}
