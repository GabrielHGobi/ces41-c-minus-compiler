/****************************************************/
/* File: code.c                                     */
/* Intermediate code emitting utilities             */
/* implementation for the C- compiler               */
/* Prof. Ana Carolina Lorena                        */
/* Code writted by Gabriel Gobi                     */
/*                 Samuel Vendramini                */
/*                 Thiago Lopes                     */
/****************************************************/

#include <stdio.h>
#include "globals.h"
#include "code.h"
#include "util.h"

/* TM location number for current instruction emission */
static int variableNumber = 0 ;

const int SPACE = 10;

/* Procedure getNewVariable returns a new temporary 
 * variable to build the intermediate code
 */
char * getNewVariable( ) {
    variableNumber++;
    char * variableName;
    variableName = malloc(12);
    char * varNumberStr;
    varNumberStr = malloc(11);
    varNumberStr = intToString(variableNumber);
    strcpy(variableName,"t");
    strcat(variableName,varNumberStr);
    return variableName;
}

/* Procedure emitComment prints a comment line 
 * with comment c in the code file
 */
void emitComment( char * c )
{ if (TraceCode) fprintf(code,"* %s\n",c); }

/* Procedure emitAssignInstruction emits a assign instruction
 * that can be le = ld or le = ld op ld2
 * op = the numeric operator
 * le = the left target variable
 * ld = 1st right source variable
 * ld2 = 2nd right source variable
 * c = a comment to be printed if TraceCode is TRUE
 */
void emitAssignInstruction( char *op, char* le, char* ld, char* ld2, char *c) 
{   if (strlen(op) == 0 && strlen(ld2) == 0) {
        fprintf(code,"\r%*s%s = %s", SPACE, "", le, ld);
        if (TraceCode) fprintf(code,"\t%s",c) ;
    } else {
        fprintf(code,"\r%*s%s = %s %s %s", SPACE, "", le, ld, op, ld2);
        if (TraceCode) fprintf(code,"\t%s",c) ;
    }
} /* emitAssingInstruction */

/* Function emitLabel emits a code label
 * before the intermediate code instruction
 * label = the label for the line of code
 * c = a comment to be printed if TraceCode is TRUE
 */
void emitLabel( char * label, char *c) 
{   
    fflush(code);
    fprintf(code, "\n%10s:", label);
    if (TraceCode) fprintf(code,"\t%s",c) ;
} /* emitLabel */


