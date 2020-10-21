#include "grammar.h"

int main()
{
  Grammar* G=(Grammar*)malloc(sizeof(Grammar));
  readGrammar("grammar.txt", G);
  for(int i=0;i<G->rule_count;i++)
  {
    SymbolNode* head=G->grammar_lhs[i];
    while(head!=NULL)
    {
      printf("%s ",head->symbol );
      head=head->next;
    }
    printf("\n");
  }
}
