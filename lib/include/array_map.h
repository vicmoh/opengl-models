/**********************************************************
 * @author Vicky Mohammad
 * Copyright 2017, Vicky Mohammad, All rights reserved.
 * https://github.com/vicmoh/DynamicStringAPI
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
 * @param this array to be freed.
 */
void free_Array(Array* this);

/**
 * Add a data to array.
 * @param this Array where data is going to be added to.
 * @param toBeAdded data to the arrray.
 */
void Array_add(Array* this, void* toBeAdded);

/**
 * Add multiple data to array.
 * @param this array of the data going to be added to.
 * @param ... are multiple data.
 */
void __Array_addMultipleData(Array* this, ...);

/**
 * Get the data from index position.
 * @param this array object.
 * @param index of the data to get.
 */
void* Array_get(Array* this, int index);

/**
 * Remove the data and the position container. If this
 * is called, the array will be resized and the
 * data will be repositioned.
 * @param this array object.
 * @param index of the position where the data and index
 * container will be removed.
 */
void Array_remove(Array* this, int index);

/**
 * A functional programming inspired from javascript. Reduce to one
 * value from the array.
 * @param this array object.
 * @param callback of with param of [total], [theElement], [optional index],
 * [optional theArray]
 * @param initialValue of the starting value.
 * @return the reduce single value.
 */
void* Array_reducer(Array* this, void* initialValue, void* (*callback)());

/**
 * A functional programming inspired from javascript. Altered each object
 * value in the array.
 * @param this array object.
 * @param callback of with param of [theElement], [optional index], [optional
 * theArray].
 * @return A new allocated array that has been altered.
 */
Array* Array_mapper(Array* this, void(destroyer)(), void* (*callback)());

/**
 * A functional programming inspired from javascript. Filter the objects
 * in the array.
 * @param this array object.
 * @param callback of with param of [theElement], [optional index], [optional
 * theArray].
 * @return A new allocated array that has been filtered.
 */
Array* Array_filter(Array* this, void(destroyer)(), void* (*callback)());

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
 * @param this map object.
 */
void free_Map(Map* this);

/**
 * Add data to map object.
 * @param this map object.
 * @param key of the value to ba added and retrieved.
 * @param toBeAdded of the data to the map.
 */
void Map_add(Map* this, const char* key, void* toBeAdded);

/**
 * Get the map data from the key string value.
 * @param this map object.
 * @param key of the value to be retrieved.
 */
void* Map_get(Map* this, const char* key);

/**
 * Remove the data in the key.
 * This also delete and free the data.
 * @param this map object.
 * @param key of the data that will be delete.
 */
void Map_remove(Map* this, const char* key);

/**
 * Get the index of the map data.
 * This is mostly used to loop through get each data.
 * @param this map object.
 * @param index of the data position.
 */
void* Map_getFromIndex(Map* this, int index);

#endif
