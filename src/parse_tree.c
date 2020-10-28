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
      works = 0;
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
          root->line_number = root->left_child->line_number;
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
  {
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
      else if (strcmp(head->left_child->symbol,"<array_type>")==0)
      {
        // Need to go deeper
        ParseTreeNode* array_node = head->left_child;
        if(strcmp(array_node->left_child->symbol,"<rect_array>")==0)
        {
          // This is a rectangular array
          declare_type.arrayType = VARCLASS_RECTANGULAR;
          declare_type.type_expression.t = TYPE_RECTANGULAR_ARRAY;
          declare_type.type_expression.array.r.basic_element_type = TYPE_INTEGER;
          declare_type.type_expression.array.r.dimension_count=0;
          // Find the indices
          ParseTreeNode* dim_node = array_node->left_child->left_child->right_sibling->left_child;
          while(1)
          {
            declare_type.type_expression.array.r.dimension_count++;
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
            // We are now pointing to a single indexing dimension
            ParseTreeNode* sing_index_node = dim_node->left_child->right_sibling;//**sing_index_node is pointing to RECT_OPEN**
            while(sing_index_node != NULL)
            {
              if (strcmp(sing_index_node->symbol,"<index>")==0)
              {
                //**for low**
                if (strcmp(sing_index_node->left_child->symbol,"INTEGER_LITERAL") == 0)
                {
                  // Static
                  if (declare_type.rectType != RECTSTATUS_DYNAMIC)
                    declare_type.rectType = RECTSTATUS_STATIC;
                  int index_value = strtol(sing_index_node->left_child->lexeme,NULL,10);
                  declare_type.type_expression.array.r.lows[declare_type.type_expression.array.r.dimension_count-1] = index_value;
                }
                else if (strcmp(sing_index_node->left_child->symbol,"VAR_ID")==0)
                {
                  // Dynamic
                  declare_type.rectType = RECTSTATUS_DYNAMIC;
                  declare_type.type_expression.array.r.highs[declare_type.type_expression.array.r.dimension_count-1]=0;

                  // Check for type error
                  TypeExpressionRecord var_ter = getTypeExpressionRecord(E,sing_index_node->left_child->lexeme);
                  if(var_ter.type_expression.t != TYPE_INTEGER)
                  {
                    printf("**ERROR**\n");
                    printf("Line Number: %zu\n",sing_index_node->left_child->line_number);
                    printf("Statement Type: Declaration\n");
                    printf("Operator: Index access\n");
                    //TODO: The desired output only makes sense in assignment
                    //statements.
                    printf("Depth:%zu\n",sing_index_node->left_child->depth);
                    printf("Array ranges must be an integer\n");
                  }
                }
                //for high
                sing_index_node=sing_index_node->right_sibling->right_sibling;
                if (strcmp(sing_index_node->left_child->symbol,"INTEGER_LITERAL") == 0)
                {
                  // Static
                  if (declare_type.rectType != RECTSTATUS_DYNAMIC)
                    declare_type.rectType = RECTSTATUS_STATIC;
                  int index_value = strtol(sing_index_node->left_child->lexeme,NULL,10);
                  declare_type.type_expression.array.r.highs[declare_type.type_expression.array.r.dimension_count-1]=index_value;
                }
                else if (strcmp(sing_index_node->left_child->symbol,"VAR_ID")==0)
                {
                  // Dynamic
                  declare_type.rectType = RECTSTATUS_DYNAMIC;
                  declare_type.type_expression.array.r.highs[declare_type.type_expression.array.r.dimension_count-1]=0;

                  // Check Type error
                  TypeExpressionRecord var_ter = getTypeExpressionRecord(E,sing_index_node->left_child->lexeme);
                  if(var_ter.type_expression.t != TYPE_INTEGER)
                  {
                    printf("**ERROR**\n");
                    printf("Line Number: %zu\n",sing_index_node->left_child->line_number);
                    printf("Statement Type: Declaration\n");
                    printf("Operator: Index access\n");
                    //TODO: The desired output only makes sense in assignment
                    //statements.
                    printf("Depth:%zu\n",sing_index_node->left_child->depth);
                    printf("Array ranges must be an integer\n");
                  }
                }

                // BREAK HERE
                break;
              }
              sing_index_node = sing_index_node->right_sibling;
            }

            // Check the next dimension!
            if (dim_node->right_sibling != NULL)
              dim_node = dim_node->right_sibling->left_child; //**added left_child to keep dim_node on <dimension>**
            else
              break;
          }
        }
        else if(strcmp(array_node->left_child->symbol,"<jagged_array>")==0)
        {
          // This is a jagged array
          declare_type.arrayType = VARCLASS_JAGGED;
          declare_type.rectType = RECTSTATUS_NOT_APPLICABLE;
          declare_type.type_expression.t = TYPE_JAGGED_ARRAY; //**changed from TYPE_RECTANGULAR_ARRAY**
          declare_type.type_expression.array.j.basic_element_type = TYPE_INTEGER;

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

          ParseTreeNode* jagged_assignment = jagged_assign_list->left_child;
          int temp_high=declare_type.type_expression.array.j.range_R1[1];
          int temp_low=declare_type.type_expression.array.j.range_R1[0];
          declare_type.type_expression.array.j.range_R2 = (RangeR2Item*)malloc(sizeof(RangeR2Item)*(temp_high-temp_low+1));
          int range_r2_item_index=0;

          if(jagged_dimension->left_child->right_sibling->right_sibling != NULL)
          {
            declare_type.type_expression.array.j.dimension_count=3;
            //**TODO: PARSE 3d jagged array

            while(1)
            {
              ParseTreeNode* jagged_size=jagged_assignment->left_child;
              int temp_size;
              while(strcmp(jagged_size->symbol,"INTEGER_LITERAL")!=0)
              {
                jagged_size=jagged_size->right_sibling;
              }
              temp_size=strtol(jagged_size->lexeme,NULL,10);
              declare_type.type_expression.array.j.range_R2[range_r2_item_index].length=temp_size;
              declare_type.type_expression.array.j.range_R2[range_r2_item_index].ranges=(int*)malloc(sizeof(int)*temp_size);
              while(jagged_size->symbol!=NULL && strcmp(jagged_size->symbol,"<list_integer_list>")!=0)
              {
                jagged_size=jagged_size->right_sibling; //**pointing to list_integer_list in rhs of jagged_assignment**
              }
              if(jagged_size->symbol==NULL)
              {
                //**TODO: error for nothing between curly braces
                printf("Error: nothing between curly braces\n");
              }
              else
              {
                ParseTreeNode* integer_list = jagged_size->left_child; //**pointing to semicolon or integer list in the lhs of list_integer_list
                for(int int_list_index=0; int_list_index<temp_size; int_list_index++)
                {
                  //parsing list_integer_list

                  if(strcmp(integer_list->symbol, "SEMICOLON")==0)
                  {
                    declare_type.type_expression.array.j.range_R2[range_r2_item_index].ranges[int_list_index] = 0;
                    //**TODO: error for 3D JA size mismatch
                    if(integer_list->right_sibling !=NULL)
                    {
                      integer_list=integer_list->right_sibling->left_child;
                    }
                    else
                    {
                      break;
                    }

                  }
                  else
                  {

                    ParseTreeNode* integer = integer_list->left_child;
                    int temp_count = 0;
                    while(strcmp(integer->symbol, "INTEGER_LITERAL")==0)
                    {
                      temp_count++;
                      if(integer->right_sibling !=NULL)
                      {
                      integer = integer->right_sibling->left_child;
                      }
                      else
                      {
                        break;
                      }
                    }

                    declare_type.type_expression.array.j.range_R2[range_r2_item_index].ranges[int_list_index] = temp_count;

                    if(integer_list->right_sibling != NULL)
                    {
                      integer_list = integer_list-> right_sibling->right_sibling->left_child;
                    }
                    else if(int_list_index<temp_size-1)
                    {
                      printf("Error: too less number of integer lists\n");
                      //**TODO: Print error for too less number of integer lists
                      break;
                    }
                  }

                }
                if(integer_list->right_sibling !=NULL)
                {
                  printf("Error: too many integer lists\n");
                  //TODO: Error for having too many integer lists.
                }
              }
              range_r2_item_index++;
              if(jagged_assignment->right_sibling != NULL)
              {
                jagged_assignment=jagged_assignment->right_sibling->left_child;
              }
              else
              {
                break;
              }
            }
          }
          else
          {
            declare_type.type_expression.array.j.dimension_count=2;
            //**TODO: PARSE 2d jagged array

            while(1)
            {
              ParseTreeNode* jagged_size=jagged_assignment->left_child;
              int temp_size;
              while(strcmp(jagged_size->symbol,"INTEGER_LITERAL")!=0)
              {
                jagged_size=jagged_size->right_sibling;
              }
              temp_size=strtol(jagged_size->lexeme,NULL,10);
              declare_type.type_expression.array.j.range_R2[range_r2_item_index].length=temp_size;
              declare_type.type_expression.array.j.range_R2[range_r2_item_index].ranges=(int*)malloc(sizeof(int)*temp_size);
              while(jagged_size!=NULL && strcmp(jagged_size->symbol,"<list_integer_list>")!=0)
              {
                jagged_size=jagged_size->right_sibling; //**pointing to list_integer_list in rhs of jagged_assignment**
              }
              if(jagged_size==NULL)
              {
                printf("Error: nothing between curly braces\n");
                //**TODO: error for nothing between curly braces
              }
              else
              {
                ParseTreeNode* integer_list = jagged_size->left_child; //**pointing to semicolon or integer list in the lhs of list_integer_list
                for(int int_list_index=0; int_list_index<temp_size; int_list_index++)
                {
                  //parsing list_integer_list

                  if(strcmp(integer_list->symbol, "SEMICOLON")==0)
                  {
                    declare_type.type_expression.array.j.range_R2[range_r2_item_index].ranges[int_list_index] = 0;
                    //**TODO: error for 2D JA size mismatch
                    if(integer_list->right_sibling !=NULL)
                    {
                      integer_list=integer_list->right_sibling->left_child;
                    }
                    else
                    {
                      break;
                    }

                  }
                  else
                  {

                    ParseTreeNode* integer = integer_list->left_child;
                    int temp_count = 0;
                    while(strcmp(integer->symbol, "INTEGER_LITERAL")==0)
                    {
                      temp_count++;
                      if(integer->right_sibling !=NULL)
                      {
                      integer = integer->right_sibling->left_child;
                      }
                      else
                      {
                        break;
                      }
                    }
                    if(temp_count>1)
                    {
                      //TODO: type definition error for 2D jagged array
                      printf("Error: Type definition error for 2D jagged array.\n");
                    }
                    declare_type.type_expression.array.j.range_R2[range_r2_item_index].ranges[int_list_index] = 1;

                    if(integer_list->right_sibling != NULL)
                    {
                      integer_list = integer_list-> right_sibling->right_sibling->left_child;
                    }
                    else if(int_list_index<temp_size-1)
                    {
                      //**TODO: Print error for too less number of integer lists
                      printf("Error: too less number of integer lists\n");
                      break;
                    }
                  }

                }
                if(integer_list->right_sibling !=NULL)
                {
                  //TODO: Error for having too many integer lists.
                  printf("Error: too less number of integer lists\n");
                }
              }
              range_r2_item_index++;
              if(jagged_assignment->right_sibling != NULL)
              {
                jagged_assignment=jagged_assignment->right_sibling->left_child;
              }
              else
              {
                break;
              }
            }
          }
        }
      }
    }
    else if (strcmp(head->symbol,"<var_declaration>")==0)
    {
          if (strcmp(head->left_child->symbol,"VAR_ID")==0)
          {
            // Collect these variables
            E->T[E->size++].var_name = head->left_child->lexeme;

            // If we are out of space, we need to reallocate.
            if (E->size==E->capacity)
            {
              E->capacity *= 2;
              E->T = realloc(E->T,E->capacity*sizeof(TypeExpressionRecord));
            }
          }
          else if (strcmp(head->left_child->symbol,"<var_list>")==0)
          {
            // List of variables
            ParseTreeNode* var_list_node = head->left_child->left_child;
            while(var_list_node != NULL)
            {
              if(strcmp(var_list_node->symbol,"<var_id_list>")==0)
                break;
              var_list_node = var_list_node->right_sibling;
            }
            while(var_list_node != NULL)
            {
              var_list_node = var_list_node->left_child;

              if (strcmp(var_list_node->symbol,"VAR_ID")==0)
              {
                // Collect these variables
                E->T[E->size++].var_name = var_list_node->lexeme;

                // If we are out of space, we need to reallocate.
                if (E->size==E->capacity)
                {
                  E->capacity *= 2;
                  E->T = realloc(E->T,E->capacity*sizeof(TypeExpressionRecord));
                }
              }

              var_list_node = var_list_node->right_sibling;
            }
          }
        }
    head = head->right_sibling;
  }

  for(size_t i = incoming_size; i < E->size; i++)
  {
    char* name = E->T[i].var_name;
    E->T[i] = declare_type;
    E->T[i].var_name = name;
  }

  // Recurse on the next statement
  if (root->right_sibling != NULL)
    populateExpTable(root->right_sibling,E);
}

