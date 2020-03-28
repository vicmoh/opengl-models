#ifndef _GL_SETUP_
#define _GL_SETUP_

#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <stdio.h>
#include <stdlib.h>

void runOpenGL(int argc, char** argv);
void reshapeWindow(int w, int h);
void loadTexture(char* filePath);
void initLightSource(void);
void checkForVectorAndShaderCondition();
void checkForTextureCondition(void (*draw)(void));
void setStartingPos();
void setMaterial();
void render();
// Controls
void specialControl(int key, int x, int y);
void keyboardControl(unsigned char key, int x, int y);
void mouseControl(int x, int y);
// Draw l-system
void displayLSystem();
void drawLSystem();
void drawLSystemFromCondition(char* start, double angle);

#endif
