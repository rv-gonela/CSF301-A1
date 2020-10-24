#include "stack.h"

void pop(Stack* st)
{
  if (st->top_index==0)
  {
    printf("Underflow\n");
    exit(0);
  }

  st->top_index--;
}

void push(Stack* st, char* x)
{
  st->stack_array[++st->top_index] = x;
}

char* peek(Stack* st)
{
  if(st->top_index==0)
  {
    printf("Underflow\n");
    exit(0);
  }
  return st->stack_array[st->top_index-1];
}
