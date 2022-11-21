/****************************************************/
/* File: analyze.c                                  */
/* Semantic analyzer implementation                 */
/* for the C- compiler                              */
/* Project for CES41: Compiladores                  */
/* Prof. Ana Carolina Lorena                        */
/* Code writted by Gabriel Gobi                     */
/*                 Samuel Vendramini                */
/*                 Thiago Lopes                     */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include "util.h"

static char * currentScope;
static char * previousScope;

/* Procedure traverse is a generic recursive 
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc 
 * in postorder to tree pointed to by t
 */
static void traverse( TreeNode * t,
               void (* preProc) (TreeNode *),
               void (* postProc) (TreeNode *) )
{ if (t != NULL)
  { preProc(t);
    { int i;
      for (i=0; i < MAXCHILDREN; i++)
        traverse(t->child[i],preProc,postProc);
    }
    postProc(t);
    traverse(t->sibling,preProc,postProc);
  }
}

/* nullProc is a do-nothing procedure to 
 * generate preorder-only or postorder-only
 * traversals from traverse
 */
static void nullProc(TreeNode * t)
{ if (t==NULL) return;
  else return;
}

/* Errors detected by insertNode */

static void notDeclaredError(TreeNode * t)
{ fprintf(listing,"Semantic error at line %d: '%s' was not declared in this scope\n",t->lineno, t->attr.id.name);
  Error = TRUE;
}

static void varAlreadyDeclaredError(TreeNode * t)
{ fprintf(listing,"Semantic error at line %d: '%s' was already declared as a variable\n",t->lineno, t->attr.id.name);
  Error = TRUE;
}

static void funAlreadyDeclaredError(TreeNode * t)
{ fprintf(listing,"Semantic error at line %d: '%s' was already declared as a function\n",t->lineno, t->attr.id.name);
  Error = TRUE;
}

/* Procedure insertNode inserts identifiers stored in t into 
 * the symbol table and do some semantical checks
 */
