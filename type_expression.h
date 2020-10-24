#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TYPE_EXPRESSION_H
#define TYPE_EXPRESSION_H

enum varClass {
  PRIMITIVE, 
  RECTANGULAR, 
  JAGGED
};

enum rectStatus{
  STATIC, 
  DYNAMIC, 
  NOT_APPLICABLE
};

enum type{
  INTEGER, 
  REAL, 
  BOOLEAN, 
  JAGGED_ARRAY, 
  RECTANGULAR_ARRAY
};

const char* var_class_map[] = {"primitive", "rectangular", "jagged"};
const char* rect_status_map[] = {"static", "dynamic", "not_applicable"};
const char* type_map[] = {"integer", "real", "Boolean", "jaggedArray", "rectangularArray"};

typedef struct {
  int length;
  int* ranges;
} rangeR2Item;

typedef struct{
  int dimension_count;
  int range_R1[2];
  rangeR2Item* range_R2;
  enum type basic_element_type;
} jaggedArray;

typedef struct{
  int dimension_count;
  int* ranges[2];
  enum type basic_element_type;
} rectArray;

typedef struct{
  union array{
    jaggedArray j;
    rectArray r;
  };
  enum type t;
} typeExpression;
#endif

