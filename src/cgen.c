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

/* tmpOffset is the memory offset for temps
   It is decremented each time a temp is
   stored, and incremeted when loaded again
*/
static int tmpOffset = 0;

/* prototype for internal recursive intermediate code generator */
static void cGen (TreeNode * tree);

/* prototype for recursive expression intermediate code generator */
char * genExp (TreeNode * tree);

/* Procedure genStmt generates code at a statement node */
static void genStmt( TreeNode * tree) { 
  TreeNode * p1, * p2, * p3;
  char * t1, * t2, * t3; 
  int savedLoc1,savedLoc2,currentLoc;
  switch (tree->kind.stmt) {
         
         case DeclK :
            if (tree->attr.id.type == Fun) {
                p1 = tree->child[0];
                p2 = tree->child[1];
                /* do nothing for p1 */
                cGen(p2);
            } 
            break; /* decl_k */

//       case IfK :
//          if (TraceCode) emitComment("-> if") ;
//          p1 = tree->child[0] ;
//          p2 = tree->child[1] ;
//          p3 = tree->child[2] ;
//          /* generate code for test expression */
//          cGen(p1);
//          savedLoc1 = emitSkip(1) ;
//          emitComment("if: jump to else belongs here");
//          /* recurse on then part */
//          cGen(p2);
//          savedLoc2 = emitSkip(1) ;
//          emitComment("if: jump to end belongs here");
//          currentLoc = emitSkip(0) ;
//          emitBackup(savedLoc1) ;
//          emitRM_Abs("JEQ",ac,currentLoc,"if: jmp to else");
//          emitRestore() ;
//          /* recurse on else part */
//          cGen(p3);
//          currentLoc = emitSkip(0) ;
//          emitBackup(savedLoc2) ;
//          emitRM_Abs("LDA",pc,currentLoc,"jmp to end") ;
//          emitRestore() ;
//          if (TraceCode)  emitComment("<- if") ;
//          break; /* if_k */

//       case RepeatK:
//          if (TraceCode) emitComment("-> repeat") ;
//          p1 = tree->child[0] ;
//          p2 = tree->child[1] ;
//          savedLoc1 = emitSkip(0);
//          emitComment("repeat: jump after body comes back here");
//          /* generate code for body */
//          cGen(p1);
//          /* generate code for test */
//          cGen(p2);
//          emitRM_Abs("JEQ",ac,savedLoc1,"repeat: jmp back to body");
//          if (TraceCode)  emitComment("<- repeat") ;
//          break; /* repeat */

      case AssignK:
         /* generate code for left and right operands */
         t1 = genExp(tree->child[0]);
         t2 = genExp(tree->child[1]);
         emitAssignInstruction("", t1, t2, "", "assign: store value");
         break; /* assign_k */

//       case ReadK:
//          emitRO("IN",ac,0,0,"read integer value");
//          loc = st_lookup(tree->attr.name);
//          emitRM("ST",ac,loc,gp,"read: store value");
//          break;
//       case WriteK:
//          /* generate code for expression to write */
//          cGen(tree->child[0]);
//          /* now output it */
//          emitRO("OUT",ac,0,0,"write ac");
//          break;
      default:
         break;
    }
} /* genStmt */

/* Procedure genExp generates code at an expression node */
char * genExp( TreeNode * tree) { 
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
      }
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
                emitAssignInstruction("+", t3, t1, t2, "op +");
                return t3;
                break;
            // case MINUS :
            //    emitRO("SUB",ac,ac1,ac,"op -");
            //    break;
            // case TIMES :
            //    emitRO("MUL",ac,ac1,ac,"op *");
            //    break;
            // case OVER :
            //    emitRO("DIV",ac,ac1,ac,"op /");
            //    break;
            // case LT :
            //    emitRO("SUB",ac,ac1,ac,"op <") ;
            //    emitRM("JLT",ac,2,pc,"br if true") ;
            //    emitRM("LDC",ac,0,ac,"false case") ;
            //    emitRM("LDA",pc,1,pc,"unconditional jmp") ;
            //    emitRM("LDC",ac,1,ac,"true case") ;
            //    break;
            // case EQ :
            //    emitRO("SUB",ac,ac1,ac,"op ==") ;
            //    emitRM("JEQ",ac,2,pc,"br if true");
            //    emitRM("LDC",ac,0,ac,"false case") ;
            //    emitRM("LDA",pc,1,pc,"unconditional jmp") ;
            //    emitRM("LDC",ac,1,ac,"true case") ;
            //    break;
            default:
               emitComment("BUG: Unknown operator");
               break;
         } /* case op */
         break; /* OpK */

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
   emitComment("End of execution.");
}
