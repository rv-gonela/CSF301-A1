#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type_expression.h"

#ifndef TYPE_EXPRESSIONTABLE_H
#define TYPE_EXPRESSIONTABLE_H

typedef struct{
  char* var_name;
  enum VarClass arrayType;
  enum RectStatus rectType;
  TypeExpression type_expression;
} TypeExpressionRecord;

typedef struct {
  TypeExpressionRecord* T;
  size_t size;
  size_t capacity;
} TypeExpressionTable;

void printTypeExpressionTable(TypeExpressionTable Te);

TypeExpressionRecord getTypeExpressionRecord(TypeExpressionTable* E, char* var_name);

#endif
