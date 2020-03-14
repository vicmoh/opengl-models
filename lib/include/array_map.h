/**********************************************************
 * @author Vicky Mohammad
 * Copyright 2017, Vicky Mohammad, All rights reserved.
 * https://github.com/vicmoh/dynamic-string-api
 **********************************************************/

#ifndef ARRAY_MAP_H
#define ARRAY_MAP_H

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Param preprocessor
#define for_in(x, object) for (int x = 0; x < (object->length); x++)
#define Array_addMultiple(...) __Array_addMultipleData(__VA_ARGS__, NULL)

/* -------------------------------------------------------------------------- */
/*                  Struct to hold the data for array and map                 */
/* -------------------------------------------------------------------------- */

typedef struct {
  void* data;
  char key[1024];
} ArrayMapData;

/* -------------------------------------------------------------------------- */
/*                                 Array class                                */
/* -------------------------------------------------------------------------- */

typedef struct {
  unsigned int length;
  ArrayMapData* index;
  void (*destroyer)();
} Array;

/**
 * Constructor to create new array object.
 * @param destroyer of the data to be deleted when
 * free_Array() is called. Can be null.
 * @return a new allocated Array Object.
 */
Array* new_Array(void (*destroyer)());

/**
 * Function to delete the array and the objects if
 * the destroyer for the @new_Array is passed.
 * @param self array to be freed.
 */
void free_Array(Array* self);

/**
 * Add a data to array.
 * @param self Array where data is going to be added to.
 * @param toBeAdded data to the arrray.
 */
void Array_add(Array* self, void* toBeAdded);

/**
 * Add multiple data to array.
 * @param self array of the data going to be added to.
 * @param ... are multiple data.
 */
void __Array_addMultipleData(Array* self, ...);

/**
 * Get the data from index position.
 * @param self array object.
 * @param index of the data to get.
 */
void* Array_get(Array* self, int index);

/**
 * Remove the data and the position container. If this
 * is called, the array will be resized and the
 * data will be repositioned. Hence, there will be 1 less data.
 * @param self array object.
 * @param index of the position where the index
 * container will be removed.
 * @return the data at the index.
 */
void* Array_pop(Array* self, int index);

/**
 * Get the array length.
 * @param self array object.
 * @return the number of data in the array.
 */
int Array_getLength(Array* self);


/* -------------------------------------------------------------------------- */
/*                                  Map class                                 */
/* -------------------------------------------------------------------------- */

typedef struct {
  unsigned int tableSize;
  ArrayMapData** table;
  void (*destroyer)();
  Array* array;
  unsigned int length;
} Map;

/**
 * Create a new allocated hash map object.
 * @param destroyer of the data to be deleted when
 * free_Map() is called. Can be null.
 * @return Allocated hash map object.
 */
Map* new_Map(void (*destroyer)());

/**
 * Free the map and data in the map if destroyer is set.
 * @param self map object.
 */
void free_Map(Map* self);

/**
 * Add data to map object. If data already exist, then
 * it will free and put the data.
 * @param self map object.
 * @param key of where the data will be put.
 * @param toBeAdded of the data that will be put.
 */
void Map_put(Map* self, const char* key, void* toBeAdded);

/**
 * Replace data to the map object. If the data already 
 * exist, it will replace the data. However, it will not 
 * free that data. Instead, it will return it.
 * @param self the map object.
 * @param key of where the data will be placed.
 * @param toBeAdded of the data that will be placed.
 * @return the data if exist. If it does not exist, 
 * it will return null.
 */
void* Map_replace(Map* self, const char* key, void* toBeAdded);

/**
 * Get the map data from the key string value.
 * @param self map object.
 * @param key of the value to be retrieved.
 */
void* Map_get(Map* self, const char* key);

/**
 * Remove the data in the key.
 * This also delete and free the data.
 * @param self map object.
 * @param key of the data that will be delete.
 */
void Map_remove(Map* self, const char* key);

/**
 * Get the data from the index table of the map object.
 * This is mostly used to loop through and get each data.
 * @param self map object.
 * @param index of the data position of the table.
 */
void* Map_getAt(Map* self, int index);

#endif
