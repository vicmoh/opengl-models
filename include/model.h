#ifndef MODEL_H
#define MODEL_H

#include "array_map.h"
#include "dynamic_string.h"
#include "file_reader.h"
#include "point.h"
#include "splitter.h"

typedef struct {
  int numOfVertices, numOfFaces;
  double *maxX, *maxY, *maxZ;
  double *minX, *minY, *minZ;
  Array* faceList;  // Array of  splits.
  Array* vertices;  // Array of points.
  bool hasError;
} Model;

/**
 * The data that has been a parsed
 */
Model* Model_parsedData;

/**
 * Parse the model data.
 */
void Model_parseModel(int argc, char** argv);

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
 * @param self of the model object.
 */
void Model_free(Model* self);

/**
 * Test the model object.
 */
void Model_test();

/**
 * Print the model to console.
 * @param self of the model object.
 */
void Model_print(Model* self);

#endif
