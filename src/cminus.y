/****************************************************/
/* File: cminus.y                                   */
/* The TINY Yacc/Bison specification file           */
/* Project for CES41: Compiladores                  */
/* Prof. Ana Carolina Lorena                        */
/* Code writted by Gabriel Gobi                     */
/*                 Samuel Vendramini                */
/*                 Thiago Lopes                     */
/****************************************************/
%{
#define YYPARSER /* distinguishes Yacc output from other code files */

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

#define YYSTYPE TreeNode *
static char * savedName; /* for use in assignments */ 
static int savedLineNo;  /* ditto */
static TreeNode * savedTree; /* stores syntax tree for later return */
static int yylex(void);
int yyerror(char *);

%}

%start program
%token IF ELSE INT RETURN VOID WHILE
%token ID NUM 
%token LT LTEQ GT GTEQ EQ DIF ASSIGN SEMI
       COMMA LBRACKET RBRACKET LBRACE RBRACE
%token PLUS MINUS
%token TIMES OVER
%token LPAREN RPAREN
%token ERROR 

%% /* Grammar for C- */

// 1. programa → declaração-lista
program     : stmt_seq 
                { savedTree = $1; } 
            ;
// 2. declaração-lista → declaração-lista declaração | declaração
stmt_seq    : stmt_seq stmt 
              { YYSTYPE t = $1;
                if ( t != NULL)
                { while (t->sibling != NULL)
                      t = t->sibling;
                  t->sibling = $2;
                  $$ = $1; }
                else $$ = $2;
              }
            | stmt { $$ = $1; }
            ;
// 3. declaração → var-declaração | fun-declaração
stmt        : var_stmt { $$ = $1; }
            | func_stmt { $$ = $1; }
            ;
// 4. var-declaração → tipo-especificador ID ; | tipo-especificador ID [ NUM ] ;
var_stmt    : spec_type ID 
                { $$ = $1;
                  $$->attr.id.name = copyString(idString);
                  $$->lineno = lineno; } 
              SEMI 
              { $$ = $3;
                $$->attr.id.type = Var; }
            | spec_type ID 
                { $$ = $1;
                  $$->attr.id.name = copyString(idString);
                  $$->lineno = lineno; } 
                LBRACKET NUM 
                { $$ = $3; 
                  $$->attr.id.type = Array;
                  $$->child[0] = newExpNode(ConstK);
                  $$->child[0]->attr.val = atoi(tokenString); }
                RBRACKET SEMI 
                { $$ = $6; }
            ;
// 5. tipo-especificador → int | void
spec_type   : INT { $$ = newExpNode(IdK);
                    $$->type.exp = Integer;
                    $$->type.name = copyString(tokenString); }
            | VOID { $$ = newExpNode(IdK);
                     $$->type.exp = Void;
                     $$->type.name = copyString(tokenString); }
            ;
// 6. fun-declaração → tipo-especificador ID ( params ) composto-decl
func_stmt   : spec_type ID
                  { $$ = $1;
                    $$->attr.id.name = copyString(idString);
                    $$->lineno = lineno; }           
              LPAREN params RPAREN comp_stmt
                { $$ = $3;
                  $$->attr.id.type = Fun;
                  $$->child[0] = $5;
                  $$->child[1] = $7;
                }
            ;
// 7. params → param-lista | void
params      : params_seq { $$ = $1; }
            | VOID { $$ = NULL; }
            ;
// 8. param-lista → param-lista, param | param
params_seq  : params_seq COMMA param 
                { YYSTYPE t = $1;
                if ( t != NULL)
                { while (t->sibling != NULL)
                      t = t->sibling;
                  t->sibling = $3;
                  $$ = $1; }
                else $$ = NULL;
                }
            | param { $$ = $1; }
            ;
// 9. param → tipo-especificador ID | tipo-especificador ID [ ]
param       : spec_type ID 
              { $$ = $1;
                $$->attr.id.name = copyString(idString);
                $$->attr.id.type = Var;
                $$->lineno = lineno;
              }
            | spec_type ID LBRACKET RBRACKET 
              { $$ = $1;
                $$->attr.id.name = copyString(idString);
                $$->attr.id.type = Array;
                $$->lineno = lineno;
              }
            ;
// 10. composto-decl → { local-declarações statement-lista }
comp_stmt   : LBRACE local_stmt stmt_list RBRACE
              { YYSTYPE t = $2;
                if ( t != NULL)
                { while (t->sibling != NULL)
                      t = t->sibling;
                  t->sibling = $3;
                  $$ = $2; }
                else $$ = $3;
              }
            ;
// 11. local-declarações → local-declarações var-declaração | vazio
local_stmt  : local_stmt var_stmt
              { YYSTYPE t = $1;
                if ( t != NULL)
                { while (t->sibling != NULL)
                      t = t->sibling;
                  t->sibling = $2;
                  $$ = $1; }
                else $$ = $2;
              }
            | %empty { $$ = NULL; }
            ;
// 12. statement-lista → statement-lista statement | vazio
stmt_list   : stmt_list stmt_t 
              { YYSTYPE t = $1;
                if ( t != NULL)
                { while (t->sibling != NULL)
                      t = t->sibling;
                  t->sibling = $2;
                  $$ = $1; }
                else $$ = $2;
              }
            | %empty { $$ = NULL; }
            ;
// 13. statement → expressão-decl | composto-decl | seleção-decl | iteração-decl | retorno-decl
stmt_t      : exp_stmt { $$ = $1; }
            | comp_stmt { $$ = $1; }
            | if_stmt { $$ = $1; }
            | while_stmt { $$ = $1; }
            | return_stmt { $$ = $1; }
            | error  { $$ = NULL; }
            ;
