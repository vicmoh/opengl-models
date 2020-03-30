#include "model.h"

void Model_parseModel(int argc, char** argv) {
  const bool debug = true;
  print("______________________________________________________");
  print("Running script...\n");

  // If no argument print the feedback.
  if (argc == 0 || argv[1] == null || isStringEqual(argv[1], "")) {
    print(
        "\nNO ARGUMENT FOUND! PLEASE SPECIFY ARGUMENT. Please read the README "
        "provided for more information.");
    print("\nScript complete.\n");
    exit(0);
    return;
  }

  // Parse and print if it failed to parse.
  Model_parsedData = new_Model(argv[1]);
  if (Model_parsedData->hasError) {
    print(
        "\nCould not parse the file. Please make sure it is in the correct"
        "format. File path might be incorrect or does not exist.");
    print("\nScript complete.\n");
    print("______________________________________________________");
    Model_free(Model_parsedData);
    exit(0);
    return;
  }
  if (debug) Model_print(Model_parsedData);
}

Model* __new_Model() {
  Model* this = malloc(sizeof(Model));
  this->faceList = 0;
  this->vertices = 0;
  this->faceList = new_Array(Splitter_free);
  this->vertices = new_Array(Point_free);
  this->hasError = false;
  this->minX = null;
  this->maxX = null;
  this->minY = null;
  this->maxY = null;
  this->minZ = null;
  this->maxZ = null;
  this->fileName = $("");
  return this;
}

void __Model_checkBoundary(Model* this, Point* point) {
  /// Init if it hasn't.
  if (this->minX == null || this->maxX == null) {
    this->minX = new_Number(point->x);
    this->maxX = new_Number(point->x);
    return;
  } else if (this->minY == null || this->maxY == null) {
    this->minY = new_Number(point->y);
    this->maxY = new_Number(point->y);
    return;
  } else if (this->minZ == null || this->maxZ == null) {
    this->minZ = new_Number(point->z);
    this->maxZ = new_Number(point->z);
    return;
  }

  // Check if it's is min or max;
  if (*this->minX > point->x) this->minX = new_Number(point->x);
  if (*this->maxX < point->x) this->maxX = new_Number(point->x);
  if (*this->minY > point->y) this->minY = new_Number(point->y);
  if (*this->maxY < point->y) this->maxY = new_Number(point->y);
  if (*this->minZ > point->z) this->minZ = new_Number(point->z);
  if (*this->maxZ < point->z) this->maxZ = new_Number(point->z);
}

Model* new_Model(String filePath) {
  const bool DEBUG = false;

  // Initialize the data.
  FileReader* file = new_FileReader(filePath);
  Model* this = __new_Model();
  $$(this->fileName, filePath);
  if (file == null) {
    this->hasError = true;
    return this;
  }

  // Initialize
  int faceCounter = 0;
  int vertexCounter = 0;
  bool isEndHeader = false;

  // Get through each file.
  for_in(next, file) {
    String eachLine = FileReader_getLineAt(file, next);
    if (eachLine == null) continue;
    if (DEBUG) print("Line[", _(next), "]: ", eachLine);
    Splitter* lineSplit = new_Splitter(eachLine, " ");

    // Determine the number of faces and vertex.
    if (isStringEqual("element", lineSplit->at[0])) {
      if (isStringEqual("face", lineSplit->at[1]))
        this->numOfFaces = atof(lineSplit->at[2]);
      else if (isStringEqual("vertex", lineSplit->at[1]))
        this->numOfVertices = atof(lineSplit->at[2]);
      Splitter_free(lineSplit);
      dispose(eachLine);
      continue;
    }

    // Check if the header ended.
    if (isStringEqual("end_header", eachLine)) {
      isEndHeader = true;
      Splitter_free(lineSplit);
      dispose(eachLine);
      continue;
    }

    // Parse the face and vertex
    if (isEndHeader) {
      if (this->numOfVertices > vertexCounter) {
        vertexCounter++;
        if (DEBUG) print("vertexCounter: ", _(vertexCounter));
        // Add the point.
        Splitter* vertexData = new_Splitter(eachLine, " ");
        Point* curPoint =
            new_PointOf(atof(vertexData->at[0]), atof(vertexData->at[1]),
                        atof(vertexData->at[2]));
        Array_add(this->vertices, curPoint);
        __Model_checkBoundary(this, curPoint);
        // Check the  max width and height.
        Splitter_free(vertexData);
      } else if (this->numOfFaces > faceCounter) {
        faceCounter++;
        if (DEBUG) print("faceCounter: ", _(faceCounter));
        Array_add(this->faceList, new_Splitter(eachLine, " "));
      }
    }

    // Free.
    Splitter_free(lineSplit);
    dispose(eachLine);
  }

  // Debug the number of vertices and faces.
  if (DEBUG)
    print("vert#: ", _(this->numOfVertices), ", faces#:", _(this->numOfFaces));

  // Free mem.
  FileReader_free(file);
  return this;
}

String Model_toString(Model* this) {
  return $("FaceList#: ", _(this->numOfFaces),
           ", vertices#: ", _(this->numOfVertices));
}

void Model_free(Model* this) {
  if (this == null) return;
  Array_free(this->faceList);
  Array_free(this->vertices);
  dispose(this->minX, this->minY, this->minZ, this->minZ, this->maxY,
          this->maxZ, this->fileName);
}

void Model_print(Model* this) {
  Garbage* gcStr = new_Garbage(free);
  print("The ant model parsed: ", Garbage_collect(gcStr, Model_toString(this)));
  print("Printing the result of the all the vertex...");

  // Printing all the points
  for_in(next, this->vertices) print(
      "Vertex[", _(next), "]: ",
      Garbage_collect(gcStr, Point_toString(Array_get(this->vertices, next))));

  // Printing all the facelist
  for_in(next, this->faceList)
      print("Face[", _(next), "]: ",
            Garbage_collect(
                gcStr, Splitter_toString(Array_get(this->faceList, next))));

  // Print boundary
  print("______________________________________________");
  print("minX: ", _(*this->minX, 6));
  print("minY: ", _(*this->minY, 6));
  print("minZ: ", _(*this->minZ, 6));
  print("maxX: ", _(*this->maxX, 6));
  print("maxY: ", _(*this->maxY, 6));
  print("maxZ: ", _(*this->maxZ, 6));
  print("______________________________________________");
  print("File: ", this->fileName);

  // Free garbage mem
  Garbage_sweep(gcStr);
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
  Model_free(test);
}
