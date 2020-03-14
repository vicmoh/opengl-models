#ifndef MODEL_H
#define MODEL_H

#include "dynamic_string.h"
#include "array_map.h"
#include "file_reader.h"
#include "point.h"
#include "splitter.h"

typedef struct {
  int numOfVertices, numOfFaces;
  Array* faceList;  // Array of doubles.
  Array* vertices;  // Array of points.
  String toString;
} Model;

/**
 * Create a new empty model.
 */
Model* __new_Model();

/**
 * Create a new model object.
 * @param filePath to be parsed.
 */
Model* new_Model(String filePath);

/**
 * Destroy and free the model.
 */
void free_Model(Model* self);

/**
 * Test the model object.
 */
void Model_test();

#endif
