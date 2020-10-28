#include "token.h"

enum Token lexemeType(char* lexeme)
{
  if (strcmp(lexeme,"program")==0)
    return TOKEN_PROGRAM;
  if (strcmp(lexeme,"()")==0)
    return TOKEN_PAR;
  if (strcmp(lexeme,"{")==0)
    return TOKEN_CURLY_OPEN;
  if (strcmp(lexeme,"}")==0)
    return TOKEN_CURLY_CLOSE;
  if (strcmp(lexeme,"declare")==0)
    return TOKEN_DECLARE;
  if (strcmp(lexeme,":")==0)
    return TOKEN_COLON;
  if (strcmp(lexeme,";")==0)
    return TOKEN_SEMICOLON;
  if (strcmp(lexeme,"integer")==0)
    return TOKEN_INTEGER;
  if (strcmp(lexeme,"real")==0)
    return TOKEN_REAL;
  if (strcmp(lexeme,"boolean")==0)
    return TOKEN_BOOL;
  if (strcmp(lexeme,"list")==0)
    return TOKEN_LIST;
  if (strcmp(lexeme,"of")==0)
    return TOKEN_OF;
  if (strcmp(lexeme,"variables")==0)
    return TOKEN_VARIABLES;
  if (strcmp(lexeme,"[")==0)
    return TOKEN_RECT_OPEN;
  if (strcmp(lexeme,"]")==0)
    return TOKEN_RECT_CLOSE;
  if (strcmp(lexeme,"..")==0)
    return TOKEN_DOUBLE_PERIOD;
  if (strcmp(lexeme,"jagged")==0)
    return TOKEN_JAGGED;
  if (strcmp(lexeme,"array")==0)
    return TOKEN_ARRAY;
  if (strcmp(lexeme,"=")==0)
    return TOKEN_EQUAL_SIGN;
  if (strcmp(lexeme,"+")==0)
    return TOKEN_PLUS;
  if (strcmp(lexeme,"-")==0)
    return TOKEN_MINUS;
  if (strcmp(lexeme,"/")==0)
    return TOKEN_DIVIDE;
  if (strcmp(lexeme,"*")==0)
    return TOKEN_MULTIPLY;
  if (strcmp(lexeme,"&&&")==0)
    return TOKEN_AND;
  if (strcmp(lexeme,"|||")==0)
    return TOKEN_OR;
  if(strcmp(lexeme,"size")==0)
    return TOKEN_SIZE;
  if(strcmp(lexeme,"values")==0)
    return TOKEN_VALUES;
  if (strcmp(lexeme,"R1")==0)
    return TOKEN_R1;
  
  for (size_t i = 0; i < strlen(lexeme); i++)
  {
    if (lexeme[i] < '0' || lexeme[i] > '9')
      return TOKEN_VAR_ID;
  }

  return TOKEN_INTEGER_LITERAL; // If it is not any of these, it must be an integer literal
}

const char* tokenToSymbolMap[31] = {"PROGRAM", "PAR", "CURLY_OPEN", "CURLY_CLOSE", "DECLARE", "COLON", "SEMICOLON", "INTEGER", "REAL", "BOOL", "LIST", "OF", "VARIABLES", "RECT_OPEN", "RECT_CLOSE", "DOUBLE_PERIOD", "JAGGED", "ARRAY", "EQUAL_SIGN", "PLUS", "MINUS", "DIVIDE", "MULTIPLY", "AND", "OR", "VAR_ID", "INTEGER_LITERAL", "SIZE", "VALUES", "R1", "NONE"};
