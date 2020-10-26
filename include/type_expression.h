#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TYPE_EXPRESSION_H
#define TYPE_EXPRESSION_H

enum VarClass {
  VARCLASS_PRIMITIVE, 
  VARCLASS_RECTANGULAR, 
  VARCLASS_JAGGED
};

enum RectStatus{
  RECTSTATUS_STATIC, 
  RECTSTATUS_DYNAMIC, 
  RECTSTATUS_NOT_APPLICABLE
};

enum Type{
  TYPE_INTEGER, 
  TYPE_REAL, 
  TYPE_BOOLEAN, 
  TYPE_JAGGED_ARRAY, 
  TYPE_RECTANGULAR_ARRAY
};

static char* var_class_map[] = {"primitive", "rectangular", "jagged"};
static char* rect_status_map[] = {"static", "dynamic", "not_applicable"};
static char* type_map[] = {"integer", "real", "Boolean", "jaggedArray", "rectangularArray"};

typedef struct {
  int length;
  int* ranges;
} RangeR2Item;

typedef struct{
  int dimension_count;
  int range_R1[2];
  RangeR2Item* range_R2;
  enum Type basic_element_type;
} JaggedArray;

typedef struct{
  int dimension_count;
  int** ranges; 
  enum Type basic_element_type;
} RectArray;

typedef struct{
  union {
    JaggedArray j;
    RectArray r;
  } array;
  enum Type t;
} TypeExpression;
#endif

