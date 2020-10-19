#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grammar.h"

void readGrammar(char* file_name, Grammar G[])
{
  FILE *fptr = fopen(file_name,"r"); // File stream
  char line_buffer[100]; // Buffer for reading lines
  SymbolNode temp;
  size_t line_number = 1;

  if (fptr==NULL)
  {
    printf("File failed to open");
    exit(0);
  }

  // Read entire file.
  while(fgets(line_buffer,sizeof(line_buffer), fptr) != NULL)
  {
    
  }
}
