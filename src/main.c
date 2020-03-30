/**
 * @author: Vicky Mohammad
 * @file This pogram parse the PLY model and creates the model.
 * using the model.
 * Reference:
 * https://www.opengl.org/archives/resources/code/samples/mjktips/TexShadowReflectLight.html
 */

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
#include "model.h"
#include "point.h"

// Show print if debug is true.
#define DEBUG true
#define SHOW_LIGHT_ARROW false
#define SHOW_MOVING_LIGHT false
#define SHOW_FLOOR false

// The patsed data file of PLY.
static double _rotate = 0;

/* flags used to control the appearance of the image */
static int _lineDrawing = 1;    // draw polygons as solid or lines
static int _lighting = 0;       // use diffuse and specular lighting
static int _smoothShading = 0;  // smooth or flat shading
static int _textures = 0;
static GLuint _textureID[1];

// Colors
const GLfloat _BLUE[] = {0.0, 0.0, 1.0, 1.0};
const GLfloat _RED[] = {1.0, 0.0, 0.0, 1.0};
const GLfloat _GREEN[] = {0.0, 1.0, 0.0, 1.0};
const GLfloat _WHITE[] = {1.0, 1.0, 1.0, 1.0};

// Attribute index for the array.
enum PositionAttribute { X, Y, Z, W };

// Variable controlling various rendering modes.
static int _directionalLight = 1;
static GLfloat _floorPlane[4];
static GLfloat _floorShadow[4][4];

// Control for the camera angle and lighting.
static float _lightAngle = 0.0, _lightHeight = 20;
GLfloat _angle = -150;   // in degrees
GLfloat _angleTwo = 30;  // in degrees

// Motion and lights
Point _motion = {.x = 0, .y = 0, .z = 0};
int _moving = 0;
int _lightMoving = 0, _lightStartX = 0, _lightStartY = 0;

static GLfloat _lightPosition[4];
static GLfloat _lightColor[] = {1, 0.3, 0.3, 1.0};  // Red-tinted light.

static GLfloat _floorVertices[4][3] = {
    {-20.0, 0.0, 20.0},
    {20.0, 0.0, 20.0},
    {20.0, 0.0, -20.0},
    {-20.0, 0.0, -20.0},
};

/* -------------------------------------------------------------------------- */
/*                            Draw model functions                            */
/* -------------------------------------------------------------------------- */

/**
 * Draw the face from multiple vertex.
 * @param index of the face.
 */
static void drawFace(int index) {
  MEM_TRACK
  Splitter *faceSplit = Model_parsedData->faceList->at[index];
  if (isStringEqual(faceSplit->at[0], "3")) glBegin(GL_TRIANGLES);
  if (isStringEqual(faceSplit->at[0], "4")) glBegin(GL_QUADS);
  glColor3f(0.3, 0.3, 0.3);  // Shadow color
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
    double normalizer = (5 / avg);
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
  _rotate -= 3.0;
  redraw();
}

/* -------------------------------------------------------------------------- */
/*                              Open GL Functions                             */
/* -------------------------------------------------------------------------- */

static void printLightPosition() {
  print("x: ", _(_lightPosition[X]));
  print("y: ", _(_lightPosition[Y]));
  print("z: ", _(_lightPosition[Z]));
  print("light: ", _(_lightPosition[W]));
}

