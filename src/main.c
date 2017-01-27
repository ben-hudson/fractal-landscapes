#include <GL/glut.h>
#include <stdbool.h>

#include "landscape.h"

#define ROUGHNESS (0.2)

#define GRID_SIZE    (65)
#define GRID_SPACING (0.4)

static Landscape *landscape = NULL;

void cleanup(void) {
  if (landscape) {
    landscape_destroy_landscape(landscape);
  }
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBegin(GL_QUADS);
  for (int i = 0; i < landscape->size - 1; i++) {
    for (int j = 0; j < landscape->size - 1; j++) {
      glVertex3f(i*GRID_SPACING, j*GRID_SPACING, landscape_get_height(landscape, i, j));
      glVertex3f(i*GRID_SPACING, (j + 1)*GRID_SPACING, landscape_get_height(landscape, i, j + 1));
      glVertex3f((i + 1)*GRID_SPACING, (j + 1)*GRID_SPACING, landscape_get_height(landscape, i + 1, j + 1));
      glVertex3f((i + 1)*GRID_SPACING, j*GRID_SPACING, landscape_get_height(landscape, i + 1, j));
    }
  }
  glEnd();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(-0.5*(GRID_SIZE - 1)*GRID_SPACING, -0.5*(GRID_SIZE - 1)*GRID_SPACING, -20.0); // move into center
  glRotatef(-60.0, 1.0, 0.0, 0.0); // rotate instead of the camera
  glTranslatef(0, 6.0, 4.0);

  glutSwapBuffers();
}

void reshape(int w, int h) {
  glViewport (0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, 1.0, 1.5, 50.0);
}

int main(int argc, char *argv[]) {
  atexit(cleanup); // fu glut

  landscape = landscape_create_landscape(GRID_SIZE, ROUGHNESS);
  landscape_raise_landscape(landscape);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (500, 500);
  glutCreateWindow("Fractal Landscapes");

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMainLoop();

  return 0;
}
