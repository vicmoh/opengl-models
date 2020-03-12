/**********************************************************
 * @author Vicky Mohammad
 * Copyright 2017, Vicky Mohammad, All rights reserved.
 * https://github.com/vicmoh/DynamicStringAPI
 **********************************************************/

#ifndef DYNAMIC_STRING_H
#define DYNAMIC_STRING_H

#include <ctype.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------------------------------------------------------------- */
/*                          20 Parameter Preprocessor                         */
/* -------------------------------------------------------------------------- */

// Param preprocessor.
#define __ARGS_SEQUENCE(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, \
                        _13, _14, _15, _16, _17, _18, _19, _20, N, ...)    \
  N
#define __ARGS(...)                                                           \
  __ARGS_SEQUENCE(__VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, \
                  8, 7, 6, 5, 4, 3, 2, 1)

/* -------------------------------------------------------------------------- */
/*                          Main String Preprocessor                          */
/* -------------------------------------------------------------------------- */

// Defining some primitive and non-primitive value.
#define String char*
#define var void*
#define null NULL
#define Number double*

// Helper functionality.
#define $(...) __newString(__VA_ARGS__, NULL)
#define dispose(...) __multipleFree(__VA_ARGS__, NULL)
#define loop(index, start, end) for (int index = start; index <= end; index++)
#define free(val)             \
  if (val != NULL) free(val); \
  val = NULL
#define numberToString(...) \
  __numberToString(__ARGS(__VA_ARGS__), (double)__VA_ARGS__)
#define _(...) __numberToString(__ARGS(__VA_ARGS__), (double)__VA_ARGS__)
#define print(...) __print($(__VA_ARGS__))

// Credit for lambda https://blog.noctua-software.com/c-lambda.html
#define LAMBDA(varfunction) ({ varfunction function; })

// ASCII range values.
#define GET_ARRAY_SIZE(array) (sizeof(array) / sizeof(*array))
#define IS_UPPERCASE_RANGE(var) (var >= 65 && var <= 90)
#define IS_LOWERCASE_RANGE(var) (var >= 97 && var <= 122)
#define IS_ALPHABET_RANGE(var) \
  ((var >= 97 && var <= 122) || (var >= 65 && var <= 90))
#define IS_NUMBER_RANGE(var) (var >= 48 && var <= 57)
#define IS_VISIBLE_RANGE(var) (var >= 33 && var <= 126)

/* -------------------------------------------------------------------------- */
/*                             Class Preprocessor                             */
/* -------------------------------------------------------------------------- */

// Class preprocessor
#define OBJECT void* _this
#define THIS(object) object* this = _this
#define CONSTRUCTOR(object, param, code) \
  object* new_##object param {           \
    object* this = new (object);         \
    code return this;                    \
  }
#define CLASS(object, instance, constructor, function) \
  typedef struct {                                     \
    instance;                                          \
  } object;                                            \
  function constructor

/* -------------------------------------------------------------------------- */
/*                              Number Functions                              */
/* -------------------------------------------------------------------------- */

/**
 * Allocated a new number.
 * @param value of the number.
 * @return the allocated number.
 */
double* new_Number(double value);

/* -------------------------------------------------------------------------- */
/*                              String Functions                              */
/* -------------------------------------------------------------------------- */

/**
 * String function for calling print
 * similar to the $() tag but to the console.
 * Call wrapper function print() instead.
 * @param val is the string.
 */
void __print(char* val);

/**
 * This function is used to create new string.
 * Call $() function instead.
 * @param val and the other variadic parameter
 * are string to be concatenated.
 * @return a new allocated string.
 */
char* __newString(char* val, ...);

/**
 * This function is so that you can free
 * multiple data at one call.
 * Call dispose() function instead.
 * @param val and the other variadic parameter
 * are the value to be freed.
 */
void __multipleFree(void* val, ...);

/**
 * Convert number to allocated string. Call using
 * numberToString() function or _() instead.
 * @param numOfArgs is the number of arg being passed.
 * @param secondArg is the int of the decimal places.
 * @return allocated string, must be free.
 */
char* __numberToString(int numOfArgs, ...);

/**
 * Check if the string is in number format.
 * @param toBeChecked.
 * @return true if it is in the format.
 */
bool isStringNumberFormat(const char* toBeChecked);

/**
 * Convert string to lower case.
 * @param toBeConverted.
 * @return allocated string. Return NULL if failed.
 */
char* stringtoLowerCase(const char* toBeConverted);

/**
 * Convert string to upper case.
 * @param toBeConverted.
 * @return allocated string. Return NULL if failed.
 */
char* stringToUpperCase(const char* toBeConverted);

/**
 * Trim the white space of the string.
 * @param toBeTrimmed.
 * @return allocated string. Return NULL if failed.
 */
char* trimString(const char* toBeTrimmed);

/**
 * Slice the string to get certain substring.
 * @param toBeSliced.
 * @param startingIndex.
 * @param endIndexToKeep.
 * @return allocated string. Return NULL if failed.
 */
char* sliceString(const char* toBeSliced, unsigned int startIndexToKeep,
                  unsigned int endIndexToKeep);

/**
 * Get the char of a string.
 * @param theString.
 * @param indexOfChar.
 * @return allocated string of the char. Return NULL if failed.
 */
char* getCharAtIndexAsString(const char* theString, int indexOfChar);

#endif
