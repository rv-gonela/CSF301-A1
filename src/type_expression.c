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
  }

  return 1;
}

void printTE(TypeExpression a)
{
}
