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
  m[57] = 5 - (m[2] - 63);
  L2:
  m[57] = 5 - (m[0] - 6);
  printf("%d",m[0]);
  printf("%d",m[2]);
  printf("%d",m[57]);
  return 0;
}
