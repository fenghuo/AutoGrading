#include <stdio.h>
#include <math.h>

int main() {
  int m[101] {0};
  m[3] = 5 - (4 - 6);
  m[2] = 5 - m[(6 - 4)];
  printf("%d",m[3]);
  goto L1 if(m[6]);
  printf("%d",m[2]);
  goto L2;
  printf("%d",m[0]);
  L2:
  printf("%d",m[0]);
  m[1] = 5 / 2 * 3;
  m[44] = 2 + 2;
  m[5] = 5 - (m[4] - 6);
  m[7] = 5 / 2 * 3;
  return 0;
}