void validateArrayId(ParseTreeNode* array_node, TypeExpressionTable* E)
{
  TypeExpressionRecord lhs_record = getTypeExpressionRecord(E,array_node->left_child->lexeme);

  // Make sure index accesses are good
  ParseTreeNode* index_node = array_node->left_child->right_sibling->right_sibling;
  int dim_index = 0;
  int first_dim, second_dim, low;
  while(1)
  {
    index_node = index_node->left_child;
    if (strcmp(index_node->left_child->symbol,"INTEGER_LITERAL")==0)
    {
      int index_value = strtol(index_node->left_child->lexeme,NULL,10);

      if (lhs_record.type_expression.t==TYPE_RECTANGULAR_ARRAY)
      {
        // It is a rectangular array, access it as such
        if (dim_index >= lhs_record.type_expression.array.r.dimension_count)
        {
          // IndexOutOfBoundsAccess
          printf("**ERROR**\n");
          printf("Line number: %zu\n",index_node->left_child->line_number);
          printf("Statement type: Assignment\n");
          printf("Operator: Index access\n");
          printf("Depth: %zu\n",index_node->left_child->depth);
          printf("Attempting to access element from a dimension that is out of bounds\n");
        }

        else if (index_value < lhs_record.type_expression.array.r.lows[dim_index] || index_value > lhs_record.type_expression.array.r.highs[dim_index])
        {
          // IndexOutOfBoundsAccess
          printf("**ERROR**\n");
          printf("Line number: %zu\n",index_node->left_child->line_number);
          printf("Statement type: Assignment\n");
          printf("Operator: Index access\n");
          printf("Depth: %zu\n",index_node->left_child->depth);
          printf("Index access in dimension %d must be between %d and %d\n",dim_index+1,lhs_record.type_expression.array.r.lows[dim_index],lhs_record.type_expression.array.r.highs[dim_index]);
        }
      }
      else if (lhs_record.type_expression.t == TYPE_JAGGED_ARRAY)
      {
        if(dim_index==0)
        {
            first_dim=index_value;
            if(lhs_record.type_expression.array.j.range_R1[0]>index_value ||lhs_record.type_expression.array.j.range_R1[1]<index_value)
            {
              // IndexOutOfBoundsAccess
              printf("**ERROR**\n");
              printf("Line number: %zu\n",index_node->left_child->line_number);
              printf("Statement type: Assignment\n");
              printf("Operator: Index access\n");
              printf("Depth: %zu\n",index_node->left_child->depth);
              printf("Index access in dimension %d must be between %d and %d\n",dim_index+1,lhs_record.type_expression.array.j.range_R1[0],lhs_record.type_expression.array.j.range_R1[1]);
            }
        }
        else if(dim_index==1)
        {
          second_dim=index_value;
          low=lhs_record.type_expression.array.j.range_R1[0];
          if(index_value>=lhs_record.type_expression.array.j.range_R2[first_dim-low].length)
          {
            // IndexOutOfBoundsAccess
            printf("**ERROR**\n");
            printf("Line number: %zu\n",index_node->left_child->line_number);
            printf("Statement type: Assignment\n");
            printf("Operator: Index access\n");
            printf("Depth: %zu\n",index_node->left_child->depth);
            printf("Index access in dimension %d must be between 0 and %d\n",dim_index+1,lhs_record.type_expression.array.j.range_R2[first_dim-low].length);
          }
        }
        else
        {
          if(lhs_record.type_expression.array.j.dimension_count==2)
          {
            // IndexOutOfBoundsAccess
            printf("**ERROR**\n");
            printf("Line number: %zu\n",index_node->left_child->line_number);
            printf("Statement type: Assignment\n");
            printf("Operator: Index access\n");
            printf("Depth: %zu\n",index_node->left_child->depth);
            printf("Attempting to access 3rd dimension element in 2D jagged array\n");
          }
          else if(index_value>=lhs_record.type_expression.array.j.range_R2[first_dim-low].ranges[second_dim])
          {
            // IndexOutOfBoundsAccess
            printf("**ERROR**\n");
            printf("Line number: %zu\n",index_node->left_child->line_number);
            printf("Statement type: Assignment\n");
            printf("Operator: Index access\n");
            printf("Depth: %zu\n",index_node->left_child->depth);
            printf("Index access in dimension %d must be between 0 and %d\n",dim_index+1,lhs_record.type_expression.array.j.range_R2[first_dim-low].ranges[second_dim]);
          }
        }

        // TODO: It is a jagged array, access it as such
      }
    }
    else if (strcmp(index_node->left_child->symbol,"VAR_ID")==0)
    {
      TypeExpressionRecord index_record = getTypeExpressionRecord(E,index_node->left_child->lexeme);
      if (index_record.type_expression.t!=TYPE_INTEGER)
      {
        printf("**ERROR**\n");
        printf("Line Number: %zu\n",index_node->left_child->line_number);
        printf("Statement Type: Assignment\n");
        printf("Operator: Index access\n");
        //TODO: The desired output only makes sense in assignment
        //statements.
        printf("Depth:%zu\n",index_node->left_child->depth);
        printf("Array ranges must be an integer\n");
      }
    }
    if (index_node->right_sibling==NULL) // Reached the end of the index list
      break;

    index_node = index_node->right_sibling;
    dim_index++;
  }
}

