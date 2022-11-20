/****************************************************/
/* File: cgen.c                                     */
/* The intermediate code generator implementation   */
/* for the C- compiler                              */
/* Prof. Ana Carolina Lorena                        */
/* Code writted by Gabriel Gobi                     */
/*                 Samuel Vendramini                */
/*                 Thiago Lopes                     */
/****************************************************/

#include "globals.h"
#include "util.h"
#include "symtab.h"
#include "code.h"
#include "cgen.h"

/* size of the int variable in bytes for array access */
const int INT_BYTE_SIZE = 4;

/* prototype for internal recursive intermediate code generator */
static void cGen (TreeNode * tree);

/* prototype for recursive expression intermediate code generator */
char * genExp (TreeNode * tree);

/* Procedure genStmt generates code at a statement node */
static void genStmt( TreeNode * tree) { 
  TreeNode * p1, * p2, * p3;
  char * t1, * t2, * t3;
  char * l1, *l2;
  int savedLoc1,savedLoc2,currentLoc;
  switch (tree->kind.stmt) {
         
         case DeclK :
            if (tree->attr.id.type == Fun) {
                emitLabel(tree->attr.id.name);
                p1 = tree->child[0];
                p2 = tree->child[1];
                /* do nothing for p1 */
                cGen(p2);
            } 
            break; /* decl_k */

         case IfK :
            p1 = tree->child[0];
            p2 = tree->child[1];
            p3 = tree->child[2];
            /* generate code for test expression */
            t1 = genExp(tree->child[0]);
            /* branch labels for true and false blocks */
            l1 = getNewBranchLabel();
            l2 = getNewBranchLabel();
            /* branch instruction */
            emitBranchInstruction(t1, l1, TRUE);
            if (p3 != NULL)
               cGen(p3);
            emitBranchInstruction("", l2, -1);
            emitLabel(l1);
            cGen(p2);
            emitLabel(l2);
            break; /* if_k */

         case WhileK:
            p1 = tree->child[0];
            p2 = tree->child[1];
            /* branch labels for repeat and next instruction blocks */
            l1 = getNewBranchLabel();
            l2 = getNewBranchLabel();
            /* repeat block*/
            emitLabel(l1);
            /* generate code for test expression */
            t1 = genExp(p1);
            /* branch instruction */
            emitBranchInstruction(t1, l2, FALSE);
            cGen(p2);
            emitBranchInstruction("", l1, -1);
            /* next instruction block */
            emitLabel(l2);
            break; /* while_k */

      case AssignK:
         p1 = tree->child[0];
         p2 = tree->child[1];
         /* generate code for left and right operands */
         t1 = genExp(p1);
         t2 = genExp(p2);
         emitAssignInstruction("", t1, t2, "");
         break; /* assign_k */
        
      case ReturnK :
         p1 = tree->child[0];
         if (p1 != NULL) {
            t1 = genExp(p1);
            emitReturnInstruction(t1);
         }
         break; /* decl_k */

      default:
         break;
    }
} /* genStmt */

/* Procedure genExp generates code at an expression node */
char * genExp( TreeNode * tree) { 
  int countParams;
  TreeNode * p1, * p2;
  char * t1, * t2, * t3;
  switch (tree->kind.exp) {

    case ConstK : ;
        char *val = intToString(tree->attr.val);
        return val;
      break; /* ConstK */
    
    case IdK :
      if (tree->attr.id.type == Var){
        char *id = copyString(tree->attr.id.name);
        return id;
      } /* IdK Var */
      else if (tree->attr.id.type == Array){
        p1 = tree->child[0];
        char* index = genExp(p1);
        t1 = getNewVariable();
        emitArrayAccessInstruction(t1, index, INT_BYTE_SIZE);
        char* arrayAccess;
        arrayAccess = malloc(strlen(tree->attr.id.name) + 3 + strlen(t1));
        strcat(arrayAccess, tree->attr.id.name);
        strcat(arrayAccess, "[");
        strcat(arrayAccess, t1);
        strcat(arrayAccess, "]");
        return arrayAccess;
      } /* IdK Array */
      break; /* IdK */

    case OpK :
         p1 = tree->child[0];
         p2 = tree->child[1];
         /* gen code for left operand */
         t1 = genExp(p1);
         /* gen code for right operand */
         t2 = genExp(p2);
         /* target variable */
         t3 = getNewVariable();
         switch (tree->attr.op) {
            case PLUS :
                emitAssignInstruction("+", t3, t1, t2);
                return t3;
                break;
            case MINUS :
               emitAssignInstruction("-", t3, t1, t2);
               return t3;
               break;
            case TIMES :
               emitAssignInstruction("*", t3, t1, t2);
               return t3;
               break;
            case OVER :
               emitAssignInstruction("/", t3, t1, t2);
               return t3;
               break;
            case LT :
               emitAssignInstruction("<", t3, t1, t2);
               return t3;
               break;
            case LTEQ :
               emitAssignInstruction("<=", t3, t1, t2);
               return t3;
               break;
            case GT :
               emitAssignInstruction(">", t3, t1, t2);
               return t3;
               break;
            case GTEQ :
               emitAssignInstruction(">=", t3, t1, t2);
               return t3;
               break;
            case DIF :
               emitAssignInstruction("!=", t3, t1, t2);
               return t3;
               break;
            case EQ :
               emitAssignInstruction("==", t3, t1, t2);
               return t3;
               break;
            default:
               emitComment("BUG: Unknown operator");
               break;
         } /* case op */
         break; /* OpK */

    case ActivK : 
        // TODO: implement subroutine invocation code gen
        p1 = tree->child[0];
        countParams = 0;
        while( p1 != NULL ){
            countParams++;
            t1 = genExp(p1);
            emitParamInstruction(t1);
            p1 = p1->sibling;
        }
        t1 = getNewVariable();
        emitCallInstruction(t1, tree->attr.id.name, countParams);
        return t1;
        break; /* ActivK */

    default:
      break;
  } 
} /* genExp */

/* Procedure cGen recursively generates code by
 * tree traversal
 */
static void cGen( TreeNode * tree)
{ if (tree != NULL)
  { switch (tree->nodekind) {
      case StmtK:
        genStmt(tree);
        break;
      case ExpK:
        genExp(tree);
        break;
      default:
        break;
    }
    cGen(tree->sibling);
  }
}

/***********************************************************/
/* the primary function of the intermediate code generator */
/***********************************************************/
/* Procedure codeGen generates three address intermediate
 * code to a code file by traversal of the syntax tree. 
 * The second parameter (codefile) is the file name
 * of the code file, and is used to print the
 * file name as a comment in the code file
 */
void codeGen(TreeNode * syntaxTree, char * codefile)
{  char * s = malloc(strlen(codefile)+7);
   strcpy(s,"File: ");
   strcat(s,codefile);
   emitComment("C- Compilation to 3 Address Intermediate Code");
   emitComment(s);
   /* generate intermediate code for C- program */
   cGen(syntaxTree);
   /* finish */
   emitHaltInstruction();
   fprintf(code, "\n\r");
   emitComment("End of execution.");
}
