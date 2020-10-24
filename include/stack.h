#include <stdio.h>
#include <stdlib.h>

#ifndef STACK_H
#define STACK_H

#define STACK_SIZE 2048 // May need to be tweaked

/*
 * Implementation of a stack.
 *
 * TODO: Chaneg datatype as per our usecase
 */
typedef struct 
{
  char* stack_array[STACK_SIZE];
  int top_index;
} Stack;

/*
 * Pop the topmost element from the stack
 */
void pop(Stack* st);

/*
 * Push the element into the stack
 */
void push(Stack* st, char* x);

/*
 * Peek the top element of the stack
 */
char* peek(Stack* st);

#endif
