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

// The patsed data file of PLY.
Model* _parsedData;
double _rotate = 0;

/* flags used to control the appearance of the image */
int GLSetup_lineDrawing = 1;    // draw polygons as solid or lines
int GLSetup_lighting = 0;       // use diffuse and specular lighting
int GLSetup_smoothShading = 0;  // smooth or flat shading
int GLSetup_textures = 0;
GLuint GLSetup_textureID[1];

// Colors
const GLfloat BLUE[] = {0.0, 0.0, 1.0, 1.0};
const GLfloat RED[] = {1.0, 0.0, 0.0, 1.0};
const GLfloat GREEN[] = {0.0, 1.0, 0.0, 1.0};
const GLfloat WHITE[] = {1.0, 1.0, 1.0, 1.0};

// Points
Point GLSetup_cameraPos = {.x = 0, .y = 0, .z = 0};

/**
 * Draw the face from multiple vertex.
 * @param index of the face.
 */
void drawFace(int index) {
  MEM_TRACK
  Splitter* faceSplit = _parsedData->faceList->at[index];
  if (isStringEqual(faceSplit->at[0], "3")) glBegin(GL_TRIANGLES);
  if (isStringEqual(faceSplit->at[0], "4")) glBegin(GL_QUADS);
  for_in(next, faceSplit) {
    if (next == 0) continue;
    int curPos = atoi(faceSplit->at[next]);
    Point* curVertex = _parsedData->vertices->at[curPos];
    double normalizer =
        15 / (fabs(*_parsedData->maxY) + fabs(*_parsedData->minY));
    double x = curVertex->x * normalizer;
    double y = curVertex->y * normalizer;
    double z = curVertex->z * normalizer;
    glNormal3f(x, y, z);
    glVertex3f(x, y, z);
  }
  glEnd();
  MEM_SWEEP
}

/**
 * Draw Based on parsed data.
 */
void draw() { for_in(next, _parsedData->faceList) drawFace(next); }

/**
 * Update function, this will
 * call in a loop in open gl.
 */
void update() {
  _rotate += 1.0;
  render();
}

//--------------------------------------------
// OpenGL functions
//--------------------------------------------

void runOpenGL(int argc, char** argv) {
  printf("Running with file: %s\n", argv[1]);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(1024, 768);
  glutCreateWindow(argv[0]);
  printf("Running OpenGL Version: %s\n", glGetString(GL_VERSION));

  // Draw
  initLightSource();
  glutReshapeFunc(reshapeWindow);
  glutDisplayFunc(render);
  glutKeyboardFunc(keyboardControl);
  glutIdleFunc(update);
  glutSpecialFunc(specialControl);
  glutMotionFunc(mouseControl);

  // Loop
  glutMainLoop();
}

void reshapeWindow(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 256.0 * 64);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

/*  Initialize material property and light source. */
void initLightSource(void) {
  GLfloat lightAmbient[] = {0.0, 0.0, 0.0, 1.0};
  GLfloat lightDiffuse[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat lightSpecular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat lightFullOff[] = {0.0, 0.0, 0.0, 1.0};
  GLfloat lightFullOn[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat lightPosition[] = {1.0, 1.0, 1.0, 0.0};
  /* if lighting is turned on then use ambient, diffuse and specular
     lights, otherwise use ambient lighting only */
  if (GLSetup_lighting == 1) {
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
  } else {
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightFullOn);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightFullOff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightFullOff);
  }
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
  // Enable the light and depth
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
}

void checkForVectorAndShaderCondition() {
  /* draw surfaces as either smooth or flat shaded */
  if (GLSetup_smoothShading == 1)
    glShadeModel(GL_SMOOTH);
  else
    glShadeModel(GL_FLAT);
  /* draw polygons as either solid or outlines */
  if (GLSetup_lineDrawing == 1)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void checkForTextureCondition(void (*draw)(void)) {
  // Turn texturing on
  if (GLSetup_textures == 1) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, GLSetup_textureID[0]);
    // If textured, then use GLSetup_white as base colour
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, WHITE);
  }
  if (draw != NULL) draw();
  if (GLSetup_textures == 1) glDisable(GL_TEXTURE_2D);
}

void setStartingPos() {
  int x = 0;  //(*_parsedData->minX + *_parsedData->maxX) / 2;
  int y = 0;  // (*_parsedData->minY + *_parsedData->maxY) / 2;
  int z = 0;  // (*_parsedData->minZ + *_parsedData->maxZ) / 2;
  glTranslatef(x + 0 + GLSetup_cameraPos.x, y + 0 + GLSetup_cameraPos.y,
               z + -100.0 + GLSetup_cameraPos.z);
  glRotatef(_rotate, 0.0, 1.0, 0.0);
}

