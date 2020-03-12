#ifndef POINT_H
#define POINT_H

#include "dynamic_string.h"

typedef struct {
  double x, y, z;
  String toString;
} Point;

/**
 * Creates a new point object
 * from a coordinate values.
 * @param x axis
 * @param y axis
 * @param z axis
 * @return allocated memory of point.
 */
Point* new_PointOf(double x, double y, double z);

/**
 * Creates a new point object
 * of 0, 0, 0 origin value.
 * @return allocated memory of point.
 */
Point* new_Point();

/**
 * Destroyer of the point object.
 */
void free_Point(Point* this);

/**
 * Testing function
 */
void test_Point();

#endif
