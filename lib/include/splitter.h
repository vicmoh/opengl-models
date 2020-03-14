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

typedef struct {
  char** at;
  unsigned int length;
  void* __private__;
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
void free_Splitter(Splitter* self);

/**
 * Get the string at certain position
 * of the splitted strings.
 * @param self object.
 * @param position of the string that has been split.
 * Position starts at zero like an array.
 * @return Allocated string.
 */
char* Splitter_getSplitAt(Splitter* self, int position);

/**
 * Get the multple splits string from the split possition.
 * @param self object.
 * @param startPosition of split included. Index start at zero.
 * @param endPosition of split includeed. Index start at zero.
 */
char* Splitter_getMultipleSplits(Splitter* self, unsigned int startPosition,
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

#endif
