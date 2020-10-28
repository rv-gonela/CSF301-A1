#include "grammar.h"

void readGrammar(char* file_name, Grammar* G)
{
  FILE *fptr = fopen(file_name,"r"); // File stream
  char line_buffer[125]; // Buffer for reading lines
  char c;
  int count = 0;

  if (fptr==NULL)
  {
    printf("File failed to open");
    exit(0);
  }

  //Count number of lines
  for (c = getc(fptr); c != EOF; c = getc(fptr))
  {
    if (c == '\n') // Increment count if this character is newline
      count++;
  }
  fclose(fptr); // Close the file

  G->grammar_lhs= (SymbolNode**)malloc((sizeof(SymbolNode*)*count));
  G->rule_count=count;

  fptr = fopen(file_name,"r");
  SymbolNode* curr_symbol;
  int k=0;
  // Read entire file
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
        G->grammar_lhs[k]=(SymbolNode*)malloc(sizeof(SymbolNode));
        G->grammar_lhs[k]->symbol=(char*)malloc(sizeof(char)*(j+1));
        memcpy(G->grammar_lhs[k]->symbol,symbol_buffer,j+1);
        G->grammar_lhs[k]->is_terminal=0;
        G->grammar_lhs[k]->symbol_size=j;
        curr_symbol=G->grammar_lhs[k++];
        curr_symbol->next=(SymbolNode*)malloc(sizeof(SymbolNode));
        lhs_flag=0;
      }

      if(line_buffer[i++]==' ')
      {
        char symbol_buffer[20];
        int j=0;
        while(line_buffer[i]!=' ' && line_buffer[i]!='\n')
        {
          symbol_buffer[j++]=line_buffer[i++];
        }
        symbol_buffer[j]='\0';
        curr_symbol=curr_symbol->next;
        curr_symbol->symbol=(char*)malloc(sizeof(char)*(j+1));
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
  }
  return;
}

void printRule(SymbolNode* head)
{
  char pretty_rule[200] = "";
  strcat(pretty_rule,head->symbol);
  strcat(pretty_rule," -> ");
  head = head->next;
  while(head != NULL)
  {
    strcat(pretty_rule,head->symbol);
    strcat(pretty_rule," ");
    head= head->next;
  }
  printf("%-140s",pretty_rule);
}
