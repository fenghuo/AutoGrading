#include <math.h>
#include <stdio.h>

int main() {
  int m[101];

  // -- BEGIN CODE GEN --

  m[0] = 7;
  m[2] = 1;
  m[3] = 1;
  m[0] = m[0] - 2;
  L1:
  m[1] = m[2] + m[3];
  m[2] = m[3];
  m[3] = m[1];
  m[0] = m[0] - 1;
  if (m[0]) goto L1;
  printf("%d\n", m[1]);
  
  // -- END CODE GEN --

  return 0;
}
