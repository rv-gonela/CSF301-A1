#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H

// Defined to allow nested pointers.
typedef struct TokenStreamNode TokenStreamNode;

/*
 * Struct defining a token stream node
 *
 * lexeme : A string containing the parsed lexeme
 * lexeme_size : Size of lexeme
 * line_number : Current line number (1 indexed)
 * token_type : The token class of the lexeme
 * next : The pointer to the next element in the linked list
 */
struct TokenStreamNode {
  char* lexeme;
  size_t lexeme_size;
  size_t line_number;
  enum Token token_type;
  TokenStreamNode* next;
};

/* 
 * Struct defining the token stream
 *
 * head : A pointer to the first element of the token stream
 */
typedef struct {
  TokenStreamNode* head;
} TokenStream;

/*
 * Reads the source code from a file and populates the token stream
 *
 * file_name : The name of the source code file
 * s : The token stream to populate
 */
void tokenizeSourceCode(char* file_name, TokenStream* s);

#endif

