#include <math.h>
#include <stdio.h>

int main() {
  int m[101];

  // -- BEGIN CODE GEN --

  m[0] = 5 / 2 * 3;
  m[2] = 5 / 2 * 3;
  printf("%d\n", m[2]);
  if (m[0]) goto L1;
  goto L2;
  L1:
  m[75] = 5 - (m[2] - ((int) pow(6, 3)));
  L2:
  m[75] = 5 - (m[0] - 6);
  printf("%d\n", m[0]);
  printf("%d\n", m[2]);
  printf("%d\n", m[75]);
  
  // -- END CODE GEN --

  return 0;
}
