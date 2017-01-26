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

// initialize corners between 0 and 1
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
  float v = 0.25*get(map, w, n) + 0.25*get(map, e, n) + 0.25*get(map, e, s) + 0.25*get(map, w, s) + frand(-ROUGHNESS*l, ROUGHNESS*l);
  set(map, x, y, v);
}

// TODO: add wrapping
void diamond(float *map, int x, int y, int l) {
  printf("doing diamond on (%d, %d)\n", x, y);
  // if point is unset (is NaN)
  if (get(map, x, y) != get(map, x, y)) {
    int n = y + l, e = x + l, s = y - l, w = x - l;
    // printf("(x, y) = (%d, %d)\n", x, y);
    // printf("n = %d, e = %d, s = %d, w = %d\n", n, e, s, w);
    float v;
    if (x == WEST) {
      //printf("WEST!\n");
      v = 0.33*get(map, x, n) + 0.33*get(map, e, y) + 0.33*get(map, x, s);
    } else if (y == NORTH) {
      //printf("NORTH!\n");
      v = 0.33*get(map, w, y) + 0.33*get(map, e, y) + 0.33*get(map, x, s);
    } else if (x == EAST) {
      //printf("EAST!\n");
      v = 0.33*get(map, w, y) + 0.33*get(map, x, n) + 0.33*get(map, x, s);
    } else if (y == SOUTH) {
      //printf("SOUTH!\n");
      v = 0.33*get(map, w, y) + 0.33*get(map, x, n) + 0.33*get(map, e, y);
    } else {
      v = 0.25*get(map, w, y) + 0.25*get(map, x, n) + 0.25*get(map, e, y) + 0.25*get(map, x, s);
    }
    v += frand(-ROUGHNESS*l, ROUGHNESS*l);
    set(map, x, y, v);
  } else {
    printf("already set\n");
  }
}

/*void step(float *map, int x, int y, int l) {
  // printf("l = %d\n", l);
  if (l > 0) {
    square(map, x, y, l);
    diamond(map, x - l, y, l);
    diamond(map, x, y - l, l);
    diamond(map, x + l, y, l);
    diamond(map, x, y + l, l);

    step(map, x - l/2, y + l/2, l/2);
    step(map, x + l/2, y + l/2, l/2);
    step(map, x - l/2, y - l/2, l/2);
    step(map, x + l/2, y - l/2, l/2);
  }
}*/

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

// TODO: delete me
void display(float *map) {
  int x, y;
  for (y = 0; y < SIZE; y++) {
    for (x = 0; x < SIZE; x++) {
      printf("%f ", get(map, x, y));
    }
    printf("\n");
  }
  printf("\n");
}

int main(int argc, char const *argv[]) {
  float *map = (float *)malloc(sizeof(float)*SIZE*SIZE);
  for (int i = 0; i < SIZE*SIZE; map[i++] = NAN);

  seed(map);
  display(map);
  generate(map, SIZE/2);
  display(map);
  free(map);
  return 0;
}