void setMaterial() {
  glMaterialf(GL_FRONT, GL_SHININESS, 30.0);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, GREEN);
  glMaterialfv(GL_FRONT, GL_SPECULAR, GREEN);
}

void render(void) {
  initLightSource();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  setMaterial();
  setStartingPos();

  // Draw
  checkForVectorAndShaderCondition();
  checkForTextureCondition(draw);
  glPopMatrix();
  glFlush();
}

void specialControl(int key, int x, int y) {
  const double CAMERA_MOVEMENT = 25;
  if (key == 'q' || key == 27) {
    exit(0);
  } else if (key == 'w' || key == GLUT_KEY_UP) {
    GLSetup_cameraPos.y += CAMERA_MOVEMENT;
    render();
    printf("w key is pressed, y=%f.\n", GLSetup_cameraPos.y);
  } else if (key == 's' || key == GLUT_KEY_DOWN) {
    GLSetup_cameraPos.y -= CAMERA_MOVEMENT;
    render();
    printf("s key is pressed, y=%f.\n", GLSetup_cameraPos.y);
  } else if (key == 'a' || key == GLUT_KEY_LEFT) {
    GLSetup_cameraPos.x -= CAMERA_MOVEMENT;
    render();
    printf("a key is pressed, x=%f.\n", GLSetup_cameraPos.x);
  } else if (key == 'd' || key == GLUT_KEY_RIGHT) {
    GLSetup_cameraPos.x += CAMERA_MOVEMENT;
    render();
    printf("d key is pressed, x=%f.\n", GLSetup_cameraPos.x);
  }
}

void keyboardControl(unsigned char key, int x, int y) {
  if (key == '1') {  // draw polygons as outlines
    GLSetup_lineDrawing = 1;
    GLSetup_lighting = 0;
    GLSetup_smoothShading = 0;
    GLSetup_textures = 0;
    render();
    printf("1 is clicked.\n");
  } else if (key == '2') {  // draw polygons as filled
    GLSetup_lineDrawing = 0;
    GLSetup_lighting = 0;
    GLSetup_smoothShading = 0;
    GLSetup_textures = 0;
    render();
    printf("2 is clicked.\n");
  } else if (key == '3') {  // diffuse and specular lighting, flat shading
    GLSetup_lineDrawing = 0;
    GLSetup_lighting = 1;
    GLSetup_smoothShading = 0;
    GLSetup_textures = 0;
    render();
    printf("3 is clicked.\n");
  } else if (key == '4') {  // diffuse and specular lighting, smooth shading
    GLSetup_lineDrawing = 0;
    GLSetup_lighting = 1;
    GLSetup_smoothShading = 1;
    GLSetup_textures = 0;
    render();
    printf("4 is clicked.\n");
  } else if (key == '5') {  // texture with  smooth shading
    GLSetup_lineDrawing = 0;
    GLSetup_lighting = 1;
    GLSetup_smoothShading = 1;
    GLSetup_textures = 1;
    render();
    printf("5 is clicked.\n");
  }
}

void mouseControl(int x, int y) {
  const bool SHOW_DEBUG = false;
  const char debug[] = "GLSetup_mouseControl():";
  GLSetup_cameraPos.z = (y * 1) - (100 * 5);
  if (SHOW_DEBUG) printf("%s x value is %d.\n", debug, x);
  if (SHOW_DEBUG) printf("%s y value is %d.\n", debug, y);
  if (SHOW_DEBUG) printf("GLSetup_cameraPos.z: %f.\n", GLSetup_cameraPos.z);
  render();
}

//---------------------------------------------------
// Main function to run the program
//---------------------------------------------------

int main(int argc, char** argv) {
  print("______________________________________________________");
  print("Running script...\n");

  // If no argument print the feedback.
  if (argc == 0 || argv[1] == null || isStringEqual(argv[1], "")) {
    print(
        "\nNO ARGUMENT FOUND! PLEASE SPECIFY ARGUMENT. Please read the README "
        "provided for more information.");
    print("\nScript complete.\n");
    return 0;
  }

  // Parse and print if it failed to parse.
  _parsedData = new_Model(argv[1]);
  if (_parsedData->hasError) {
    print(
        "\nCould not parse the file. Please make sure it is in the correct"
        "format. File path might be incorrect or does not exist.");
    print("\nScript complete.\n");
    print("______________________________________________________");
    Model_free(_parsedData);
    return 0;
  }
  if (DEBUG) Model_print(_parsedData);

  // Initialize OpenGL and GLUT then run.
  runOpenGL(argc, argv);
  Model_free(_parsedData);
  return 0;
}
