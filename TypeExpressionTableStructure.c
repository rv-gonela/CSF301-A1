enum types{primitive =0, rectangular =1, jagged =2};
enum rectStatus{stat, dyn, not_applicable};
enum type{integer, real, Boolean, jaggedArray, rectangularArray};

typedef struct range_R2{
  int length;
  int* ranges;
}

typedef struct jaggedArray{
  type t;
  int dimensions;
  int range_R1[2];
  range_R2 range_R2*;
  type basicElementType;
}jaggedArray;

typedef struct rectArray{
  type t;
  int dimensions;
  int* ranges[2];
  type basicElementType;
}rectArray;

typedef union typeExpression{
  jaggedArray j;
  rectArray r;
  type t;
}typeExpression;

typedef struct typeExpressionRecord{
  char* Field1[20];
  types Field2;
  rectStatus Field3;
  typeExpression Field4;
}typeExpressionRecord;

typedef struct typeExpressionTable{
  typeExpressionRecord* T;
}typeExpressionTable;
