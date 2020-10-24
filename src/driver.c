#include "token_stream.h"

int main()
{
  TokenStream* s = (TokenStream*)malloc(sizeof(TokenStream));
  tokenizeSourceCode("source.txt",s);

  TokenStreamNode* head = s->head;
  while(head != NULL)
  {
    printf("%s\n",head->lexeme);
    head = head->next;
  }
}