void validateExpression(ParseTreeNode* expression_root, TypeExpressionTable* E)
{
  // 1. Booleans can only be operated on by AND and OR
  // 2. Operands must always be of the same type
  // 3. Divide gives a real
  if(strcmp(expression_root->symbol,"<arr_id>")==0)
  {
    validateArrayId(expression_root,E);
    expression_root->type_expression.t = TYPE_INTEGER;
  }
  else if (strcmp(expression_root->symbol,"INTEGER_LITERAL")==0)
  {
    expression_root->type_expression.t = TYPE_INTEGER;
  }
  else if (strcmp(expression_root->symbol,"VAR_ID")==0)
  {
    expression_root->type_expression = getTypeExpressionRecord(E,expression_root->lexeme).type_expression;
  }
  else
  {
    ParseTreeNode* term1 = expression_root->left_child;
    validateExpression(term1,E); // First term
    if(expression_root->left_child->right_sibling==NULL) // There is ony one term!
    {
      expression_root->type_expression = term1->type_expression;
      return;
    }
    ParseTreeNode* term2 = expression_root->left_child->right_sibling->right_sibling;
    validateExpression(term2,E); // Second term THIS MAY NOT EXIST. CHECK IF IT DOES!!

    char* operation = expression_root->left_child->right_sibling->left_child->symbol; // Operation

    // The terms should definitely have the same type
    if(!isTEEqual(term1->type_expression, term2->type_expression))
    {
      printf("**ERROR**\n");
      printf("Line Number: %zu\n",expression_root->line_number);
      printf("Statement type: Assignment\n");
      printf("Operator: %s\n",operation);
      // TODO: print type
      // TODO: print type
      printf("Depth: %zu\n",expression_root->depth);
      printf("Operands must be of the same type!\n");
    }

    // Combine based on operation
    if (strcmp(operation,"PLUS")==0 || strcmp(operation,"MINUS")==0 || strcmp(operation,"MULTIPLY")==0)
    {
      if (term1->type_expression.t == TYPE_BOOLEAN || term2->type_expression.t == TYPE_BOOLEAN)
      {
        printf("**ERROR**\n");
        printf("Line Number: %zu\n",expression_root->line_number);
        printf("Statement type: Assignment\n");
        printf("Operator: %s\n",operation);
        // TODO: print type
        printf("Depth: %zu\n",expression_root->depth);
        printf("This operation cannot be done on booleans!\n");
      }
      expression_root->type_expression = term1->type_expression;
    }
    else if (strcmp(operation,"DIVIDE")==0)
    {
      if (term1->type_expression.t == TYPE_BOOLEAN || term2->type_expression.t == TYPE_BOOLEAN)
      {
        printf("**ERROR**\n");
        printf("Line Number: %zu\n",expression_root->line_number);
        printf("Statement type: Assignment\n");
        printf("Operator: %s\n",operation);
        // TODO: print type
        printf("Depth: %zu\n",expression_root->depth);
        printf("This operation cannot be done on booleans!\n");
      }
      if (term1->type_expression.t == TYPE_RECTANGULAR_ARRAY || term2->type_expression.t == TYPE_RECTANGULAR_ARRAY)
      {
        printf("**ERROR**\n");
        printf("Line Number: %zu\n",expression_root->line_number);
        printf("Statement type: Assignment\n");
        printf("Operator: %s\n",operation);
        // TODO: print type
        printf("Depth: %zu\n",expression_root->depth);
        printf("This operation cannot be done on arrays!\n");
      }
      if (term1->type_expression.t == TYPE_JAGGED_ARRAY || term2->type_expression.t == TYPE_JAGGED_ARRAY)
      {
        printf("**ERROR**\n");
        printf("Line Number: %zu\n",expression_root->line_number);
        printf("Statement type: Assignment\n");
        printf("Operator: %s\n",operation);
        // TODO: print type
        printf("Depth: %zu\n",expression_root->depth);
        printf("This operation cannot be done on arrays!\n");
      }
      expression_root->type_expression.t = TYPE_REAL;
    }
    else if (strcmp(operation,"AND")==0 || strcmp(operation,"OR")==0)
    {
      if (term1->type_expression.t != TYPE_BOOLEAN || term2->type_expression.t != TYPE_BOOLEAN)
      {
        printf("**ERROR**\n");
        printf("Line Number: %zu\n",expression_root->line_number);
        printf("Statement type: Assignment\n");
        printf("Operator: %s\n",operation);
        // TODO: print type
        printf("Depth: %zu\n",expression_root->depth);
        printf("This operation must be done on booleans!\n");
      }
      expression_root->type_expression = term1->type_expression;
    }
  }
}

