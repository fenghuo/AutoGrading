#include <stdio.h>
#include <math.h>

int main() {
  int m[101] {0};
  m[34] = 5 / 2 * 3;
  m[51] = 2 + m[m[34] - m[34] + 34];
  m[65] = 5 - m[22 - 4 + 34] - 6;
  printf("%d",m[51]);
  printf("%d",m[65]);
  printf("%d",m[34]);
  return 0;
}
