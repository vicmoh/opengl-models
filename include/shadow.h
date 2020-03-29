#ifndef SHADOW_H
#define SHADOW_H

// MacOS OpenGL Headers
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include "array_map.h"
#include "dynamic_string.h"
#include "splitter.h"

GLfloat Shadow_floorPlane[4];
GLfloat Shadow_floorShadow[4][4];

GLfloat Shadow_floorVertices[4][3] = {
    {-20.0, 0.0, 20.0},
    {20.0, 0.0, 20.0},
    {20.0, 0.0, -20.0},
    {-20.0, 0.0, -20.0},
};

enum Shadow_pointAttributes { X, Y, Z, W };
enum Shadow_planeAttributes { A, B, C, D };

void Shadow_redraw(GLfloat lightPosition[4], void (*drawModel)());

void Shadow_drawFloor();

void Shadow_shadowMatrix(GLfloat shadowMat[4][4], GLfloat groundplane[4],
                         GLfloat lightpos[4]);

void Shadow_findPlane(GLfloat plane[4], GLfloat v0[3], GLfloat v1[3],
                      GLfloat v2[3]);

#endif