#include <stdbool.h>

typedef struct {
  int size;
  float roughness;
  float *heights; // TODO: change this to a flexible length array
} Landscape;

Landscape *landscape_create_landscape(int size, float roughness);
void landscape_destroy_landscape(Landscape *landscape);

bool landscape_raise_landscape(Landscape *landscape);
bool landscape_flatten_landscape(Landscape *Landscape);
