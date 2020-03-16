#ifndef POINT_H
#define POINT_H

#include "dynamic_string.h"

typedef struct {
  double x, y, z;
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
void free_Point(Point* self);

/**
 * To string of the point object.
 * @param self of the point object
 * @return of the point object string.
 */
String Point_toString(Point* self);

/**
 * Testing function
 */
void Point_test();

#endif
