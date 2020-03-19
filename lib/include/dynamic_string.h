/**********************************************************
 * @author Vicky Mohammad
 * Copyright 2017, Vicky Mohammad, All rights reserved.
 * https://github.com/vicmoh/dynamic-string-api
 **********************************************************/

#ifndef DYNAMIC_STRING_H
#define DYNAMIC_STRING_H

#include "variadic.h"

/* -------------------------------------------------------------------------- */
/*                              Macros Shortcuts                              */
/* -------------------------------------------------------------------------- */

/**
 * Create a new allocated string.
 *
 * Usage example:
 *
 * String hello = $("Hello world!, my name is ", name, ".");
 */
#define $(...) __newString(VARIADIC_ARGS(__VA_ARGS__))

/**
 * Similar to $() tag except.
 * It frees the var and then assigned the string.
 *
 * Useage example:
 *
 * $$(thisVarWillBeFreedAndThen, "concatenated string", "more string");
 */
#define $$(var, ...) \
  free(var);         \
  var = $(__VA_ARGS__)

/**
 * Used for number representation when using the string
 * tag $(). This wrap with indicator in the string tag
 * that it needs to be freed.
 */
#define _(...) __numberForString(__numberToString(VARIADIC_ARGS_NUMBER(__VA_ARGS__)))

/*
 * Print function that acts similar to the
 * $() string tag.
 */
#define print(...) __print($(__VA_ARGS__))

/**
 * Multiple free in multiple paramaters.
 * for example:
 *
 * dispose(freeThis, andThis, andSoOn);
 */
#define dispose(...) __multipleFree(VARIADIC_ARGS(__VA_ARGS__))

/**
 * Loop through range values. The first param is the index
 * as int, the second param is the start index the third param
 * is the end index.
 *
 * For example looping "num" 0 to 10.
 *
 * loop(num, 0, 10) { ... }
 */
#define loop(index, start, end) for (int index = start; index <= end; index++)

/**
 * Replace the normal free so that it checks
 * for NULL before hand.
 */
#define free(val) \
  if (val != NULL) free(val);

/* -------------------------------------------------------------------------- */
/*                          Main String Preprocessor                          */
/* -------------------------------------------------------------------------- */

// Defining some primitive and non-primitive value.
#define String char*
#define var void*
#define null NULL
#define Number double*

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
char* __newString(VARIADIC_PARAM);

/**
 * This function is used for the _() tag.
 * Which wrap the number string to determin
 * if this string needs to be freed.
 * @param val will be call with the __numberToString()
 * function in macro.
 */
char* __numberForString(const char* val);

/**
 * This function is so that you can free
 * multiple data at one call.
 * Call dispose() function instead.
 * @param val and the other variadic parameter
 * are the value to be freed.
 */
void __multipleFree(VARIADIC_PARAM);

/**
 * Convert number to allocated string. Call using
 * numberToString() function or _() instead.
 * @param numOfArgs is the number of arg being passed.
 * @param secondArg is the int of the decimal places.
 * @return allocated string, must be free.
 */
char* __numberToString(VARIADIC_PARAM);
#define numberToString(...) __numberToString(VARIADIC_ARGS(__VA_ARGS__))

/**
 * Check if the string is in number format.
 * @param toBeChecked.
 * @return true if it is in the format.
 */
bool isStringNumberFormat(const char* toBeChecked);

/**
 * Compare two string of both string are equal.
 * @param a of the first string to be compared.
 * @param b of the second string.
 * @return true if string is equal, else return false.
 */
bool isStringEqual(const char* a, const char* b);

/**
 * Convert string to lower case.
 * @param toBeConverted.
 * @return allocated string. Return NULL if failed.
 */
char* stringToLowerCase(const char* toBeConverted);

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
char* substring(const char* toBeSliced, unsigned int startIndexToKeep,
                unsigned int endIndexToKeep);

/**
 * Get the char of a string.
 * @param theString.
 * @param indexOfChar.
 * @return allocated string of the char. Return NULL if failed.
 */
char* getCharAtIndexAsString(const char* theString, int indexOfChar);

#endif
