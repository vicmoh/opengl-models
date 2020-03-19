/**********************************************************
 * @author Vicky Mohammad
 * Copyright 2017, Vicky Mohammad, All rights reserved.
 * https://github.com/vicmoh/dynamic-string-api
 **********************************************************/

#ifndef CLASS_H
#define CLASS_H

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

#endif
