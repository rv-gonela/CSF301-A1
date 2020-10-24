#include "parse_tree.h"

void createParseTree(ParseTree* t, TokenStream* s, Grammar* G)
{
  TokenStreamNode* token_head = s->head;
  char* start_symbol = "<program>";
  Stack* st = (Stack*)malloc(sizeof(Stack));
  push(st,start_symbol);

  while(token_head != NULL)
  {
    // Get the next terminal for left derivation
    char* curr_symbol = peek(st);
    pop(st);

    // Select the rule
    for (size_t i = 0; i < G->rule_count; i++)
    {
      // Candidate rule
      if (strcmp(G->grammar_lhs[i]->symbol,curr_symbol)==0)
      {

      }
    }
  }


}
