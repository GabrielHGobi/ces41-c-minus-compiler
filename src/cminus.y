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
program     : stmt_seq { } 
            ;
// 2. declaração-lista → declaração-lista declaração | declaração
stmt_seq    : stmt_seq stmt { }
            | stmt { }
            ;
// 3. declaração → var-declaração | fun-declaração
stmt        : var_stmt { }
            | func_stmt { }
            ;
// 4. var-declaração → tipo-especificador ID ; | tipo-especificador ID [ NUM ] ;
var_stmt    : spec_type ID SEMI { }
            | spec_type ID LBRACKET NUM RBRACKET SEMI { }
            ;
// 5. tipo-especificador → int | void
spec_type   : INT { }
            | VOID { }
            ;
// 6. fun-declaração → tipo-especificador ID ( params ) composto-decl
func_stmt   : spec_type ID LPAREN params RPAREN comp_stmt { }
            ;
// 7. params → param-lista | void
params      : params_seq { }
            | VOID { }
            ;
// 8. param-lista → param-lista, param | param
params_seq  : params_seq COMMA param { }
            | param { }
            ;
// 9. param → tipo-especificador ID | tipo-especificador ID [ ]
param       : spec_type ID { }
            | spec_type ID LBRACKET RBRACKET { }
            ;
// 10. composto-decl → { local-declarações statement-lista }
comp_stmt   : LBRACE local_stmt stmt_list RBRACE { }
            ;
// 11. local-declarações → local-declarações var-declaração | vazio
local_stmt  : local_stmt var_stmt { }
            | %empty { }
            ;
// 12. statement-lista → statement-lista statement | vazio
stmt_list   : stmt_list stmt_t { }
            | %empty { }
            ;
// 13. statement → expressão-decl | composto-decl | seleção-decl | iteração-decl | retorno-decl
stmt_t      : exp_stmt { }
            | comp_stmt { }
            | if_stmt { }
            | while_stmt { }
            | return_stmt { }
            | error  { $$ = NULL; }
            ;
// 14. expressão-decl → expressão ; | ;
exp_stmt    : exp SEMI { }
            | SEMI { }
            ;
// 15. seleção-decl → if ( expressão ) statement | if ( expressão ) statement else statement
if_stmt     : IF LPAREN exp RPAREN stmt_t { }
            | IF LPAREN exp RPAREN stmt_t ELSE stmt_t { }
            ;
// 16 . iteração-decl → while ( expressão ) statement
while_stmt  : WHILE LPAREN exp RPAREN stmt_t { }
            ;
// 17. retorno-decl → return ; | return expressão;
return_stmt : RETURN SEMI { }
            | RETURN exp SEMI { }
            ;
// 18. expressão → var = expressão | simples-expressão
exp         : var ASSIGN exp { }
            | simple_exp { }
            ;
// 19. var → ID | ID [ expressão ]
var         : ID { }
            | ID LBRACKET exp RBRACKET { }
            ;
// 20. simples-expressão → soma-expressão relacional soma-expressão | soma-expressão
simple_exp  : sum_exp relational sum_exp { }
            | sum_exp { }
            ;
// 21. relacional → <= | < | > | >= | == | !=
relational  : LTEQ { }
            | LT { }
            | GT { }
            | GTEQ { }
            | EQ { }
            | DIF { }
            ;
// 22. soma-expressão → soma-expressão soma termo | termo
sum_exp     : sum_exp sum term { }
            | term { }
            ;
// 23. soma → + | -
sum         : PLUS { }
            | MINUS { }
            ;
// 24. termo → termo mult fator | fator
term        : term mult factor { }
            | factor { }
            ;
// 25. mult → * | /             
mult        : TIMES { }
            | OVER { }
            ;
// 26. fator → ( expressão ) | var | ativação | NUM
factor      : LPAREN exp RPAREN { }
            | var { }
            | activation { }
            | NUM { }
            | error  { $$ = NULL; }
            ;
// 27. ativação → ID ( args )           
activation  : ID LPAREN args RPAREN { }
            ;
// 28. args → arg-lista | vazio 
args        : arg_seq { }
            | %empty { }
            ;
// 29. arg-lista → arg-lista , expressão | expressão 
arg_seq     : arg_seq COMMA exp { }
            | exp { }
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
