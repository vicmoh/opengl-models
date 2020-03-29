// Calculates a ray that passes through each pixel of the viewing window.
// Draws randomly coloured pixel in the viewing window.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// MacOS OpenGL Headers
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

// My libraries
#include "dynamic_string.h"
#include "file_reader.h"
#include "gl_setup.h"
#include "model.h"
#include "point.h"

// Show print if debug is true.
#define DEBUG true
#define SHOW_TEST false
#define SHOW_LIGHT_ARROW false
#define SHOW_MOVING_LIGHT false

// The patsed data file of PLY.
static double _rotate = 0;

/* flags used to control the appearance of the image */
static int _lineDrawing = 1;    // draw polygons as solid or lines
static int _lighting = 0;       // use diffuse and specular lighting
static int _smoothShading = 0;  // smooth or flat shading
static int _textures = 0;
static GLuint _textureID[1];

// Colors
const GLfloat BLUE[] = {0.0, 0.0, 1.0, 1.0};
const GLfloat RED[] = {1.0, 0.0, 0.0, 1.0};
const GLfloat GREEN[] = {0.0, 1.0, 0.0, 1.0};
const GLfloat WHITE[] = {1.0, 1.0, 1.0, 1.0};

// Points
static Point _cameraPos = {.x = 0, .y = 0, .z = 0};

/**
 * Draw the face from multiple vertex.
 * @param index of the face.
 */
static void drawFace(int index) {
  MEM_TRACK
  Splitter *faceSplit = Model_parsedData->faceList->at[index];
  if (isStringEqual(faceSplit->at[0], "3")) glBegin(GL_TRIANGLES);
  if (isStringEqual(faceSplit->at[0], "4")) glBegin(GL_QUADS);
  double offsetY = fabs(*Model_parsedData->minY);
  for_in(next, faceSplit) {
    if (next == 0) continue;
    int curPos = atoi(faceSplit->at[next]);
    Point *curVertex = Model_parsedData->vertices->at[curPos];
    double sumOfBoundary =
        fabs(*Model_parsedData->maxX) + fabs(*Model_parsedData->minX) +
        fabs(*Model_parsedData->maxY) + fabs(*Model_parsedData->minY) +
        fabs(*Model_parsedData->maxZ) + fabs(*Model_parsedData->minZ);
    double avg = sumOfBoundary / 6;
    double normalizer = 5 / avg;
    double x = curVertex->x * normalizer;
    double y = (curVertex->y + offsetY + 1) * normalizer;
    double z = curVertex->z * normalizer;
    double distance = sqrt(x * x + y * y + z * z);
    glNormal3f(x / distance, y / distance, z / distance);
    glVertex3f(x, y, z);
  }
  glEnd();
  MEM_SWEEP
}

/**
 * Draw Based on parsed data.
 */
static void drawModel() {
  for_in(next, Model_parsedData->faceList) drawFace(next);
}

static void redraw();
static void update() {
  _rotate += 1.0;
  redraw();
}

/* -------------------------------------------------------------------------- */
/*                              Open GL Functions                             */
/* -------------------------------------------------------------------------- */

// Variable controlling various rendering modes.
static int animation = 1;
static int directionalLight = 1;

static GLfloat floorPlane[4];
static GLfloat floorShadow[4][4];

// Control for the camera angle and lighting.
static float lightAngle = 0.0, lightHeight = 20;
GLfloat angle = -150;  // in degrees
GLfloat angle2 = 30;   // in degrees

int moving = 0, startx = 0, starty;
int lightMoving = 0, lightStartX = 0, lightStartY = 0;

static GLfloat lightPosition[4];
static GLfloat lightColor[] = {0.8, 1.0, 0.8, 1.0};  // Green-tinted light.

static GLfloat floorVertices[4][3] = {
    {-20.0, 0.0, 20.0},
    {20.0, 0.0, 20.0},
    {20.0, 0.0, -20.0},
    {-20.0, 0.0, -20.0},
};

enum { X, Y, Z, W };
enum { A, B, C, D };

