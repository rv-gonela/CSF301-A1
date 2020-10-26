#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "token.h"
#include "token_stream.h"
#include "grammar.h"
#include "stack.h"
#include "type_expression.h"
#include "type_expression_table.h"

#ifndef PARSE_TREE_H
#define PARSE_TREE_H

typedef struct ParseTreeNode ParseTreeNode;


/*
 * Struct for a single node of the parse tree
 */
struct ParseTreeNode{
  ParseTreeNode* left_child;
  ParseTreeNode* right_sibling;
  int is_leaf; // If it is a leaf, its also a terminal
  size_t depth;

  // Stuff a leaf node needs
  char* lexeme;
  size_t line_number;
  
  // Stuff a non leaf node needs
  TypeExpression type_expression;
  SymbolNode* grammar_rule;
  char* symbol;
};

/* 
 * Struct containing the entire parse tree
 */
typedef struct{
  ParseTreeNode* root;
} ParseTree;

/*
 * Builds the parse tree given the token stream and grammar
 */
void createParseTree(ParseTree* t, TokenStream* s, Grammar* G);

/*
 * Prints the given parse tree
 *
 * TODO: Type expressions
 */
void printParseTree(ParseTree* t);

/*
 * Traverses the parse tree and populates the type expression table
 */
void traverseParseTree(ParseTree* t, TypeExpressionTable* E);


#endif
