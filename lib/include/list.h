/**********************************************************
 * @author Vicky Mohammad
 * Copyright 2017, Vicky Mohammad, All rights reserved.
 * https://github.com/vicmoh/DynamicStringAPI
 **********************************************************/

// guard
#ifndef LIST_H
#define LIST_H

// include libraries
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define for_each(type, var, list)                  \
  ListIterator __iter = List_createIterator(list); \
  type var = NULL;                                 \
  while ((var = List_nextElement(&__iter)) != NULL)

/**********************************************************
 * object struct
 **********************************************************/

typedef struct __ListNode__ {
  void* data;
  struct __ListNode__* previous;
  struct __ListNode__* next;
} ListNode;

typedef struct {
  ListNode* current;
} ListIterator;

typedef struct {
  ListNode* head;
  ListNode* tail;
  int length;
  void (*deleteData)();
} List;

/**********************************************************
 * list function
 **********************************************************/

/**
 * Create a new list.
 * @param destroyer function that frees the list data object.
 * @return new allocated list.
 */
List* new_List(void (*destroyer)());

/**
 * Create a new node.
 * @param data of add to the node
 * @return the node with the data.
 */
ListNode* List_newNode(void* data);

/**
 * Free the list including the data if
 * the [destroyer] parameter is passed.
 * @param list to be freed.
 */
void free_List(List* list);

/**
 * Insert a data to the front of the list.
 * @param this list object.
 * @param toBeAdded data to the front.
 */
void List_insertFront(List* this, void* toBeAdded);

/**
 * Insert a data to the back of the list.
 * @param this list object.
 * @param toBeAdded data to the back.
 */
void List_insertBack(List* this, void* toBeAdded);

/**
 * Insert a data based on the the compare function.
 * @param this list object.
 * @param toBeAdded data based on the compare function.
 * @param compare function callback with two object comparision
 * of the callback return 0 then it is the same else it is not.
 */
void List_insertSorted(List* this, void* toBeAdded,
                       int (*compare)(const void* object1,
                                      const void* object2));

/**
 * Pop the data from back of list and remove
 * the last node.
 * @param this list object.
 * @return data of the list.
 */
void* List_pop(List* this);

/**
 * Get the data from the front of the list.
 * @param this object.
 * @return data from front of the list.
 */
void* List_getFromFront(List* this);

/**
 * Get the data from back of the list.
 * @param this object.
 * @return data from back of the list.
 */
void* List_getFromBack(List* this);

/**
 * Find element of the list.
 * @param this list object.
 * @param compare function callback that compare
 * if the object is the same. Return true if found.
 * @return the data.
 */
void* List_findElement(List* this,
                       bool (*compare)(const void* object1,
                                       const void* object2),
                       const void* search);

/**
 * Get the length of the list.
 * @param this list object.
 * @return length of the list.
 */
int List_getLength(List* this);

/**
 * Return the string of the object.
 * @param toString is the function callback to create
 * string of individual objects.
 * @param return string of list of each object based on
 * the [toString] callback function being passed.
 */
char* List_toString(List* this, char* (*toString)(char*));

/**
 * Create a list iterator that iterate from
 * begining to next element of the list.
 * must call [List_nextElement] function.
 * @param this list object.
 * @return the list iterator.
 */
ListIterator List_createIterator(List* this);

/**
 * Create a list iterator that iterate from
 * end to previous element of the list.
 * must call [List_prevElement] function.
 * @param this list object.
 * @return the list iterator.
 */
ListIterator List_createBackIterator(List* this);

/**
 * Go to the next iteration and get the element.
 * @param iter of the where to start.
 * @return the data of the list.
 */
void* List_nextElement(ListIterator* iter);

/**
 * Go to the previous iteration and get the element.
 * @param iter of the where to start.
 * @return the data of the list.
 */
void* List_prevElement(ListIterator* iter);

#endif
