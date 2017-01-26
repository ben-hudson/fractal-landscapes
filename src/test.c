#include <stdio.h>
#include <math.h>
#include <stdint.h>

// this is gross af
#define IS_NAN(X) (*((uint32_t *)&(X)) == 0x7FC00000)

int main() {
  float a = NAN;
  uint32_t *b = (uint32_t *)&a;
  printf("%x, %f", *b, a);
  printf("%d", a != a);
  printf("%d", IS_NAN(a));
}