/* Create a matrix that will project the desired shadow. */
void shadowMatrix(GLfloat shadowMat[4][4], GLfloat groundplane[4],
                  GLfloat lightpos[4]) {
  // Find dot product between light position vector and ground plane normal.
  GLfloat dot = groundplane[X] * lightpos[X] + groundplane[Y] * lightpos[Y] +
                groundplane[Z] * lightpos[Z] + groundplane[W] * lightpos[W];

  shadowMat[0][0] = dot - lightpos[X] * groundplane[X];
  shadowMat[1][0] = 0.f - lightpos[X] * groundplane[Y];
  shadowMat[2][0] = 0.f - lightpos[X] * groundplane[Z];
  shadowMat[3][0] = 0.f - lightpos[X] * groundplane[W];

  shadowMat[0][1] = 0.f - lightpos[Y] * groundplane[X];
  shadowMat[1][1] = dot - lightpos[Y] * groundplane[Y];
  shadowMat[2][1] = 0.f - lightpos[Y] * groundplane[Z];
  shadowMat[3][1] = 0.f - lightpos[Y] * groundplane[W];

  shadowMat[0][2] = 0.f - lightpos[Z] * groundplane[X];
  shadowMat[1][2] = 0.f - lightpos[Z] * groundplane[Y];
  shadowMat[2][2] = dot - lightpos[Z] * groundplane[Z];
  shadowMat[3][2] = 0.f - lightpos[Z] * groundplane[W];

  shadowMat[0][3] = 0.f - lightpos[W] * groundplane[X];
  shadowMat[1][3] = 0.f - lightpos[W] * groundplane[Y];
  shadowMat[2][3] = 0.f - lightpos[W] * groundplane[Z];
  shadowMat[3][3] = dot - lightpos[W] * groundplane[W];
}

/* Find the plane equation given 3 points. */
void findPlane(GLfloat plane[4], GLfloat v0[3], GLfloat v1[3], GLfloat v2[3]) {
  GLfloat vec0[3], vec1[3];
  /* Need 2 vectors to find cross product. */
  vec0[X] = v1[X] - v0[X];
  vec0[Y] = v1[Y] - v0[Y];
  vec0[Z] = v1[Z] - v0[Z];

  vec1[X] = v2[X] - v0[X];
  vec1[Y] = v2[Y] - v0[Y];
  vec1[Z] = v2[Z] - v0[Z];
  /* find cross product to get A, B, and C of plane equation */
  plane[A] = vec0[Y] * vec1[Z] - vec0[Z] * vec1[Y];
  plane[B] = -(vec0[X] * vec1[Z] - vec0[Z] * vec1[X]);
  plane[C] = vec0[X] * vec1[Y] - vec0[Y] * vec1[X];
  plane[D] = -(plane[A] * v0[X] + plane[B] * v0[Y] + plane[C] * v0[Z]);
}

/* Draw a floor (possibly textured). */
static void drawFloor(void) {
  glDisable(GL_LIGHTING);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3fv(floorVertices[0]);
  glTexCoord2f(0.0, 16.0);
  glVertex3fv(floorVertices[1]);
  glTexCoord2f(16.0, 16.0);
  glVertex3fv(floorVertices[2]);
  glTexCoord2f(16.0, 0.0);
  glVertex3fv(floorVertices[3]);
  glEnd();
  glEnable(GL_LIGHTING);
}

static void redraw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // Reposition of the light source.
  lightPosition[1] = lightHeight;
  if (SHOW_MOVING_LIGHT) {
    lightPosition[0] = 12 * cos(lightAngle);
    lightPosition[2] = 12 * sin(lightAngle);
    if (directionalLight)
      lightPosition[3] = 0.0;
    else
      lightPosition[3] = 1.0;
  }

  shadowMatrix(floorShadow, floorPlane, lightPosition);

  glPushMatrix();
  /* Perform scene rotations based on user mouse input. */
  glRotatef(angle2, 1.0, 0.0, 0.0);
  glRotatef(angle, 0.0, 1.0, 0.0);

  /* Tell GL new light source position. */
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

  /* Back face culling will get used to only draw either the top or the
    bottom floor.  This let's us get a floor with two distinct
    appearances.  The top floor surface is reflective and kind of red.
    The bottom floor surface is not reflective and blue. */

  /* Draw "bottom" of floor in blue. */
  glFrontFace(GL_CW); /* Switch face orientation. */
  glColor4f(0.1, 0.1, 0.1, 1.0);
  drawFloor();
  glFrontFace(GL_CCW);

  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS, 3, 0xffffffff);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  /* Draw "top" of floor.  Use blending to blend in reflection. */
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(0.7, 0.0, 0.0, 0.3);
  glColor4f(1.0, 1.0, 1.0, 0.3);
  drawFloor();
  glDisable(GL_BLEND);

  drawModel();

  glStencilFunc(GL_LESS, 2, 0xffffffff);
  glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

  glEnable(GL_POLYGON_OFFSET_FILL);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_LIGHTING);  // Force the 50% black.
  glColor4f(0.0, 0.0, 0.0, 0.5);

  glPushMatrix();
  glMultMatrixf((GLfloat *)floorShadow);  // Draw shadows

  drawModel();

  glPopMatrix();
  glDisable(GL_BLEND);
  glEnable(GL_LIGHTING);
  glDisable(GL_POLYGON_OFFSET_FILL);
  glDisable(GL_STENCIL_TEST);

  /// Determine whether to show
  /// Where the light source comes from.
  if (SHOW_LIGHT_ARROW) {
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 0.0);
    if (directionalLight) {
      /* Draw an arrowhead. */
      glDisable(GL_CULL_FACE);
      glTranslatef(lightPosition[0], lightPosition[1], lightPosition[2]);
      glRotatef(lightAngle * -180.0 / M_PI, 0, 1, 0);
      glRotatef(atan(lightHeight / 12) * 180.0 / M_PI, 0, 0, 1);
      glBegin(GL_TRIANGLE_FAN);
      glVertex3f(0, 0, 0);
      glVertex3f(2, 1, 1);
      glVertex3f(2, -1, 1);
      glVertex3f(2, -1, -1);
      glVertex3f(2, 1, -1);
      glVertex3f(2, 1, 1);
      glEnd();
      // Draw a white line from light direction.
      glColor3f(1.0, 1.0, 1.0);
      glBegin(GL_LINES);
      glVertex3f(0, 0, 0);
      glVertex3f(5, 0, 0);
      glEnd();
      glEnable(GL_CULL_FACE);
    } else {
      // Draw a yellow ball at the light source.
      glTranslatef(lightPosition[0], lightPosition[1], lightPosition[2]);
      glutSolidSphere(1.0, 5, 5);
    }
    glEnable(GL_LIGHTING);
    glPopMatrix();
  }

  glPopMatrix();
  glutSwapBuffers();
}

