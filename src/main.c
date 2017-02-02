#include <GL/glut.h>
#include <stdbool.h>
#include <stdio.h>

#include "landscape.h"

#define ROUGHNESS    (0.4)

#define GRID_SIZE    (65)
#define GRID_SPACING (1)

#define FRAME_RATE   (30)
#define FRAME_PERIOD (1000/FRAME_RATE)

static Landscape *landscape = NULL;
static int rotation = 0;

static float land_specular[4] = {0.0, 0.0, 0.0, 1.0};
static float water_specular[4] = {-0.2, -0.2, 1.0, 1.0};

void cleanup(void) {
  if (landscape) {
    landscape_destroy_landscape(landscape);
  }
}

void draw_landscape(void) {
  glMaterialfv(GL_FRONT, GL_SPECULAR, land_specular);
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < landscape->size - 1; i++) {
    for (int j = 0; j < landscape->size - 1; j++) {
      // normal calculation
      //   2---3
      //   |  /|
      // y u v |
      //   |/  |
      //   1-w-2'
      //     x

      // 1-2-3 triangle
      // n = vxu
      // n_x = v_y*u_z - v_z*u_y
      // n_y = v_z*u_x - v_x*u_z
      // n_z = v_x*u_y - v_y*u_x
      float u_z, v_z, w_z;
      u_z = landscape_get_height(landscape, i, j + 1) - landscape_get_height(landscape, i, j);
      v_z = landscape_get_height(landscape, i + 1, j + 1) - landscape_get_height(landscape, i, j);
      w_z = landscape_get_height(landscape, i + 1, j) - landscape_get_height(landscape, i, j);
      glNormal3f(GRID_SPACING*(u_z - v_z), -GRID_SPACING*u_z, GRID_SPACING*GRID_SPACING);
      glVertex3f(i*GRID_SPACING, j*GRID_SPACING, landscape_get_height(landscape, i, j));
      glVertex3f(i*GRID_SPACING, (j + 1)*GRID_SPACING, landscape_get_height(landscape, i, j + 1));
      glVertex3f((i + 1)*GRID_SPACING, (j + 1)*GRID_SPACING, landscape_get_height(landscape, i + 1,
          j + 1));
      // 1-2'-3 triangle
      // n = wxv
      // n_x = w_y*v_z - w_z*v_y
      // n_y = w_z*v_x - w_x*v_z
      // n_z = w_x*v_y - w_y*v_x
      glNormal3f(-w_z*GRID_SPACING, GRID_SPACING*(w_z - v_z), GRID_SPACING*GRID_SPACING);
      glVertex3f(i*GRID_SPACING, j*GRID_SPACING, landscape_get_height(landscape, i, j));
      glVertex3f((i + 1)*GRID_SPACING, j*GRID_SPACING, landscape_get_height(landscape, i + 1, j));
      glVertex3f((i + 1)*GRID_SPACING, (j + 1)*GRID_SPACING, landscape_get_height(landscape, i + 1,
          j + 1));
    }
  }
  // close the sides
  for (int i = 0; i < landscape->size - 1; i++) {
    // x planes
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(0.0, GRID_SPACING*i, landscape->height_min);
    glVertex3f(0.0, GRID_SPACING*i, landscape_get_height(landscape, 0, i));
    glVertex3f(0.0, GRID_SPACING*(i + 1), landscape_get_height(landscape, 0, i + 1));
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(0.0, GRID_SPACING*i, landscape->height_min);
    glVertex3f(0.0, GRID_SPACING*(i + 1), landscape->height_min);
    glVertex3f(0.0, GRID_SPACING*(i + 1), landscape_get_height(landscape, 0, i + 1));
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f((landscape->size - 1)*GRID_SPACING, GRID_SPACING*i, landscape->height_min);
    glVertex3f((landscape->size - 1)*GRID_SPACING, GRID_SPACING*i,
        landscape_get_height(landscape, (landscape->size - 1), i));
    glVertex3f((landscape->size - 1)*GRID_SPACING, GRID_SPACING*(i + 1),
        landscape_get_height(landscape, (landscape->size - 1), i + 1));
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f((landscape->size - 1)*GRID_SPACING, GRID_SPACING*i, landscape->height_min);
    glVertex3f((landscape->size - 1)*GRID_SPACING, GRID_SPACING*(i + 1),
        landscape->height_min);
    glVertex3f((landscape->size - 1)*GRID_SPACING, GRID_SPACING*(i + 1),
        landscape_get_height(landscape, (landscape->size - 1), i + 1));

    // y planes
    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(GRID_SPACING*i, 0.0, landscape->height_min);
    glVertex3f(GRID_SPACING*i, 0.0, landscape_get_height(landscape, i, 0));
    glVertex3f(GRID_SPACING*(i + 1), 0.0, landscape_get_height(landscape, i + 1, 0));
    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(GRID_SPACING*i, 0.0, landscape->height_min);
    glVertex3f(GRID_SPACING*(i + 1), 0.0, landscape->height_min);
    glVertex3f(GRID_SPACING*(i + 1), 0.0, landscape_get_height(landscape, i + 1, 0));
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(GRID_SPACING*i, (landscape->size - 1)*GRID_SPACING, landscape->height_min);
    glVertex3f(GRID_SPACING*i, (landscape->size - 1)*GRID_SPACING,
        landscape_get_height(landscape, i, (landscape->size - 1)));
    glVertex3f(GRID_SPACING*(i + 1), (landscape->size - 1)*GRID_SPACING,
        landscape_get_height(landscape, i + 1, (landscape->size - 1)));
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(GRID_SPACING*i, (landscape->size - 1)*GRID_SPACING, landscape->height_min);
    glVertex3f(GRID_SPACING*(i + 1), (landscape->size - 1)*GRID_SPACING,
        landscape->height_min);
    glVertex3f(GRID_SPACING*(i + 1), (landscape->size - 1)*GRID_SPACING,
        landscape_get_height(landscape, i + 1, (landscape->size - 1)));
  }
  glEnd();
}

