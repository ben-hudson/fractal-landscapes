#include <stdbool.h>

typedef struct {
  int size;
  float roughness;
  float height_max;
  float height_min;
  float heights[1]; // beginning of heights array
} Landscape;

Landscape *landscape_create_landscape(int size, float roughness);
void landscape_destroy_landscape(Landscape *landscape);
void landscape_generate_landscape(Landscape *landscape);
float landscape_get_height(Landscape *landscape, int x, int y);
