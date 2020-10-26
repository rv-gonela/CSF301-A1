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

  printf("Grammar loaded\n");

  /*for (int i = 0; i < G->rule_count; i++)*/
  /*{*/
    /*SymbolNode* head = G->grammar_lhs[i];*/
    /*while(head != NULL)*/
    /*{*/
      /*printf("%s ", head->symbol);*/
      /*head = head->next;*/
    /*}*/
    /*printf("\n\n");*/
  /*}*/

  ParseTree* t = (ParseTree*)malloc(sizeof(ParseTree));
  createParseTree(t,s,G);
  printParseTree(t);

  free(s);
  free(G);
  free(t);
}
