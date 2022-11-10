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

/* counter for variable memory locations */
static int location = 0;

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

/* Procedure insertNode inserts 
 * identifiers stored in t into 
 * the symbol table 
 */
static void insertNode( TreeNode * t)
{ switch (t->nodekind)
  { case StmtK:
      switch (t->kind.stmt)
      { case AssignK:
          //
          break;
        default:
          break;
      }
      break;
    case ExpK:
      switch (t->kind.exp)
      { case IdK:
        case ActivK:
          if (st_lookup(t->attr.id.name) == -1)
          /* not yet in table, so treat as new definition */
            st_insert(t->attr.id.name, "", 0, t->attr.id.type, 
                        t->type, t->lineno,location++);
          else
          /* already in table, so ignore location, 
             add line number of use only */ 
            st_insert(t->attr.id.name, "", -1, -1, -1, t->lineno, 0);
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

/* Function buildSymtab constructs the symbol 
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode * syntaxTree)
{ /* Inserting manually pre-built functions input and output */
  TreeNode * inputOutputFunNode = newExpNode(IdK);
  inputOutputFunNode->attr.id.type = Fun;
  inputOutputFunNode->lineno = -1;
  inputOutputFunNode->attr.id.name = copyString("input");
  inputOutputFunNode->type = Integer;
  insertNode(inputOutputFunNode);
  inputOutputFunNode->attr.id.name = copyString("output");
  inputOutputFunNode->type = Void;
  insertNode(inputOutputFunNode);
  /* Traversomg the sintax tree */
  traverse(syntaxTree,insertNode,nullProc);
  if (TraceAnalyze)
  { fprintf(listing,"\nSymbol table:\n\n");
    printSymTab(listing);
  }
}

static void typeError(TreeNode * t, char * message)
{ fprintf(listing,"Type error at line %d: %s\n",t->lineno,message);
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
          else if (t->attr.id.type == Fun){
            // BucketList l = hashTable[i];
          }
          break;
        default:
          break;
      }
      break;
    case StmtK:
      switch (t->kind.stmt)
      { case IfK:
          if (t->child[0]->type != Boolean)
            typeError(t->child[0],"if test is not Boolean");
          break;
        case AssignK:
          if (t->child[1]->type != Integer)
            typeError(t->child[1],"assignment of non-integer value");
          break;
        case WhileK:
          if (t->child[0]->type != Boolean)
            typeError(t->child[0],"while test is not Boolean");
          break;
        // case ReturnK:
        //   if (t->child[1]->type == Integer)
        //     typeError(t->child[1],"repeat test is not Boolean");
        //   break;
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