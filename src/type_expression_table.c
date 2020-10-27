#include "type_expression_table.h"

void printTypeExpressionTable(TypeExpressionTable T){
  /*for(int i=0;i<T.size;i++)*/
  /*{*/
    /*printf("%-24s", T[i].field1);*/
    /*printf("%-15s", var_class_map[T[i].field2]);*/
    /*printf("%-18s", rect_status_map[T[i].field3]);*/
    /*printf("<type =");*/
    /*printf("%s", type_map[T[i].field4.t]);*/
    /*if(T[i].field2!=0)*/
    /*{*/
      /*if(T[i].field4.t==1)*/
      /*{*/
        /*rectArray r=T[i].field4.array.r;*/
        /*printf(", dimensions=%d, ", r.dimension_count);*/
        /*for(int dim=1; dim<=r.dimension_count; dim++)*/
        /*{*/
          /*printf("range_R%d= (%d,%d), ", dim, r.ranges[dim-1][0], r.ranges[dim-1][1]);*/
        /*}*/
      /*}*/
      /*else*/
      /*{*/
        /*jaggedArray j=T[i].field4.array.j;*/
        /*printf(", dimensions=%d, ", j.dimension_count);*/
        /*printf("range_R1= (%d,%d), ", j.range_R1[0], j.range_R1[1]);*/
        /*int temcountp=j.range_R1[1] - j.range_R1[0] + 1;*/
        /*printf("range_R2= (");*/
        /*if(j.dimension_count==2)*/
        /*{*/
          /*for(int dim = 0; dim < count-1; dim++)*/
          /*{*/
            /*printf("%d, ", j.range_R2[dim].ranges[0]);*/
          /*}*/
          /*printf("%d", j.range_R2[count-1].ranges[0]);*/
        /*}*/
        /*else*/
        /*{*/
          /*for(int dim = 0; dim < count-1; dim++)*/
          /*{*/
            /*printf("%d[", j.range_R2[dim].length);*/
            /*for(int dim2=0; dim2 < j.range_R2[dim].length-1; dim2++)*/
            /*{*/
              /*printf("%d, ", j.range_R2[dim].ranges[dim2]);*/
            /*}*/
            /*printf("%d] ,", j.range_R2[dim].ranges[j.range_R2[dim].length-1]);*/
          /*}*/
          /*for(int dim2=0; dim2 < j.range_R2[count-1].length-1; dim2++)*/
          /*{*/
            /*printf("%d, ", j.range_R2[count-1].ranges[dim2]);*/
          /*}*/
          /*printf("%d]", j.range_R2[count-1].ranges[j.range_R2[count-1].length-1]);*/
        /*}*/
      /*}*/
      /*printf("),basicElementType = integer");*/
    /*}*/
    /*printf(">\n");*/
  /*}*/
  /*return;*/
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
