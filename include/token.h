#include <string.h>

#ifndef TOKEN_H
#define TOKEN_H

/*
 * Enum defining all the terminals (token classes)
 */
enum Token {
  TOKEN_PROGRAM,
  TOKEN_PAR,
  TOKEN_CURLY_OPEN,
  TOKEN_CURLY_CLOSE,
  TOKEN_DECLARE,
  TOKEN_COLON,
  TOKEN_SEMICOLON,
  TOKEN_INTEGER,
  TOKEN_REAL,
  TOKEN_BOOL,
  TOKEN_LIST,
  TOKEN_OF,
  TOKEN_VARIABLES,
  TOKEN_RECT_OPEN,
  TOKEN_RECT_CLOSE,
  TOKEN_DOUBLE_PERIOD,
  TOKEN_JAGGED,
  TOKEN_ARRAY,
  TOKEN_EQUAL_SIGN,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_DIVIDE,
  TOKEN_MULTIPLY,
  TOKEN_AND,
  TOKEN_OR,
  TOKEN_VAR_ID,
  TOKEN_INTEGER_LITERAL,
  TOKEN_SIZE,
  TOKEN_VALUES,
  TOKEN_NONE
};

/*
 * Categorizes a given lexeme into its token class.
 *
 * lexeme : The given lexeme
 */
enum Token lexemeType(char* lexeme);

extern const char* tokenToSymbolMap[30];

#endif
