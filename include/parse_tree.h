#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "token_stream.h"
#include "grammar.h"
#include "stack.h"
#include "type_expression.h"

#ifndef PARSE_TREE_H
#define PARSE_TREE_H

typedef struct ParseTreeNode ParseTreeNode;

struct ParseTreeNode{
  ParseTreeNode* left_child;
  ParseTreeNode* right_sibling;
  char* non_terminal;
  typeExpression type;
};

typedef struct{
  ParseTreeNode* root;
} ParseTree;

void createParseTree(ParseTree *t, TokenStream *s, Grammar* G);

#endif
