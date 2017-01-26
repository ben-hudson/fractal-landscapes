#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define SIZE  (9)
#define WEST  (0)
#define SOUTH (0)
#define EAST  (SIZE - 1)
#define NORTH (SIZE - 1)

#define ROUGHNESS (0.5)

// random float between min and max
float frand(float min, float max) {
  float f = (float)rand()/RAND_MAX;
  return min + f*(max - min);
}

float get(float *map, int x, int y) {
  return map[y*SIZE + x];
}

void set(float *map, int x, int y, float v) {
  map[y*SIZE + x] = v;
}

// ew
int wrap(int a) {
  int m = 0;
  if (a < 0) {
    m = 1;
  } else if (a >= SIZE) {
    m = -1;
  }
  return a + m*(SIZE - 1);
}

void seed(float *map) {
  srand(time(NULL));
  set(map, WEST, NORTH, frand(0, 1));
  set(map, EAST, NORTH, frand(0, 1));
  set(map, WEST, SOUTH, frand(0, 1));
  set(map, EAST, SOUTH, frand(0, 1));
}

void square(float *map, int x, int y, int l) {
  printf("doing square on (%d, %d)\n", x, y);
  int n = y + l, e = x + l, s = y - l, w = x - l;
  float v = 0.25*get(map, w, n) + 0.25*get(map, e, n) + 0.25*get(map, e, s)
      + 0.25*get(map, w, s) + frand(-ROUGHNESS*l, ROUGHNESS*l);
  set(map, x, y, v);
}

void diamond(float *map, int x, int y, int l) {
  printf("doing diamond on (%d, %d)\n", x, y);
  // if point is unset
  if (get(map, x, y) != get(map, x, y)) {
    int n = wrap(y + l), e = wrap(x + l), s = wrap(y - l), w = wrap(x - l);
    float v = 0.25*get(map, w, y) + 0.25*get(map, x, n) + 0.25*get(map, e, y)
        + 0.25*get(map, x, s) + frand(-ROUGHNESS*l, ROUGHNESS*l);
    set(map, x, y, v);
  }
}

void generate(float *map, int l) {
  int i, j;
  while (l > 0) {
    printf("l = %d\n", l);
    for(i = l; i < SIZE; i += l*2) {
      for(j = l; j < SIZE; j += l*2) {
        square(map, i, j, l);
      }
    }
    for(i = l; i < SIZE; i += l*2) {
      for(j = l; j < SIZE; j += l*2) {
        diamond(map, i - l, j, l);
        diamond(map, i, j - l, l);
        diamond(map, i + l, j, l);
        diamond(map, i, j + l, l);
      }
    }
    l /= 2;
  }
}

// // TODO: delete me
// void display(float *map) {
//   int x, y;
//   for (y = 0; y < SIZE; y++) {
//     for (x = 0; x < SIZE; x++) {
//       printf("%f ", get(map, x, y));
//     }
//     printf("\n");
//   }
//   printf("\n");
// }

int main(int argc, char const *argv[]) {
  float *map = (float *)malloc(sizeof(float)*SIZE*SIZE);
  for (int i = 0; i < SIZE*SIZE; map[i++] = NAN);
  // ^ this guy

  seed(map);
  generate(map, SIZE/2);
  free(map);
  return 0;
}
