#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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
  int is_leaf; // If it is a leaf, its also a terminal
  size_t depth;

  // Stuff a leaf node needs
  char* lexeme;
  size_t line_number;
  
  // Stuff a non leaf node needs
  typeExpression type_expression;
  SymbolNode* grammar_rule;
  char* symbol;
};

typedef struct{
  ParseTreeNode* root;
} ParseTree;

void createParseTree(ParseTree* t, TokenStream* s, Grammar* G);

void printParseTree(ParseTree* t);

#endif
