#include "model.h"

Model* __new_Model() {
  Model* this = malloc(sizeof(Model));
  this->faceList = 0;
  this->vertices = 0;
  this->faceList = null;
  this->vertices = null;
  return this;
}

Model* new_Model(String filePath) {
  const bool DEBUG = true;

  // Initialize the data.
  FileReader* fs = new_FileReader(filePath);
  if (fs == null) return null;
  Model* this = __new_Model();

  this->numOfFaces = 0;
  this->numOfVertices = 0;
  bool isEndHeader = false;

  // Get through each file.
  for_in(next, fs) {
    String eachLine = FileReader_getLineAt(fs, next);
    if (eachLine == null) continue;
    if (DEBUG) print("Line[", _(next), "]: ", $(eachLine));
    Splitter* lineSplit = new_Splitter(eachLine, " ");

    // Determine the number of faces and vertex.
    if (strcmp("element", lineSplit->list[0]) == 0) {
      if (strcmp("face", lineSplit->list[1]) == 0)
        this->numOfFaces = atof(lineSplit->list[2]);
      else if (strcmp("vertex", lineSplit->list[1]) == 0)
        this->numOfVertices = atof(lineSplit->list[2]);
      free_Splitter(lineSplit);
      dispose(eachLine);
      continue;
    }

    // Check if the header ended.
    if (strcmp("end_header", eachLine) == 0) {
      isEndHeader = true;
      free_Splitter(lineSplit);
      dispose(eachLine);
      continue;
    }

    // Free.
    free_Splitter(lineSplit);
    dispose(eachLine);
  }

  // Debug the number of vertices and faces.
  if (DEBUG)
    print("vert#: ", _(this->numOfVertices), ", faces#:", _(this->numOfFaces));

  // Free mem.
  free_FileReader(fs);
  return this;
}

void free_Model(Model* this) {
  if (this == null) return;
  free_Array(this->faceList);
  free_Array(this->vertices);
}

void Model_test() {
  Model* test = new_Model("./assets/ant.ply");
  free_Model(test);
}