void validateParseTree(ParseTreeNode* root, TypeExpressionTable* E)
{
  // Sanity check
  if(strcmp(root->symbol,"<assignment_list>"))
    return;

  root = root->left_child; // Go to the assignment statement
  TypeExpressionRecord lhs_record;
  TypeExpressionRecord rhs_record;

  ParseTreeNode* assignment_node = root->left_child;
  while(assignment_node !=  NULL)
  {
    if (strcmp(assignment_node->symbol,"<assign_var>")==0)
    {
      // This is the LHS
      if(strcmp(assignment_node->left_child->symbol,"VAR_ID")==0)
      {
        // This is just a normal variable
        lhs_record = getTypeExpressionRecord(E,assignment_node->left_child->lexeme);
        assignment_node->type_expression = lhs_record.type_expression;
      }
      else if (strcmp(assignment_node->left_child->symbol,"<arr_id>")==0)
      {
        // This is an array **weep**
        ParseTreeNode* array_node = assignment_node->left_child;
        lhs_record.type_expression.t = TYPE_INTEGER;
        array_node->type_expression = lhs_record.type_expression;
        validateArrayId(array_node,E);
      }
    }
    else if (strcmp(assignment_node->symbol,"<expression>")==0)
    {
      // This is the RHS QwQ
      validateExpression(assignment_node,E);
      rhs_record.type_expression = assignment_node->type_expression;
    }

    assignment_node = assignment_node->right_sibling;
  }

  if (!isTEEqual(rhs_record.type_expression,lhs_record.type_expression))
  {
    printf("**ERROR**\n");
    printf("Line Number: %zu\n",root->line_number);
    printf("Statement type: Assignment\n");
    printf("Operator: =\n");
    // TODO: print type
    printf("Depth: %zu\n",root->depth);
    printf("Assignment must have same type on RHS and LHS\n");
  }

  if(root->right_sibling != NULL)
    validateParseTree(root->right_sibling,E);
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