// 14. expressão-decl → expressão ; | ;
exp_stmt    : exp SEMI { $$ = $1; }
            | SEMI { $$ = NULL; }
            ;
// 15. seleção-decl → if ( expressão ) statement | if ( expressão ) statement else statement
if_stmt     : IF LPAREN exp RPAREN stmt_t 
              { $$ = newStmtNode(IfK);
                $$->child[0] = $3;
                $$->child[1] = $5;
              }
            | IF LPAREN exp RPAREN stmt_t ELSE stmt_t
              { $$ = newStmtNode(IfK);
                $$->child[0] = $3;
                $$->child[1] = $5;
                $$->child[2] = $7;
              }
            ;
// 16 . iteração-decl → while ( expressão ) statement
while_stmt  : WHILE LPAREN exp RPAREN stmt_t 
              { $$ = newStmtNode(WhileK);
                $$->child[0] = $3;
                $$->child[1] = $5;
              }
            ;
// 17. retorno-decl → return ; | return expressão;
return_stmt : RETURN SEMI 
              { $$ = newStmtNode(ReturnK); }
            | RETURN exp SEMI 
              { $$ = newStmtNode(ReturnK); 
                $$->child[0] = $2;
              }
            ;
// 18. expressão → var = expressão | simples-expressão
exp         : var ASSIGN exp 
              { $$ = newStmtNode(AssignK);
                $$->child[0] = $1;
                $$->child[1] = $3;
              }
            | simple_exp { $$ = $1; }
            ;
// 19. var → ID | ID [ expressão ]
var         : ID 
              { $$ = newExpNode(IdK);
                $$->attr.id.name = copyString(idString);
                $$->attr.id.type = Var;
                $$->lineno = lineno;
              }
            | ID  
              { $$ = newExpNode(IdK);
                $$->attr.id.name = copyString(idString);
                $$->lineno = lineno;
              }
              LBRACKET exp RBRACKET
              { 
                $$ = $2;
                $$->attr.id.type = Array;
                $$->child[0] = $4;
              }
            ;
// 20. simples-expressão → soma-expressão relacional soma-expressão | soma-expressão
simple_exp  : sum_exp relational sum_exp 
              { $$ = $2;
                $$->child[0] = $1;
                $$->child[1] = $3;
              }
            | sum_exp { $$ = $1; }
            ;
// 21. relacional → <= | < | > | >= | == | !=
relational  : LTEQ { $$ = newExpNode(OpK);
                     $$->attr.op = LTEQ; }
            | LT { $$ = newExpNode(OpK);
                   $$->attr.op = LT; }
            | GT { $$ = newExpNode(OpK);
                   $$->attr.op = GT; }
            | GTEQ { $$ = newExpNode(OpK);
                     $$->attr.op = GTEQ; }
            | EQ { $$ = newExpNode(OpK);
                   $$->attr.op = EQ; }
            | DIF { $$ = newExpNode(OpK);
                    $$->attr.op = DIF; }
            ;
// 22. soma-expressão → soma-expressão soma termo | termo
sum_exp     : sum_exp sum term 
              { $$ = $2;
                $$->child[0] = $1;
                $$->child[1] = $3;
              }
            | term { $$ = $1; }
            ;
// 23. soma → + | -
sum         : PLUS { $$ = newExpNode(OpK);
                     $$->attr.op = PLUS; }
            | MINUS { $$ = newExpNode(OpK);
                      $$->attr.op = MINUS; }
            ;
// 24. termo → termo mult fator | fator
term        : term mult factor 
              { $$ = $2;
                $$->child[0] = $1;
                $$->child[1] = $3;
              }
            | factor { $$ = $1; }
            ;
// 25. mult → * | /             
mult        : TIMES { $$ = newExpNode(OpK);
                      $$->attr.op = TIMES; } 
            | OVER { $$ = newExpNode(OpK);
                     $$->attr.op = OVER; }
            ;
// 26. fator → ( expressão ) | var | ativação | NUM
factor      : LPAREN exp RPAREN { $$ = $2; }
            | var { $$ = $1; }
            | activation { $$ = $1; }
            | NUM 
              { $$ = newExpNode(ConstK);
                $$->attr.val = atoi(tokenString);
              }
            | error  { $$ = NULL; }
            ;
// 27. ativação → ID ( args )           
activation  : ID { $$ = newExpNode(ActivK);
                   $$->attr.id.name = copyString(idString);
                   $$->lineno = lineno; }  
              LPAREN args RPAREN 
                { $$ = $2;
                  $$->attr.id.type = Fun;
                  $$->child[0] = $4;
                }
            ;
// 28. args → arg-lista | vazio 
args        : arg_seq { $$ = $1; }
            | %empty { $$ = NULL; }
            ;
// 29. arg-lista → arg-lista , expressão | expressão 
arg_seq     : arg_seq COMMA exp 
              { YYSTYPE t = $1;
                if ( t != NULL)
                { while (t->sibling != NULL)
                      t = t->sibling;
                  t->sibling = $3;
                  $$ = $1; }
                else $$ = NULL;
              }
            | exp { $$ = $1; }
            ;


%%

int yyerror(char * message)
{ fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
  fprintf(listing,"Current token: ");
  printToken(yychar,tokenString);
  Error = TRUE;
  return 0;
}

/* yylex calls getToken to make Yacc/Bison output
 * compatible with ealier versions of the TINY scanner
 */
static int yylex(void)
{ return getToken(); }

TreeNode * parse(void)
{ yyparse();
  return savedTree;
}

