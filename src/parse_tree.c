#include "parse_tree.h"

// Fills the next level with the grammar rule we are trying
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

// Frees all the children of a node
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
      root->left_child = NULL;
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
  assert(root != NULL); // Sanity check

  printf("Node:\n");
  printf("Symbol:\t%s\n",root->symbol);
  printf("Depth:\t%zu\n",root->depth);
  if (root->is_leaf)
  {
    printf("Lexeme:\t%s\n",root->lexeme);
    printf("Line number:\t%zu\n",root->line_number);
  }
  else
  {
    //TODO: type expression
    printf("Grammar rule: ");
    printRule(root->grammar_rule);
    printf("\n");
  }
  printf("\n");

  // Preorder Traversal
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
//------------------------------------------
void populateExpTable(ParseTreeNode* root, TypeExpressionTable* E)
{
  if (strcmp(root->symbol,"<declaration_list>")!=0) // Sanity check
    return;
  root = root->left_child; // Move to the declare statement

  // Traverse the declaration statement
  ParseTreeNode* head = root->left_child;
  size_t incoming_size = E->size;
  TypeExpressionRecord declare_type;
  while(head != NULL)
  { //**Here we are not jumping into <var_list> to take into account the variable names.
    if (strcmp(head->symbol,"<data_type>")==0)
    {
      // Populate the type expression with this information
      if(strcmp(head->left_child->symbol,"<primitive>")==0)
      {
        declare_type.arrayType = VARCLASS_PRIMITIVE;
        declare_type.rectType= RECTSTATUS_NOT_APPLICABLE;

        // Check which primitive type this is
        ParseTreeNode* primitive_node = head->left_child;
        if (strcmp(primitive_node->left_child->symbol,"INTEGER")==0)
          declare_type.type_expression.t = TYPE_INTEGER;
        else if (strcmp(primitive_node->left_child->symbol,"REAL")==0)
          declare_type.type_expression.t = TYPE_REAL;
        else if (strcmp(primitive_node->left_child->symbol,"BOOL")==0)
          declare_type.type_expression.t = TYPE_BOOLEAN;
      }
      else if (strcmp(head->left_child->symbol,"<array_type>")==0) //**changed array to array_type**
      {
        // Need to go deeper
        ParseTreeNode* array_node = head->left_child;
        if(strcmp(array_node->left_child->symbol,"<rect_array>")==0)
        {
          // This is a rectangular array
          declare_type.arrayType = VARCLASS_RECTANGULAR;
          declare_type.type_expression.t = TYPE_RECTANGULAR_ARRAY;

          // Find the indices
          ParseTreeNode* dim_node = array_node->left_child->left_child->right_sibling->left_child; // Now it points at the dimension in the rhs of rect_dimension_list//** changed array_node->left_child->right_sibling to left_child->left_child->right_sibling->left_child**
          while(1)
          {
            declare_type.type_expression.array.r.dimension_count++;
            //DONE TODO: Allocate more space for the newest range
            if(declare_type.type_expression.array.r.dimension_count==1)
            {
              declare_type.type_expression.array.r.lows=(int*)malloc(sizeof(int));
              declare_type.type_expression.array.r.highs=(int*)malloc(sizeof(int));
            }
            else
            {
              declare_type.type_expression.array.r.lows=(int*)realloc(declare_type.type_expression.array.r.lows, sizeof(int)*declare_type.type_expression.array.r.dimension_count);
              declare_type.type_expression.array.r.highs=(int*)realloc(declare_type.type_expression.array.r.highs, sizeof(int)*declare_type.type_expression.array.r.dimension_count);
            }
            //**removed dim_node = dim_node->left_child;
            // We are now pointing to a single indexing dimension
            ParseTreeNode* sing_index_node = dim_node->left_child;//**sing_index_node i spointing to RECT_OPEN**
            while(sing_index_node != NULL)
            {
              if (strcmp(sing_index_node->left_child->symbol,"<index>")==0)
              {
                //**for low**
                if (strcmp(sing_index_node->left_child->symbol,"INTEGER_LITERAL") == 0)
                {
                  // Static
                  if (declare_type.rectType != RECTSTATUS_DYNAMIC)
                    declare_type.rectType = RECTSTATUS_STATIC;

                  //DONE TODO:Store the index values
                  int index_value = strtol(sing_index_node->lexeme,NULL,10); // Beware, for I have never used this function before

                  declare_type.type_expression.array.r.lows[declare_type.type_expression.array.r.dimension_count-1];

                }
                else if (strcmp(sing_index_node->left_child->symbol,"VAR_ID")==0)
                {
                  // Dynamic
                  declare_type.rectType = RECTSTATUS_DYNAMIC;
                  declare_type.type_expression.array.r.highs[declare_type.type_expression.array.r.dimension_count-1]=0;
                  //DONE TODO:Store the index values
                  //**TODO: Check for type error**
                }
                //for high
                sing_index_node=sing_index_node->right_sibling->right_sibling;
                if (strcmp(sing_index_node->left_child->symbol,"INTEGER_LITERAL") == 0)
                {
                  // Static
                  if (declare_type.rectType != RECTSTATUS_DYNAMIC)
                    declare_type.rectType = RECTSTATUS_STATIC;

                  //DONE TODO:Store the index values
                  int index_value = strtol(sing_index_node->lexeme,NULL,10); // Beware, for I have never used this function before

                  declare_type.type_expression.array.r.highs[declare_type.type_expression.array.r.dimension_count-1]=index_value;

                }
                else if (strcmp(sing_index_node->left_child->symbol,"VAR_ID")==0)
                {
                  // Dynamic
                  declare_type.rectType = RECTSTATUS_DYNAMIC;
                  declare_type.type_expression.array.r.highs[declare_type.type_expression.array.r.dimension_count-1]=0;
                  // DONE TODO:Store the index values
                  //**TODO: Check for type error**
                }
              }
              sing_index_node = sing_index_node->right_sibling;
            }

            // Check the next dimension!
            if (dim_node->right_sibling != NULL)
              dim_node = dim_node->right_sibling->left_child; //**added left_child to keep dim_node on <dimension>**
          }
        }
        else if(strcmp(array_node->left_child->symbol,"<jagged_array>")==0)
        {
          // This is a jagged array
          declare_type.arrayType = VARCLASS_JAGGED;
          declare_type.rectType = RECTSTATUS_NOT_APPLICABLE;
          declare_type.type_expression.t = TYPE_JAGGED_ARRAY; //**changed from TYPE_RECTANGULAR_ARRAY**

          // TODO: parse expression for jagged array
          ParseTreeNode* jagged_dimension_list = array_node->left_child->left_child->right_sibling->right_sibling;
          ParseTreeNode* first_dim = jagged_dimension_list->left_child;
          ParseTreeNode* sing_index_node = first_dim->left_child->right_sibling;
          int index_value = strtol(sing_index_node->left_child->lexeme,NULL,10);
          declare_type.type_expression.array.j.range_R1[0] = index_value;
          sing_index_node= sing_index_node->right_sibling->right_sibling;
          index_value = strtol(sing_index_node->left_child->lexeme,NULL,10);
          declare_type.type_expression.array.j.range_R1[1] = index_value;
          ParseTreeNode* jagged_dimension = first_dim->right_sibling;
          ParseTreeNode* jagged_assign_list = jagged_dimension_list->right_sibling->right_sibling->right_sibling->right_sibling;
          if(jagged_dimension->left_child->right_sibling->right_sibling != NULL)
          {
            declare_type.type_expression.array.j.dimension_count=3;
            //**TODO: PARSE 3d jagged array
          }
          else
          {
            declare_type.type_expression.array.j.dimension_count=2;
            //**TODO: PARSE 2d jagged array 
          }
        }
      }
    }
    else
    {
      if (strcmp(head->symbol,"VAR_ID")==0)
      {
        // Collect these variables
        E->T[E->size++].var_name = head->lexeme;

        // If we are out of space, we need to reallocate.
        if (E->size==E->capacity)
        {
          E->capacity *= 2;
          E->T = realloc(E->T,E->capacity*sizeof(TypeExpressionRecord));
        }
      }
    }
    head = head->right_sibling;
  }

  for (size_t i = incoming_size+1; i < E->size; i++)
  {
    char* name = E->T[i].var_name;
    E->T[i] = declare_type;
    E->T[i].var_name = name;
  }

  // Recurse on the next statement
  if (root->right_sibling != NULL)
  {
    populateExpTable(root->right_sibling,E);
  }
}

// TODO
void validateParseTree(ParseTreeNode* root, TypeExpressionTable* E)
{
}

void traverseParseTree(ParseTree* t, TypeExpressionTable* E)
{
  ParseTreeNode* root = t-> root;

  // Descend the tree to reach the statement list
  root = root->left_child;
  while(strcmp(root->symbol,"<statement_list>")!=0) //**changed == to !=**
  {
    root = root->right_sibling;
  }

  // Fill the type expression table with the declaration list
  ParseTreeNode* declarationRoot = root->left_child;
  populateExpTable(declarationRoot,E);

  // Validate the assignment statements with the type expression table
  ParseTreeNode* assignmentRoot = declarationRoot->right_sibling;
  validateParseTree(assignmentRoot,E);
}
