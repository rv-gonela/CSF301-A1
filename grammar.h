#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef GRAMMAR_H
#define GRAMMAR_H

typedef struct SymbolNode SymbolNode;
typedef struct Grammar Grammar;

struct SymbolNode {
  char* symbol;
  size_t symbol_size;
  int is_terminal;
  SymbolNode* next;
};

typedef struct Grammar{
  SymbolNode** grammar_lhs;
  int rule_count;
};

void readGrammar(char* file_name, Grammar* G);

#endif