/* -------------------------------------------------------------------------- */
/*                                  Controls                                  */
/* -------------------------------------------------------------------------- */

// Mouse control.
static void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      moving = 1;
      startx = x;
      starty = y;
    }
    if (state == GLUT_UP) moving = 0;
  }
  if (button == GLUT_MIDDLE_BUTTON) {
    if (state == GLUT_DOWN) {
      lightMoving = 1;
      lightStartX = x;
      lightStartY = y;
    }
    if (state == GLUT_UP) lightMoving = 0;
  }
}

// Camera motion.
static void motion(int x, int y) {
  if (moving) {
    angle = angle + (x - startx);
    angle2 = angle2 + (y - starty);
    startx = x;
    starty = y;
    glutPostRedisplay();
  }
  if (lightMoving) {
    lightAngle += (x - lightStartX) / 40.0;
    lightHeight += (lightStartY - y) / 20.0;
    lightStartX = x;
    lightStartY = y;
    glutPostRedisplay();
  }
}

// Advance time varying state when idle callback registered.
static void idle(void) {
  static float time = 0.0;
  time = glutGet(GLUT_ELAPSED_TIME) / 500.0;
  if (!lightMoving) lightAngle += 0.03;
  glutPostRedisplay();
}

// When not visible, stop animating.  Restart when visible again.
static void visible(int vis) {
  if (vis == GLUT_VISIBLE) {
    if (animation) glutIdleFunc(idle);
  } else {
    if (!animation) glutIdleFunc(NULL);
  }
}

// Press key to redraw if stopped  drawing.
static void key(unsigned char c, int x, int y) {
  if (c == 27) exit(0);  // Escape key
  glutPostRedisplay();
}

// Press key to redraw if stopped  drawing.
static void special(int k, int x, int y) { glutPostRedisplay(); }

/* -------------------------------------------------------------------------- */
/*                                    Main                                    */
/* -------------------------------------------------------------------------- */

int main(int argc, char **argv) {
  Model_parseModel(argc, argv);

  // Init the window.
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL |
                      GLUT_MULTISAMPLE);
  glutCreateWindow(argv[1]);

  // Init the open gl callbacks.
  glutDisplayFunc(redraw);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutVisibilityFunc(visible);
  glutKeyboardFunc(key);
  glutIdleFunc(update);
  glutSpecialFunc(special);

  // Init the modes and enable.
  glPolygonOffset(-2.0, -1.0);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glLineWidth(3.0);
  glMatrixMode(GL_PROJECTION);
  gluPerspective(40.0,    // Field of view in degree
                 1.0,     // Asplect ratio
                 20.0,    // The z near
                 100.0);  // The z far
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(0.0, 8.0, 60.0,  // eye is at (0,0,30)
            0.0, 8.0, 0.0,   // center is at (0,0,0)
            0.0, 1.0, 0.);   // up is in positive Y direction

  // Set the lighting
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  // Setup floor plane for projected shadow calculations.
  findPlane(floorPlane, floorVertices[1], floorVertices[2], floorVertices[3]);

  glutMainLoop();
  return 0;
}