void draw_water(void) {
  float water_height = (landscape->height_max + landscape->height_min)/2;

  glMaterialfv(GL_FRONT, GL_SPECULAR, water_specular);
  glBegin(GL_TRIANGLES);
  // top plane
  glNormal3f(0.0, 0.0, 1.0);
  glVertex3f(0.0, 0.0, water_height);
  glVertex3f(0.0, (GRID_SIZE - 1)*GRID_SPACING, water_height);
  glVertex3f((GRID_SIZE - 1)*GRID_SPACING, (GRID_SIZE - 1)*GRID_SPACING, water_height);
  glNormal3f(0.0, 0.0, 1.0);
  glVertex3f(0.0, 0.0, water_height);
  glVertex3f((GRID_SIZE - 1)*GRID_SPACING, 0.0, water_height);
  glVertex3f((GRID_SIZE - 1)*GRID_SPACING, (GRID_SIZE - 1)*GRID_SPACING, water_height);

  // x planes
  glNormal3f(-1.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, water_height);
  glVertex3f(0.0, (GRID_SIZE - 1)*GRID_SPACING, landscape->height_min);
  glVertex3f(0.0, 0.0, landscape->height_min);
  glNormal3f(-1.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, water_height);
  glVertex3f(0.0, (GRID_SIZE - 1)*GRID_SPACING, water_height);
  glVertex3f(0.0, (GRID_SIZE - 1)*GRID_SPACING, landscape->height_min);
  glNormal3f(1.0, 0.0, 0.0);
  glVertex3f((GRID_SIZE - 1)*GRID_SPACING, 0.0, water_height);
  glVertex3f((GRID_SIZE - 1)*GRID_SPACING, (GRID_SIZE - 1)*GRID_SPACING, landscape->height_min);
  glVertex3f((GRID_SIZE - 1)*GRID_SPACING, 0.0, landscape->height_min);
  glNormal3f(1.0, 0.0, 0.0);
  glVertex3f((GRID_SIZE - 1)*GRID_SPACING, 0.0, water_height);
  glVertex3f((GRID_SIZE - 1)*GRID_SPACING, (GRID_SIZE - 1)*GRID_SPACING, water_height);
  glVertex3f((GRID_SIZE - 1)*GRID_SPACING, (GRID_SIZE - 1)*GRID_SPACING, landscape->height_min);

  // y planes
  glNormal3f(0.0, -1.0, 0.0);
  glVertex3f(0.0, 0.0, water_height);
  glVertex3f((GRID_SIZE - 1)*GRID_SPACING, 0.0, landscape->height_min);
  glVertex3f(0.0, 0.0, landscape->height_min);
  glNormal3f(0.0, -1.0, 0.0);
  glVertex3f(0.0, 0.0, water_height);
  glVertex3f((GRID_SIZE - 1)*GRID_SPACING, 0.0, water_height);
  glVertex3f((GRID_SIZE - 1)*GRID_SPACING, 0.0, landscape->height_min);
  glNormal3f(0.0, 1.0, 0.0);
  glVertex3f(0.0, (GRID_SIZE - 1)*GRID_SPACING, water_height);
  glVertex3f((GRID_SIZE - 1)*GRID_SPACING, (GRID_SIZE - 1)*GRID_SPACING, landscape->height_min);
  glVertex3f(0.0, (GRID_SIZE - 1)*GRID_SPACING, landscape->height_min);
  glNormal3f(0.0, 1.0, 0.0);
  glVertex3f(0.0, (GRID_SIZE - 1)*GRID_SPACING, water_height);
  glVertex3f((GRID_SIZE - 1)*GRID_SPACING, (GRID_SIZE - 1)*GRID_SPACING, water_height);
  glVertex3f((GRID_SIZE - 1)*GRID_SPACING, (GRID_SIZE - 1)*GRID_SPACING, landscape->height_min);
  glEnd();
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // draw
  draw_landscape();
  draw_water();
  // center and spin
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotatef(rotation, 0.0, 0.0, 1.0);
  glTranslatef(-0.5*(landscape->size - 1)*GRID_SPACING, -0.5*(landscape->size - 1)*GRID_SPACING,
      -(landscape->height_max + landscape->height_min)/2);

  glutSwapBuffers();
}

void refresh(int value) {
  rotation = (rotation + 1)%360;
  glutTimerFunc(FRAME_PERIOD, refresh, 0);
  glutPostRedisplay();
}

void reshape(int w, int h) {
  glViewport (0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-30.0*w/h, 30.0*w/h, -30.0, 30.0, -46.0, 46.0);
  gluLookAt(0.0, -1.0, 0.4, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard_handler(unsigned char key, int x, int y) {
  landscape_generate_landscape(landscape);
}

int main(int argc, char *argv[]) {
  atexit(cleanup); // fu glut

  landscape = landscape_create_landscape(GRID_SIZE, ROUGHNESS);
  landscape_generate_landscape(landscape);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (1100, 400);
  glutCreateWindow("Fractal Landscapes");

  glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);

  // add light
  GLfloat light_position[4] = {0.0, 1.0, 1.0, 0.0};
  GLfloat light_ambient[4] = {0.2, 0.2, 0.2, 1.0};
  GLfloat light_specular[4] = {1.0, 1.0, 1.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard_handler);
  glutTimerFunc(FRAME_PERIOD, refresh, 0);
  glutMainLoop();
  return 0;
}
