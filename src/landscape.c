#include "landscape.h"

#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

// return random float between min and max
float frand(float min, float max) {
  float f = (float)rand()/RAND_MAX;
  return min + f*(max - min);
}

float get(Landscape *map, int x, int y) {
  return map->heights[y*map->size + x];
}

void set(Landscape *map, int x, int y, float v) {
  map->heights[y*map->size + x] = v;
  if (v > map->height_max) {
    map->height_max = v;
  }
  if (v < map->height_min) {
    map->height_min = v;
  }
}

// ew
int wrap(Landscape *map, int a) {
  int m = 0;
  if (a < 0) {
    m = 1;
  } else if (a >= map->size) {
    m = -1;
  }
  return a + m*(map->size - 1);
}

// initialize corners
void seed(Landscape *map) {
  srand(time(NULL));
  set(map, 0, map->size - 1, frand(0, 1));
  set(map, map->size - 1, map->size - 1, frand(0, 1));
  set(map, 0, 0, frand(0, 1));
  set(map, map->size - 1, 0, frand(0, 1));
}

void square(Landscape *map, int x, int y, int l) {
  int n = y + l, e = x + l, s = y - l, w = x - l;
  float v = 0.25*get(map, w, n) + 0.25*get(map, e, n) + 0.25*get(map, e, s) + 0.25*get(map, w, s)
      + frand(-map->roughness*l, map->roughness*l);
  set(map, x, y, v);
}

void diamond(Landscape *map, int x, int y, int l) {
  // if point is unset
  // IEEE NaN returns false for ALL comparisons
  if (get(map, x, y) != get(map, x, y)) {
    int n = wrap(map, y + l), e = wrap(map, x + l), s = wrap(map, y - l), w = wrap(map, x - l);
    float v = 0.25*get(map, w, y) + 0.25*get(map, x, n) + 0.25*get(map, e, y) + 0.25*get(map, x, s)
        + frand(-map->roughness*l, map->roughness*l);
    set(map, x, y, v);
  }
}

void generate(Landscape *map) {
  int l = map->size/2;
  while (l > 0) {
    for (int i = l; i < map->size; i += l*2) {
      for (int j = l; j < map->size; j += l*2) {
        square(map, i, j, l);
      }
    }
    for (int i = l; i < map->size; i += l*2) {
      for(int j = l; j < map->size; j += l*2) {
        diamond(map, i - l, j, l);
        diamond(map, i, j - l, l);
        diamond(map, i + l, j, l);
        diamond(map, i, j + l, l);
      }
    }
    l /= 2;
  }
}

void reset(Landscape *map) {
  for (int i = 0; i < map->size*map->size; map->heights[i++] = NAN);
  map->height_max = FLT_MIN;
  map->height_min = FLT_MAX;
}

Landscape *landscape_create_landscape(int size, float roughness) {
  Landscape *map = (Landscape *)malloc(sizeof(Landscape) + sizeof(float)*(size*size - 1));
  if (map) {
    map->size = size;
    map->roughness = roughness;
    reset(map);
  }
  return map;
}

void landscape_destroy_landscape(Landscape *map) {
  free(map);
}

void landscape_generate_landscape(Landscape *map) {
  reset(map);
  seed(map);
  generate(map);
}

float landscape_get_height(Landscape *map, int x, int y) {
  // if (x >= map->size || y >= map->size) {
  //   return 0;
  // }
  return get(map, x, y);
}
