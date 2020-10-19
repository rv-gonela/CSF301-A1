#include "token_stream.h"

void tokenizeSourceCode(char* file_name, TokenStream* s)
{
  FILE *fptr = fopen(file_name,"r"); // File stream
  char line_buffer[100]; // Buffer for reading lines
  char lexeme_buffer[100]; // Buffer for reading lexemes
  size_t lexeme_buffer_size = 0; // Lexeme buffer size
  TokenStreamNode* head = s->head; // Current head of the linked list being built
  size_t line_number = 1; 

  if (fptr==NULL)
  {
    printf("File failed to open");
    exit(0);
  }

  // Read entire file.
  while(fgets(line_buffer,sizeof(line_buffer), fptr) != NULL)
  {
    printf("%s\n",line_buffer);
    for (size_t i = 0; i < 100; i++)
    {
      if (line_buffer[i]=='\n'|| line_buffer[i]==' ') // Line end
      {
        if (lexeme_buffer_size != 0)
        {
          head = (TokenStreamNode*) malloc(sizeof(TokenStreamNode));
          head->lexeme = (char*)malloc((lexeme_buffer_size+1)*sizeof(char));
          memcpy(head->lexeme,lexeme_buffer,lexeme_buffer_size);
          head->lexeme[lexeme_buffer_size] = '\0';
          head->lexeme_size = lexeme_buffer_size;
          head->line_number = line_number;
          head->token_type = lexemeType(head->lexeme);
          head = head->next;
        }
        if (line_buffer[i]=='\n')
        {
          line_number++;
          break;
        }
      }
      else
      {
        lexeme_buffer[lexeme_buffer_size++] = line_buffer[i];
      }
    }
  }
}


