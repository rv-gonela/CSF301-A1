#include "token.h"

enum Token lexemeType(char* lexeme)
{
  if (strcmp(lexeme,"program")==0)
    return PROGRAM;
  if (strcmp(lexeme,"()")==0)
    return PAR;
  if (strcmp(lexeme,"{")==0)
    return CURLY_OPEN;
  if (strcmp(lexeme,"}")==0)
    return CURLY_CLOSE;
  if (strcmp(lexeme,"declare")==0)
    return DECLARE;
  if (strcmp(lexeme,":")==0)
    return COLON;
  if (strcmp(lexeme,";")==0)
    return SEMICOLON;
  if (strcmp(lexeme,"integer")==0)
    return INTEGER;
  if (strcmp(lexeme,"real")==0)
    return REAL;
  if (strcmp(lexeme,"boolean")==0)
    return BOOL;
  if (strcmp(lexeme,"list")==0)
    return LIST;
  if (strcmp(lexeme,"of")==0)
    return OF;
  if (strcmp(lexeme,"variables")==0)
    return VARIABLES;
  if (strcmp(lexeme,"[")==0)
    return RECT_OPEN;
  if (strcmp(lexeme,"]")==0)
    return RECT_CLOSE;
  if (strcmp(lexeme,"jagged")==0)
    return JAGGED;
  if (strcmp(lexeme,"array")==0)
    return ARRAY;
  if (strcmp(lexeme,"=")==0)
    return EQUAL_SIGN;
  if (strcmp(lexeme,"+")==0)
    return PLUS;
  if (strcmp(lexeme,"-")==0)
    return MINUS;
  if (strcmp(lexeme,"/")==0)
    return DIVIDE;
  if (strcmp(lexeme,"*")==0)
    return MULTIPLY;
  if (strcmp(lexeme,"&&&")==0)
    return AND;
  if (strcmp(lexeme,"|||")==0)
    return OR;
  
  for (size_t i = 0; i < strlen(lexeme); i++)
  {
    if (lexeme[i] < '0' || lexeme[i] > '9')
      return STRING;
  }

  return INTEGER_LITERAL; // If it is not any of these, it must be a variable name
}
