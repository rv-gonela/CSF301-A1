#include <string.h>

#ifndef TOKEN_H
#define TOKEN_H

enum Token {
    PROGRAM,
    PAR,
    CURLY_OPEN,
    CURLY_CLOSE,
    DECLARE,
    COLON,
    SEMICOLON,
    INTEGER,
    REAL,
    BOOL,
    LIST,
    OF,
    VARIABLES,
    RECT_OPEN,
    RECT_CLOSE,
    DOUBLE_PERIOD,
    JAGGED,
    ARRAY,
    EQUAL_SIGN,
    PLUS,
    MINUS,
    DIVIDE,
    MULTIPLY,
    AND,
    OR,
    STRING
};

enum Token lexemeType(char* lexeme);

#endif
