#include "token_stream.h"
#include "parse_tree.h"
#include "grammar.h"

int main()
{
  TokenStream* s = (TokenStream*)malloc(sizeof(TokenStream));
  tokenizeSourceCode("source.txt",s);
  printf("Source Code Tokenized\n");

  Grammar* G = (Grammar*)malloc(sizeof(Grammar));
  readGrammar("grammar.txt",G);
  //Driver code to print grammar rules
  /*
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
  */
  printf("Grammar loaded\n");

  ParseTree* t = (ParseTree*)malloc(sizeof(ParseTree));
  createParseTree(t,s,G);
  printParseTree(t);

  TypeExpressionTable* E = malloc(sizeof(TypeExpressionTable));
  E->capacity = 8;
  E->size = 0;
  E->T = malloc(E->capacity*sizeof(TypeExpressionRecord));
  traverseParseTree(t,E);
  printTypeExpressionTable(*E);

  free(s);
  free(G);
  free(t);
}
