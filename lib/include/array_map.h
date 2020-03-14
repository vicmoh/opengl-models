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
 * data will be repositioned.
 * @param self array object.
 * @param index of the position where the data and index
 * container will be removed.
 */
void Array_remove(Array* self, int index);

/**
 * A functional programming inspired from javascript. Reduce to one
 * value from the array.
 * @param self array object.
 * @param callback of with param of [total], [theElement], [optional index],
 * [optional theArray]
 * @param initialValue of the starting value.
 * @return the reduce single value.
 */
void* Array_reducer(Array* self, void* initialValue, void* (*callback)());

/**
 * A functional programming inspired from javascript. Altered each object
 * value in the array.
 * @param self array object.
 * @param callback of with param of [theElement], [optional index], [optional
 * theArray].
 * @return A new allocated array that has been altered.
 */
Array* Array_mapper(Array* self, void(destroyer)(), void* (*callback)());

/**
 * A functional programming inspired from javascript. Filter the objects
 * in the array.
 * @param self array object.
 * @param callback of with param of [theElement], [optional index], [optional
 * theArray].
 * @return A new allocated array that has been filtered.
 */
Array* Array_filter(Array* self, void(destroyer)(), void* (*callback)());

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
 * Add data to map object.
 * @param self map object.
 * @param key of the value to ba added and retrieved.
 * @param toBeAdded of the data to the map.
 */
void Map_add(Map* self, const char* key, void* toBeAdded);

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
 * Get the index of the map data.
 * This is mostly used to loop through get each data.
 * @param self map object.
 * @param index of the data position.
 */
void* Map_getFromIndex(Map* self, int index);

#endif
