#include <string.h>

#ifndef TOKEN_H
#define TOKEN_H

/*
 * Enum defining all the terminals (token classes)
 */
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

/*
 * Categorizes a given lexeme into its token class.
 *
 * lexeme : The given lexeme
 */
enum Token lexemeType(char* lexeme);

#endif
