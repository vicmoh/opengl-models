#include "model.h"

Model* __new_Model() {
  Model* this = malloc(sizeof(Model));
  this->faceList = 0;
  this->vertices = 0;
  this->faceList = new_Array(free);
  this->vertices = new_Array(free_Point);
  this->toString = $("");
  return this;
}

Model* new_Model(String filePath) {
  const bool DEBUG = false;

  // Initialize the data.
  FileReader* fs = new_FileReader(filePath);
  if (fs == null) return null;
  Model* this = __new_Model();

  // Initialize
  int faceCounter = 0;
  int vertexCounter = 0;
  bool isEndHeader = false;

  // Get through each file.
  for_in(next, fs) {
    String eachLine = FileReader_getLineAt(fs, next);
    if (eachLine == null) continue;
    if (DEBUG) print("Line[", _(next), "]: ", eachLine);
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

    // Parse the face and vertex
    if (isEndHeader) {
      if (this->numOfVertices > vertexCounter) {
        vertexCounter++;
        if (DEBUG) print("vertexCounter: ", _(vertexCounter));
        Splitter* vertexData = new_Splitter(eachLine, " ");
        Array_add(this->vertices, new_PointOf(atof(vertexData->list[0]),
                                              atof(vertexData->list[1]),
                                              atof(vertexData->list[2])));
        free_Splitter(vertexData);
      } else if (this->numOfFaces > faceCounter) {
        faceCounter++;
        if (DEBUG) print("faceCounter: ", _(faceCounter));
        Array_add(this->faceList, $(eachLine));
      }
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

String Model_toString(Model* this) {
  free(this->toString);
  this->toString = $("FaceList#: ", _(this->numOfFaces),
                     ", vertices#: ", _(this->numOfVertices));
  return this->toString;
}

void free_Model(Model* this) {
  if (this == null) return;
  free(this->toString);
  free_Array(this->faceList);
  free_Array(this->vertices);
}

void Model_test() {
  print("Testing the parsing of the ant.ply model.");
  Model* test = new_Model("./assets/ant.ply");
  print("The ant model parsed: ", Model_toString(test));
  free_Model(test);
}