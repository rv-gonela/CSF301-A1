#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef GRAMMAR_H
#define GRAMMAR_H

typedef struct SymbolNode SymbolNode;

struct SymbolNode {
  char* symbol;
  boolean is_terminal;
  SymbolNode* next;
};

typedef struct {
  char* lhs;
  SymbolNode* head;
} Grammar;

void readGrammar(char* file_name, Grammar G[]);

#endif
