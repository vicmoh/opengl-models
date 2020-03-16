#include "point.h"

Point* new_PointOf(double x, double y, double z) {
  Point* this = malloc(sizeof(Point));
  this->x = x;
  this->y = y;
  this->z = z;
  return this;
}

Point* new_Point() { return new_PointOf(0, 0, 0); }

void free_Point(Point* this) { dispose(this); }

String Point_toString(Point* this) {
  return $("{\"x\": ", _(this->x, 4), ", \"y\": ", _(this->y, 4),
           ", \"z\": ", _(this->z, 4), "}");
}

void Point_test() {
  print("Testing the initializer for the point.");
  Point* point = new_PointOf(1, 2, 3);
  free_Point(point);
}