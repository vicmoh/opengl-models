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
#include "model.h"
#include "point.h"

// Show print if debug is true.
#define DEBUG true
#define SHOW_TEST false

// Window size.
#define WINDOW_SIZE 512

Model* _parsedData;

/**
 * OpenGL calls this to draw the screen.
 */
void display() {
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glPointSize(1.0);

  /* draw individual pixels */
  glBegin(GL_POINTS);
  glEnd();
  glFlush();
}

/**
 * Read the keyboard, when q is pressed, exit the program.
 * @param key of the keyboard key that is pressed.
 * @param x of cursor
 * @param y of cursor
 */
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 27:
    case 'q':
      exit(0);
      break;
  }
}

/**
 * Testing function.
 */
void test() {
  print("Running script...");
  print("__________Testign__________");
  Point_test();
  Model_test();
  print("___________________________");
  print("Script complete.");
}

/* -------------------------------------------------------------------------- */
/*                      Main Function to run the program                      */
/* -------------------------------------------------------------------------- */

int main(int argc, char** argv) {
  if (SHOW_TEST) test();
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

  // Initialize OpenGL and GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
  glutCreateWindow("2D Test");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
