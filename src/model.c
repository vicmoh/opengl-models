#include "model.h"

Model* __new_Model() {
  Model* this = malloc(sizeof(Model));
  this->faceList = 0;
  this->vertices = 0;
  this->faceList = new_Array(free_Splitter);
  this->vertices = new_Array(free_Point);
  return this;
}

Model* new_Model(String filePath) {
  const bool DEBUG = true;

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
    if (DEBUG) print("Line[", _(next), "]: ", $(eachLine));
    Splitter* lineSplit = new_Splitter(eachLine, " ");

    // Determine the number of faces and vertex.
    if (strcmp("element", lineSplit->at[0]) == 0) {
      if (strcmp("face", lineSplit->at[1]) == 0)
        this->numOfFaces = atof(lineSplit->at[2]);
      else if (strcmp("vertex", lineSplit->at[1]) == 0)
        this->numOfVertices = atof(lineSplit->at[2]);
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
        Array_add(this->vertices,
                  new_PointOf(atof(vertexData->at[0]), atof(vertexData->at[1]),
                              atof(vertexData->at[2])));
        free_Splitter(vertexData);
      } else if (this->numOfFaces > faceCounter) {
        faceCounter++;
        if (DEBUG) print("faceCounter: ", _(faceCounter));
        Array_add(this->faceList, new_Splitter(eachLine, " "));
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
  return $("FaceList#: ", _(this->numOfFaces),
           ", vertices#: ", _(this->numOfVertices));
}

void free_Model(Model* this) {
  if (this == null) return;
  free(this->toString);
  free_Array(this->faceList);
  free_Array(this->vertices);
}

void Model_test() {
  print("Testing the parsing of the ant.ply model.");
  Garbage* gcStr = new_Garbage(free);
  Model* test = new_Model("./assets/ant.ply");
  // Testing the model.
  print("The ant model parsed: ", Garbage_collect(gcStr, Model_toString(test)));
  print("Printing the result of the all the vertex...");

  // Printing all the points
  for_in(next, test->vertices) print(
      "Vertex[", _(next), "]: ",
      Garbage_collect(gcStr, Point_toString(Array_get(test->vertices, next))));

  // Printing all the facelist
  for_in(next, test->faceList)
      print("Face[", _(next), "]: ",
            Garbage_collect(
                gcStr, Splitter_toString(Array_get(test->faceList, next))));

  // Determine if the vertices number is the same after parsed.
  if (test->numOfVertices == test->vertices->length)
    print("Vertices number match!");

  // Determine if the face list number is the same after parsed
  if (test->numOfFaces == test->faceList->length) print("Faces number match!");

  Garbage_sweep(gcStr);
  free_Model(test);
}
