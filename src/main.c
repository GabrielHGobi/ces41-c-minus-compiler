/****************************************************/
/* File: main.c                                     */
/* Main program for C- compiler                     */
/* Project for CES41: Compiladores                  */
/* Prof. Ana Carolina Lorena                        */
/* Code writted by Gabriel Gobi                     */
/*                 Samuel Vendramini                */
/*                 Thiago Lopes                     */
/****************************************************/

#include "globals.h"

/* set NO_PARSE to TRUE to get a scanner-only compiler */
#ifndef NO_PARSE
    #define NO_PARSE FALSE
#endif
/* set NO_ANALYZE to TRUE to get a parser-only compiler */
#ifndef NO_ANALYZE
    #define NO_ANALYZE FALSE
#endif
/* set NO_CODE to TRUE to get a compiler that does not generate code */
#ifndef NO_CODE
  #define NO_CODE FALSE
#endif

#include "util.h"
#if NO_PARSE
#include "scan.h"
#else
#include "parse.h"
#if !NO_ANALYZE
#include "analyze.h"
#if !NO_CODE
#include "cgen.h"
#endif
#endif
#endif

/* allocate global variables */
int lineno = 0;
FILE * source;
FILE * redundant_source;
FILE * listing;
FILE * code;

/* allocate and set tracing flags */
int EchoSource = TRUE;
int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int TraceCode = TRUE;

int Error = FALSE;

int main( int argc, char * argv[] )
{   
    TreeNode * syntaxTree;
    char pgm[120]; /* source code file name */
    if (argc != 2)
    { fprintf(stderr,"usage: %s <filename>\n",argv[0]);
      exit(1);
    }
    strcpy(pgm,argv[1]);
    if (strchr (pgm, '.') == NULL)
        strcat(pgm,".cm");
    source = fopen(pgm,"r");
    redundant_source = fopen(pgm, "r");
    if (source==NULL)
    { fprintf(stderr,"File %s not found\n",pgm);
        exit(1);
    }
    listing = stdout; /* send listing to screen */
    fprintf(listing,"\nC- COMPILATION: %s\n",pgm);
  #if NO_PARSE
    while (getToken()!=ENDFILE);
  #else
    syntaxTree = parse();
    if (TraceParse) {
        fprintf(listing,"\nSyntax tree:\n");
        printTree(syntaxTree);
    }
  #if !NO_ANALYZE
  if (! Error) {
    if (TraceAnalyze) fprintf(listing,"\nBuilding Symbol Table...\n");
    buildSymtab(syntaxTree);
    if (TraceAnalyze) fprintf(listing,"\nChecking for main...\n");
    mainCheck();
  } if (! Error) {
    if (TraceAnalyze) fprintf(listing,"\nChecking types...\n");
    typeCheck(syntaxTree);
    if (TraceAnalyze) fprintf(listing,"\nSemantic analysis finished\n");
  }
  #if !NO_CODE
    if (! Error)
    { char * codefile;
      int fnlen = strcspn(pgm,".");
      codefile = (char *) calloc(fnlen+5, sizeof(char));
      strncpy(codefile,pgm,fnlen);
      strcat(codefile,".cmm");
      code = fopen(codefile,"w");
      if (code == NULL)
      { printf("Unable to open %s\n",codefile);
        exit(1);
      }
      codeGen(syntaxTree,codefile);
      fclose(code);
    }
  #endif
  #endif
  #endif
    fclose(source);
    return 0;
}
