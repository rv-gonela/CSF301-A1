#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "token_stream.h"
#include "type_expression.h"

#ifndef PARSE_TREE_H
#define PARSE_TREE_H

typedef struct ParseTreeNode ParseTreeNode;

struct ParseTreeNode{
  ParseTreeNode* left_child;
  ParseTreeNode* right_sibling;
  // Some representation of the non-terminal
  // Tagged union? Why?
  // Type? How to represent it?
};

typedef struct{
  ParseTreeNode* root;
} ParseTree;

#endif
