#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "landscape.h"

#define SIZE  (513)

int main(int argc, char const *argv[]) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    return 1;
  }
  SDL_Window *window = SDL_CreateWindow("maps 'n shit", 100, 100, SIZE, SIZE, SDL_WINDOW_SHOWN);
  SDL_Surface *surf = SDL_GetWindowSurface(window);

  Landscape *landscape = landscape_create_landscape(SIZE, 0.2);
  landscape_raise_landscape(landscape);

  // this is a 2D test
  float max = -1000000, min = 1000000; // hup
  for (int i = 0; i < landscape->size*landscape->size; i++) {
    if (landscape->heights[i] > max) {
      max = landscape->heights[i];
    }
    if (landscape->heights[i] < min) {
      min = landscape->heights[i];
    }
  }
  // remap heights to colours
  int *bitmap = (int *)malloc(sizeof(int)*landscape->size*landscape->size);
  unsigned char height;
  for (int i = 0; i < landscape->size*landscape->size; i++) {
    height = 255*(landscape->heights[i] - min)/(max - min);
    bitmap[i] = 0 | height << 24 | height << 16 | height << 8;
  }
  SDL_Surface *map = SDL_CreateRGBSurfaceFrom(bitmap, SIZE, SIZE, sizeof(int)*8, SIZE*sizeof(int), 0xFF000000, 0x00FF0000, 0x0000FF00, 0x00000000);
  if (map == NULL) {
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }
  SDL_BlitSurface(map, NULL, surf, NULL);
  SDL_FreeSurface(map);
  free(bitmap);

  SDL_Event e;
  bool run = true;
  while(run) {
    while(SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        run = false;
      }
    }
    SDL_UpdateWindowSurface(window);
  }

  landscape_destroy_landscape(landscape);

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
