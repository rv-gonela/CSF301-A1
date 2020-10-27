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

void printTE(TypeExpression a)
{
}