static void insertNode( TreeNode * t)
{ switch (t->nodekind)
  { case StmtK:
      switch (t->kind.stmt)
      { case DeclK: ;
          BucketList st_entry = st_lookup(t->attr.id.name);
          if (st_entry == NULL) {
            st_insert(t->attr.id.name, currentScope, t->attr.id.type, t->type, t->lineno); 
            if (t->attr.id.type == Fun){
              previousScope = copyString(currentScope);
              currentScope = copyString(t->attr.id.name);
            }
          }
          else {
            if (st_entry->idType == Var || st_entry->idType == Array) {
              if ((strcmp(st_entry->scope,currentScope) == 0))
                varAlreadyDeclaredError(t);
              else
                st_insert(t->attr.id.name, currentScope, t->attr.id.type, t->type, t->lineno);
            }
            else if (st_entry->idType == Fun) {
              funAlreadyDeclaredError(t);
            } else
              st_insert(t->attr.id.name, currentScope, t->attr.id.type, t->type, t->lineno);
          }
          break;
        default:
          break;
      }
      break;

    case ExpK:
      switch (t->kind.exp)
      { case IdK:
        case ActivK: ;
          // Must check the scope for the Activ and Id
          BucketList st_entry = st_lookup(t->attr.id.name);
          if (st_entry == NULL) {
            notDeclaredError(t);
          }
          else {
            // It's in the same scope
            if (st_entry->scope == currentScope) {
              st_insert(t->attr.id.name, currentScope, t->attr.id.type, t->type, t->lineno);
            }
            else {
              BucketList l = st_entry;
              do {
                // It's in a global scope
                if ( (l != NULL) && (strcmp(l->scope, "") == 0) ){
                  st_insert(t->attr.id.name, "", t->attr.id.type, t->type, t->lineno);
                  break;
                }
                l = l->next;
              } while ((l != NULL) && (strcmp(t->attr.id.name,l->name) == 0));
              // The variable declaration exists in another local scope
              if ( (st_entry == NULL) ||  (strcmp(t->attr.id.name,st_entry->name) != 0) ){
                notDeclaredError(t);
              }
            }
          }
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

/* Function changeScope updates the scope to the previous 
 * when exiting some scope when traversing is postorder
 */
static void changeScope(TreeNode * t) {
  if (t == NULL) return;
  if (t->nodekind == StmtK && t->kind.stmt == DeclK && t->attr.id.type == Fun) {
    currentScope = copyString(previousScope);
  }
  return;
}

/* Function buildSymtab constructs the symbol 
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode * syntaxTree)
{ /* Inserting manually pre-built functions input and output */
  currentScope = copyString(""); 
  TreeNode * inputOutputFunNode = newStmtNode(DeclK);
  inputOutputFunNode->attr.id.type = Fun;
  inputOutputFunNode->lineno = -1;
  inputOutputFunNode->attr.id.name = copyString("input");
  inputOutputFunNode->type = Integer;
  insertNode(inputOutputFunNode);
  currentScope = copyString(""); 
  inputOutputFunNode->attr.id.name = copyString("output");
  inputOutputFunNode->type = Void;
  insertNode(inputOutputFunNode);
  currentScope = copyString(""); 
  /* Traversing the sintax tree */
  traverse(syntaxTree,insertNode,changeScope);
  if (TraceAnalyze)
  { fprintf(listing,"\nSymbol table:\n\n");
    printSymTab(listing);
  }
}

/* Errors detected by checkNode */

static void typeError(TreeNode * t, char * message)
{ fprintf(listing,"Semantic error at line %d: %s\n", t->lineno, message);
  Error = TRUE;
}

static void invalidVoidError(TreeNode * t)
{ fprintf(listing,"Semantic error at line %d: invalid use of void expression\n",t->lineno);
  Error = TRUE;
}

/* Procedure checkNode performs
 * type checking at a single tree node
 */
static void checkNode(TreeNode * t)
{  switch (t->nodekind)
  { case ExpK:
      switch (t->kind.exp)
      { case OpK:
          if ((t->child[0]->type != Integer) ||
              (t->child[1]->type != Integer))
            typeError(t,"Op applied to non-integer");
          if ((t->attr.op == EQ) || (t->attr.op == LT) || (t->attr.op == LTEQ) || 
              (t->attr.op == GT) || (t->attr.op == GTEQ) || (t->attr.op == DIF))
            t->type = Boolean;
          else
            t->type = Integer;
          break;
        case ConstK:
          t->type = Integer;
          break;
        case IdK:
          if (t->attr.id.type == Var || t->attr.id.type == Array)
            t->type = Integer;
          break;
        default:
          break;
      }
      break;
    case StmtK:
      switch (t->kind.stmt)
      { case DeclK:
          if (t->type == Void && t->attr.id.type != Fun)
            typeError(t, "variable declared void");
          break;
        case IfK:
          if (t->child[0]->type != Boolean)
            typeError(t->child[0],"if test is not Boolean");
          break;
        case AssignK:
          if (t->child[1]->kind.exp == ActivK) {
            BucketList st_entry = st_lookup(t->child[1]->attr.id.name);
            if (st_entry->dataType == Void)
              invalidVoidError(t);
          } else if (t->child[1]->type != Integer)
            typeError(t->child[1],"assignment of non-integer value");
          break;
        case WhileK:
          if (t->child[0]->type != Boolean)
            typeError(t->child[0],"while test is not Boolean");
          break;
        default:
          break;
      }
      break;
    default:
      break;

  } 
}

/* Procedure typeCheck performs type checking 
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode * syntaxTree)
{ traverse(syntaxTree,nullProc,checkNode);
}

/* Procedure mainCheck just search the symbol
 * table looking for main declaration
 */
void mainCheck()
{ BucketList st_entry = st_lookup("main");
  if ( st_entry != NULL )
    if ( st_entry->dataType == Void ) return;
  fprintf(listing,"Semantic error: undefined reference to 'main'\n");
  Error = TRUE;
}