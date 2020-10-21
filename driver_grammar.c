#include "grammar.h"

int main()
{
  Grammar* G=(Grammar*)malloc(sizeof(Grammar));
  readGrammar("grammar.txt", G);

  SymbolNode** list=G->grammar_lhs;
  SymbolNode* prod_rule=*list;
  for(int i=0;i<G->rule_count;i++)
  {
    SymbolNode* head=prod_rule;
    while(head != NULL)
    {
      printf("%s ",head->symbol);
      head = head->next;
    }
    printf("\n");
    prod_rule++;
  }
}