/* Create a matrix that will project the desired shadow. */
static void calcShadowMatrix(GLfloat shadowMat[4][4], GLfloat groundplane[4],
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
static void findPlane(GLfloat plane[4], GLfloat v0[3], GLfloat v1[3],
                      GLfloat v2[3]) {
  GLfloat vec0[3], vec1[3];
  /* Need 2 vectors to find cross product. */
  vec0[X] = v1[X] - v0[X];
  vec0[Y] = v1[Y] - v0[Y];
  vec0[Z] = v1[Z] - v0[Z];

  vec1[X] = v2[X] - v0[X];
  vec1[Y] = v2[Y] - v0[Y];
  vec1[Z] = v2[Z] - v0[Z];
  /* find cross product to get A, B, and C of plane equation */
  plane[0] = vec0[Y] * vec1[Z] - vec0[Z] * vec1[Y];
  plane[1] = -(vec0[X] * vec1[Z] - vec0[Z] * vec1[X]);
  plane[2] = vec0[X] * vec1[Y] - vec0[Y] * vec1[X];
  plane[3] = -(plane[0] * v0[X] + plane[1] * v0[Y] + plane[2] * v0[Z]);
}

// Draw a floor
static void drawFloor(void) {
  glDisable(GL_LIGHTING);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3fv(_floorVertices[X]);
  glTexCoord2f(0.0, 16.0);
  glVertex3fv(_floorVertices[Y]);
  glTexCoord2f(16.0, 16.0);
  glVertex3fv(_floorVertices[Z]);
  glTexCoord2f(16.0, 0.0);
  glVertex3fv(_floorVertices[W]);
  glEnd();
  glEnable(GL_LIGHTING);
}

static void redraw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glClearColor(1, 1, 1, 1);

  // Reposition of the light source.
  _lightPosition[Y] = _lightHeight;
  if (SHOW_MOVING_LIGHT) {
    _lightPosition[X] = 12 * cos(_lightAngle);
    _lightPosition[Z] = 12 * sin(_lightAngle);
    if (_directionalLight)
      _lightPosition[W] = 0.0;
    else
      _lightPosition[W] = 1.0;
  }

  calcShadowMatrix(_floorShadow, _floorPlane, _lightPosition);

  glPushMatrix();
  // Perform scene rotations based on user mouse input.
  glRotatef(_angleTwo, 1.0, 0.0, 0.0);
  glRotatef(_angle, 0.0, 1.0, 0.0);

  // New light source position
  glLightfv(GL_LIGHT0, GL_POSITION, _lightPosition);

  /* Back face culling will get used to only draw either the top or the
    bottom floor.  This let's us get a floor with two distinct
    appearances.  The top floor surface is reflective and kind of red.
    The bottom floor surface is not reflective and blue. */

  // Bottom floor color.
  if (SHOW_FLOOR) {
    glFrontFace(GL_CW); /* Switch face orientation. */
    glColor4f(0.1, 0.1, 0.1, 1.0);
    drawFloor();
    glFrontFace(GL_CCW);

    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 3, 0xffffffff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  }

  // Draw top floor
  if (SHOW_FLOOR) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0, 1.0, 1.0, 0.3);
    drawFloor();
    glDisable(GL_BLEND);
  }

  glRotatef(_rotate, 0, 1, 0);
  drawModel();

  glStencilFunc(GL_LESS, 2, 0xffffffff);
  glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

  glEnable(GL_POLYGON_OFFSET_FILL);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_LIGHTING);  // Force the 50% black.
  glColor4f(0.0, 0.0, 0.0, 0.5);

  glPushMatrix();
  glMultMatrixf((GLfloat *)_floorShadow);  // Draw shadows

  drawModel();

  glPopMatrix();

  /// Setting
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
    if (_directionalLight) {
      /* Draw an arrowhead. */
      glDisable(GL_CULL_FACE);
      glTranslatef(_lightPosition[0], _lightPosition[1], _lightPosition[2]);
      glRotatef(_lightAngle * -180.0 / M_PI, 0, 1, 0);
      glRotatef(atan(_lightHeight / 12) * 180.0 / M_PI, 0, 0, 1);
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
      glTranslatef(_lightPosition[0], _lightPosition[1], _lightPosition[2]);
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
static void mouseControl(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      _moving = 1;
      _motion.x = x;
      _motion.y = y;
    }
    if (state == GLUT_UP) _moving = 0;
  }
  if (SHOW_MOVING_LIGHT)
    if (button == GLUT_MIDDLE_BUTTON) {
      if (state == GLUT_DOWN) {
        _lightMoving = 1;
        _lightStartX = x;
        _lightStartY = y;
      }
      if (state == GLUT_UP) _lightMoving = 0;
    }
}

// Special control for sizing the model.
void specialControl(int key, int x, int y) {
  const double CAMERA_MOVEMENT = 25;
  if (key == 'q' || key == 27) {
    exit(0);
  } else if (key == 'w' || key == GLUT_KEY_UP) {
    _motion.z += CAMERA_MOVEMENT;
    redraw();
    printf("w key is pressed, y=%f.\n", _motion.z);
  } else if (key == 's' || key == GLUT_KEY_DOWN) {
    _motion.z -= CAMERA_MOVEMENT;
    redraw();
    printf("s key is pressed, y=%f.\n", _motion.z);
  }
}

// Camera motion.
static void motion(int x, int y) {
  if (_moving) {
    _angle = _angle + (x - _motion.x);
    _angleTwo = _angleTwo + (y - _motion.y);
    _motion.x = x;
    _motion.y = y;
    glutPostRedisplay();
  }
  if (SHOW_MOVING_LIGHT)
    if (_lightMoving) {
      _lightAngle += (x - _lightStartX) / 40.0;
      _lightHeight += (_lightStartY - y) / 20.0;
      _lightStartX = x;
      _lightStartY = y;
      glutPostRedisplay();
    }
}

// Press key to redraw if stopped  drawing.
static void key(unsigned char c, int x, int y) {
  if (c == 27) exit(0);  // Escape key
  glutPostRedisplay();
}

/* ------------------------------------------------------------------------- */
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
  glutMouseFunc(mouseControl);
  glutMotionFunc(motion);
  glutKeyboardFunc(key);
  glutIdleFunc(update);
  glutSpecialFunc(specialControl);

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
  glLightfv(GL_LIGHT0, GL_DIFFUSE, _lightColor);
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  // Setup floor plane for projected shadow calculations.
  findPlane(_floorPlane, _floorVertices[1], _floorVertices[2],
            _floorVertices[3]);

  glutMainLoop();
  return 0;
}
