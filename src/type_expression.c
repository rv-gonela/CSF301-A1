#include "type_expression.h"

int isTEEqual(TypeExpression a, TypeExpression b)
{
  if (a.t != b.t)
    return 0;

  if (a.t==TYPE_RECTANGULAR_ARRAY)
  {
    RectArray ar = a.array.r;
    RectArray br = b.array.r;

    // This check should always be true but might as well
    if(ar.basic_element_type != br.basic_element_type)
      return 0;
    if (ar.dimension_count != br.dimension_count)
      return 0;
    for (size_t i = 0; i < ar.dimension_count; i++)
    {
      if (ar.highs[i] != br.highs[i] || ar.lows[i] != br.lows[i])
        return 0;
    }
  }

  if(a.t == TYPE_JAGGED_ARRAY)
  {
    // TODO: jagged array equality
    JaggedArray aj = a.array.j;
    JaggedArray bj = b.array.j;
    if (aj.dimension_count != bj.dimension_count)
      return 0;
    if(aj.range_R1[0]!=bj.range_R1[0] || aj.range_R1[1]!=bj.range_R1[1])
      return 0;
    if(aj.dimension_count==2)
    {
      for(int i=0; i<aj.range_R1[1]-aj.range_R1[0]+1; i++)
      {
        if(aj.range_R2[i].length!=bj.range_R2[i].length)
          return 0;
      }
    }
    else
    {
      for(int i=0; i<aj.range_R1[1]-aj.range_R1[0]+1; i++)
      {
        if(aj.range_R2[i].length!=bj.range_R2[i].length)
          return 0;
        for(int j=0;j<aj.range_R2[i].length;j++)
        {
          if(aj.range_R2[i].ranges[j]!=bj.range_R2[i].ranges[j])
            return 0;
        }
      }
    }

  }

  return 1;
}

// Print the type expression
void printTE(TypeExpression a)
{
  printf("<type = ");
  printf("%s",type_map[a.t]);
  if (a.t==TYPE_RECTANGULAR_ARRAY)
  {
    printf(", dimensions=%d",a.array.r.dimension_count);
    for (int dim = 1; dim <= a.array.r.dimension_count; dim++)
      printf(" range_R%d=(%d,%d)", dim, a.array.r.lows[dim-1], a.array.r.highs[dim-1]);
    printf(" basicElementType = integer");
  }
  if (a.t==TYPE_JAGGED_ARRAY)
  {
    JaggedArray j=a.array.j;
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
    printf(" basicElementType = integer");
  }
  printf(">");
}
