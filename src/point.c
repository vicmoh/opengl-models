#include "point.h"

Point* new_PointOf(double x, double y, double z) {
  Point* this = malloc(sizeof(Point));
  this->x = x;
  this->y = y;
  this->z = z;
  this->toString =
      $("{\"x\": ", _(x, 4), ", \"y\": ", _(y, 4), ", \"z\": ", _(z, 4), "}");
  return this;
}

Point* new_Point() { return new_PointOf(0, 0, 0); }

void free_Point(Point* this) { dispose(this->toString, this); }

void Point_test() {
  print("Testing the initializer for the point.");
  Point* point = new_PointOf(1, 2, 3);
  print("The to string result is: ", point->toString);
  free_Point(point);
}