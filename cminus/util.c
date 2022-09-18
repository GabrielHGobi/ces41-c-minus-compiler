/****************************************************/
/* File: util.c                                     */
/* Utility function implementation for C- compiler  */
/* Project for CES41: Compiladores                  */
/* Prof. Ana Carolina Lorena                        */
/* Code writted by Gabriel Gobi                     */
/*                 Samuel Vendramini                */
/*                 Thiago Lopes                     */
/****************************************************/

#include "globals.h"
#include "util.h"

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
void printToken( TokenType token, const char* tokenString )
{ switch (token)
  { case IF:
    case ELSE:
    case INT:
    case RETURN:
    case VOID:
    case WHILE:
      fprintf(listing,
         "reserved word: %s\n",tokenString);
      break;
    case PLUS: fprintf(listing,"+\n"); break;
    case MINUS: fprintf(listing,"-\n"); break;
    case TIMES: fprintf(listing,"*\n"); break;
    case OVER: fprintf(listing,"/\n"); break;
    case LT: fprintf(listing,"<\n"); break;
    case LTEQ: fprintf(listing,"<\n"); break;
    case GT: fprintf(listing,">\n"); break;
    case GTEQ: fprintf(listing,">=\n"); break;
    case EQ: fprintf(listing,"==\n"); break;
    case DIF: fprintf(listing,"!=\n"); break;
    case ASSIGN: fprintf(listing,"=\n"); break;
    case SEMI: fprintf(listing,";\n"); break;
    case COMMA: fprintf(listing,",\n"); break;
    case LPAREN: fprintf(listing,"(\n"); break;
    case RPAREN: fprintf(listing,")\n"); break;
    case LBRACKET: fprintf(listing,"[\n"); break;
    case RBRACKET: fprintf(listing,"]\n"); break;
    case LBRACE: fprintf(listing,"{\n"); break;
    case RBRACE: fprintf(listing,"}\n"); break;
    case ENDFILE: fprintf(listing,"EOF\n"); break;
    case NUM:
      fprintf(listing,
          "NUM, val= %s\n",tokenString);
      break;
    case ID:
      fprintf(listing,
          "ID, name= %s\n",tokenString);
      break;
    case ERROR:
      fprintf(listing,
          "ERROR: %s\n",tokenString);
      break;
    default: /* should never happen */
      fprintf(listing,"Unknown token: %d\n",token);
  }
}