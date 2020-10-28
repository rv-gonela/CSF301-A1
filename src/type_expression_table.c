#include "type_expression_table.h"

void printTypeExpressionTable(TypeExpressionTable Te){
  TypeExpressionRecord* T=Te.T;
  for(int i=0;i<Te.size;i++)
  {
    printf("%-22s", T[i].var_name);
    printf("%-15s", var_class_map[T[i].arrayType]);
    printf("%-18s", rect_status_map[T[i].rectType]);
    printf("<type = ");
    printf("%s", type_map[T[i].type_expression.t]);
    if(T[i].arrayType!=0)
    {
      if(T[i].arrayType==1)
      {
        RectArray r=T[i].type_expression.array.r;
        printf(", dimensions=%d,", r.dimension_count);
        for(int dim=1; dim<=r.dimension_count; dim++)
        {
          printf(" range_R%d=(%d,%d),", dim, r.lows[dim-1], r.highs[dim-1]);
        }
      }
      else
      {
        JaggedArray j=T[i].type_expression.array.j;
        printf(", dimensions=%d, ", j.dimension_count);
        printf("range_R1=(%d,%d), ", j.range_R1[0], j.range_R1[1]);
        int count=j.range_R1[1] - j.range_R1[0] + 1;
        printf("range_R2=(");
        if(j.dimension_count==2)
        {
          for(int dim = 0; dim < count-1; dim++)
          {
            printf("%d,", j.range_R2[dim].length);
          }
          printf("%d),", j.range_R2[count-1].length);
        }
        else
        {
          for(int dim = 0; dim < count-1; dim++)
          {
            printf("%d[", j.range_R2[dim].length);
            for(int dim2=0; dim2 < j.range_R2[dim].length-1; dim2++)
            {
              printf("%d,", j.range_R2[dim].ranges[dim2]);
            }
            printf("%d], ", j.range_R2[dim].ranges[j.range_R2[dim].length-1]);
          }
          printf("%d[", j.range_R2[count-1].length);
          for(int dim2=0; dim2 < j.range_R2[count-1].length-1; dim2++)
          {
            printf("%d,", j.range_R2[count-1].ranges[dim2]);
          }
          printf("%d]),", j.range_R2[count-1].ranges[j.range_R2[count-1].length-1]);
        }
      }
      printf(" basicElementType = integer");
    }
    printf(">\n");
  }
  return;
}

TypeExpressionRecord getTypeExpressionRecord(TypeExpressionTable* E, char* var_name)
{
  TypeExpressionRecord ret_value;
  for (int i = 0; i < E->size; i++)
  {
    if (strcmp(E->T[i].var_name,var_name)==0)
    {
      return E->T[i];
    }
  }
  return ret_value;
}
