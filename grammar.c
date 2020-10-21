#include "grammar.h"

void readGrammar(char* file_name, Grammar* G)
{
  FILE *fptr = fopen(file_name,"r"); // File stream
  char line_buffer[100]; // Buffer for reading lines
  char c;
  int count = 0;


  if (fptr==NULL)
  {
    printf("File failed to open");
    exit(0);
  }
  //count number of lines
  for (c = getc(fptr); c != EOF; c = getc(fptr))
  {
    if (c == '\n') // Increment count if this character is newline
    {
      count = count + 1;
    }
  }  // Close the file
  fclose(fptr);

  G->grammar_lhs= (SymbolNode**)malloc((sizeof(SymbolNode*)*count))
  G->rule_count=count;

  fptr = fopen(file_name,"r");
  // Read entire file.

  SymbolNode* prod_head = *(G->grammar_lhs);

  while(fgets(line_buffer,sizeof(line_buffer), fptr) != NULL)
  {
    int lhs_flag=1;
    int i=0;

    while(line_buffer[i]!='\n')
    {
      if(lhs_flag)
      {
        char symbol_buffer[20];
        int j=0;
        while( line_buffer[i]!=' ')
        {
          symbol_buffer[j++]=line_buffer[i++];
        }
        symbol_buffer[j]='\0';
        *prod_head=(SymbolNode*)malloc(sizeof(SymbolNode));
        strcpy(prod_head->symbol,symbol_buffer);
        prod_head->is_terminal=0;
        prod_head->symbol_size=j;
        SymbolNode* curr_symbol=prod_head;
        curr_symbol->next=(SymbolNode*)malloc(sizeof(SymbolNode));
        lhs_flag=0;
      }

      if(line_buffer[i++]==' ')
      {
        char symbol_buffer[20];
        int j=0;
        while(line_buffer[i]!=' ' || line_buffer[i]!='\n')
        {
          symbol_buffer[j++]=line_buffer[i++];
        }
        symbol_buffer[j]='\0';
        curr_symbol=curr_symbol->next;
        strcpy(curr_symbol->symbol,symbol_buffer);
        curr_symbol->is_terminal = (symbol_buffer[0]=='<') ? 0 : 1 ;
        curr_symbol->symbol_size=j;
        curr_symbol->next=(SymbolNode*)malloc(sizeof(SymbolNode));
      }
    }
    if(curr_symbol->next!=NULL)
    {
      curr_symbol->next=NULL;
      free(curr_symbol->next);
    }
    prod_head++;
  }
}
