/**********************************************************
 * @author Vicky Mohammad
 * Copyright 2017, Vicky Mohammad, All rights reserved.
 * https://github.com/vicmoh/dynamic-string-api
 **********************************************************/

#ifndef SPLITTER_H
#define SPLITTER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array_map.h"
#include "dynamic_string.h"

typedef struct {
  char** at;
  unsigned int length;
} Splitter;

/**
 * Create new object to split a string.
 * @param toBeSplit of the string.
 * @param delimiter of the split.
 * @return the allocated object.
 */
Splitter* new_Splitter(char* toBeSplit, const char* delimiter);

/**
 * Free the object.
 * @param self object including the attribute.
 */
void Splitter_free(Splitter* self);

/**
 * Get the string at certain position
 * of the splitted strings.
 * @param self object.
 * @param position of the string that has been split.
 * Position starts at zero like an array.
 * @return Allocated string.
 */
char* Splitter_at(Splitter* self, int position);

/**
 * Get the multple splits string from the split position.
 * @param self object.
 * @param startPosition of split included. Index start at zero.
 * @param endPosition of split includeed. Index start at zero.
 */
char* Splitter_getMultiSplits(Splitter* self, unsigned int startPosition,
                              unsigned int endPosition);

/**
 * Search the splitted string by index or count.
 * @param self object.
 * @param search string that has been split.
 * @param trueIfIndexSearch_falseIfCounting type of search.
 * @return index of the search of the count found.
 */
int Splitter_search(Splitter* self, const char* search,
                    bool trueIfIndexSearch_falseIfCounting);

/**
 * To string the object of the splitter object.
 * @param self the splitter object.
 * @return the string of the splitter object.
 */
char* Splitter_toString(Splitter* self);

#endif
