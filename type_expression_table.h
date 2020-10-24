#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type_expression.h"

#ifndef TYPEEXPRESSIONTABLE_H
#define TYPEEXPRESSIONTABLE_H

typedef struct{
  char* field1[21];
  enum varClass field2;
  enum rectStatus field3;
  typeExpression field4;
} typeExpressionRecord;

typedef struct {
  typeExpressionRecord* T;
  size_t size;
} typeExpressionTable;

void printTypeExpressionTable(typeExpressionTable T);

#endif
