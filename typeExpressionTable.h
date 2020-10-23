#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TYPEEXPRESSIONTABLE_H
#define TYPEEXPRESSIONTABLE_H

typedef enum varClass varClass;
typedef enum rectStatus rectStatus;
typedef enum type type;

enum varClass{PRIMITIVE, RECTANGULAR, JAGGED};
enum rectStatus{STATIC, DYNAMIC, NOT_APPLICABLE};
enum type{INTEGER, REAL, BOOLEAN, JAGGED_ARRAY, RECTANGULAR_ARRAY};

const char* var_class_map[] = {"primitive", "rectangular", "jagged"};
const char* rect_status_map[] = {"static", "dynamic", "not_applicable"};
const char* type_map[] = {"integer", "real", "Boolean", "jaggedArray", "rectangularArray"};

typedef struct rangeR2Item rangeR2Item;
typedef struct jaggedArray jaggedArray;
typedef struct rectArray rectArray;
typedef union typeExpression typeExpression;
typedef struct typeExpressionRecord typeExpressionRecord;

struct rangeR2Item{
  int length;
  int* ranges;
};

struct jaggedArray{
  int dimension_count;
  int range_R1[2];
  rangeR2Item* range_R2;
  type basic_element_type;
};

struct rectArray{
  int dimension_count;
  int* ranges[2];
  type basic_element_type;
};

struct typeExpression{
  union array{
    jaggedArray j;
    rectArray r;
  };
  type t;
};

struct typeExpressionRecord{
  char* field1[21];
  varClass field2;
  rectStatus field3;
  typeExpression field4;
};

typedef struct typeExpressionTable{
  typeExpressionRecord* T;
  size_t size;
}typeExpressionTable;

void printTypeExpressionTable(typeExpressionTable T);

#endif
