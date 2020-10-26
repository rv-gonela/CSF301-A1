#include "parse_tree.h"

void fillSiblingsWithRule(ParseTreeNode* root, SymbolNode* head)
{
  ParseTreeNode* prev = NULL;
  while(head != NULL)
  {
    root->symbol = head->symbol;
    root->right_sibling = (ParseTreeNode*)malloc(sizeof(ParseTreeNode));
    root->right_sibling->depth = root->depth;
    root->right_sibling->is_leaf = 0;
    prev = root;
    root = root->right_sibling;
    head = head->next;
  }
  if(prev != NULL)
  {
    free(root);
    prev->right_sibling = NULL;
  }
}

void freeSiblingsMemory(ParseTreeNode* root)
{
  while(root != NULL)
  {
    ParseTreeNode* tmp = root;
    root = root->right_sibling;
    free(tmp);
  }
}

int fillParseTree(ParseTreeNode* root, TokenStreamNode** head, Grammar* G)
{
  char* symbol = root->symbol;
  int works = 0;
  TokenStreamNode* savedHead = *head; // Save the head to revert if things go wrong

  // If it is a terminal, make sure it matches the token!
  if (symbol[0]!='<')
  {
    if (strcmp(tokenToSymbolMap[(*head)->token_type],symbol)==0)
    {
      root->is_leaf = 1;
      root->lexeme = (*head)->lexeme;
      root->line_number = (*head)->line_number;
      *head = (*head)->next;
      works = 1;
    }
    else
    {
      works = 0;
    }
  }
  else
  {
    // It is nonterminal, so let us check which rule can work!
    for (size_t i = 0; i < G->rule_count; i++)
    {
      if (strcmp(G->grammar_lhs[i]->symbol,symbol)==0)
      {
        // Create the children
        root->left_child = (ParseTreeNode*)malloc(sizeof(ParseTreeNode)); 
        root->left_child->depth = root->depth+1;
        root->left_child->is_leaf = 0;
        fillSiblingsWithRule(root->left_child,G->grammar_lhs[i]->next);

        // Try the rule
        works |= fillParseTree(root->left_child,head,G);
        if (!works)
        {
          *head = savedHead;
          freeSiblingsMemory(root->left_child);
          root->left_child = NULL;
        }
        else
        {
          root->grammar_rule = G->grammar_lhs[i];
          break;
        }
      }
    }
  }

  // Continue trying the next rules
  if (works && root->right_sibling != NULL)
  {
    works &= fillParseTree(root->right_sibling,head,G);
  }

  if(!works)
  {
    *head = savedHead;
  }

  return works;
}

void createParseTree(ParseTree* t, TokenStream* s, Grammar* G)
{
  char* start_symbol = "<program>";

  // Initialize the node
  t->root = (ParseTreeNode*)malloc(sizeof(ParseTreeNode));
  t->root->depth = 0;
  t->root->is_leaf = 0;
  t->root->symbol = start_symbol;
  t->root->right_sibling = NULL;
  TokenStreamNode** ptr = &(s->head);

  if (fillParseTree(t->root,ptr,G))
  {
    printf("Parse Tree Created Successfully!\n");
  }
  else
  {
    printf("Something went wrong :(\n");
    exit(0);
  }
}

void recursePrintParseTree(ParseTreeNode* root)
{
  assert(root != NULL);
  // Print this node
  printf("Node:\n");
  if(root->is_leaf)
  {
    printf("Node is a leaf\n");
    printf("Lexeme:%s\n",root->lexeme);
    printf("Line number: %zu\n",root->line_number);
  }
  else
  {
    printf("Node is internal\n");
    // TODO: Type expression
    printf("Symbol:%s\n",root->symbol);
    /*printf("Grammar rule: TODO\n"); // TODO grammar rule*/
  }

  // DFS
  if(root->left_child != NULL)
    recursePrintParseTree(root->left_child);
  if(root->right_sibling != NULL)
    recursePrintParseTree(root->right_sibling);
}

void printParseTree(ParseTree* t)
{
  ParseTreeNode* root = t->root;
  recursePrintParseTree(root);
}
