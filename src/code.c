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
static int branchLabelNumber = 0;

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

/* Procedure getNewBranchLabel returns a new label 
 * to a branch of code to build the intermediate code
 */
char * getNewBranchLabel( ) {
    branchLabelNumber++;
    char * branchLabelName;
    branchLabelName = malloc(12);
    char * labelNumberStr;
    labelNumberStr = malloc(11);
    labelNumberStr = intToString(branchLabelNumber);
    strcpy(branchLabelName,"L");
    strcat(branchLabelName,labelNumberStr);
    return branchLabelName;
}

/* Procedure emitComment prints a comment line 
 * with comment c in the code file
 */
void emitComment( char * c )
{ if (TraceCode) fprintf(code,"* %s\n",c); }

/* Function emitLabel emits a code label
 * before the intermediate code instruction
 * label = the label for the line of code
 */
void emitLabel( char * label) 
{   
    fflush(code);
    fprintf(code, "\n%10s:", label);
} /* emitLabel */

/* Procedure emitAssignInstruction emits a assign instruction
 * that can be le = ld or le = ld op ld2
 * op = the numeric operator
 * le = the left target variable
 * ld = 1st right source variable
 * ld2 = 2nd right source variable
 */
void emitAssignInstruction( char *op, char* le, char* ld, char* ld2) 
{   if (strlen(op) == 0 && strlen(ld2) == 0) {
        fprintf(code,"\r%*s%s = %s", SPACE, "", le, ld);
    } else {
        fprintf(code,"\r%*s%s = %s %s %s", SPACE, "", le, ld, op, ld2);
    }
} /* emitAssingInstruction */

/* Procedure emitBranchInstruction emits a branch instruction
 * that can be goto L or if_true x goto L
 * L = the target label
 * x = the test variable
 * checkTrue = TRUE, if the test is if_true or FALSE, if the test is if_false
 */
void emitBranchInstruction(char* x,  char * L, int checkTrue)
{   if (strlen(x) == 0) {
        fprintf(code,"\r%*sgoto %s", SPACE, "", L);
    } else {
        if(checkTrue)
            fprintf(code,"\r%*sif_true %s goto %s", SPACE, "", x, L);
        else
            fprintf(code,"\r%*sif_false %s goto %s", SPACE, "", x, L);
    }
} /* emitBranchInstruction */

/* Procedure emitReturnInstruction emits a return instruction
 * that is return v
 * v = the variable to be returned by the subroutine
 */
void emitReturnInstruction( char* v)
{
    fprintf(code,"\r%*sreturn %s", SPACE, "", v);
}

/* Procedure emitParamInstruction emits a param instruction
 * that is param x
 * x = the variable to be used as a param to subroutine activation
 */
void emitParamInstruction( char* x )
{
    fprintf(code,"\r%*sparam %s", SPACE, "", x);
}

/* Procedure emitCallInstruction emits a activation instruction
 * that is x = call f, n when the subroutine has a return value
 * or call f, n when it doesn't
 * x = the target variable to the return of the function
 * f = the name of the function
 * n = the number of parameters of the function
 */
void emitCallInstruction( char* x, char *f, int n)
{
    fprintf(code,"\r%*s%s = call %s, %d", SPACE, "", x, f, n);
}

/* Procedure emitArrayAccessInstruction emits array access instructions
 * that are t_temp = i * n and t = v[t_temp]
 * t = the target variable to store the memory offset
 * i = the index of the array member
 * n = the byte size of the type of the array members
 */
void emitArrayAccessInstruction( char* t, char* i, int n)
{
    fprintf(code,"\r%*s%s = %s * %d", SPACE, "", t, i, n);
}

/* Procedure emitHaltInstruction prints a halt instruction 
 * in the code file
 */
void emitHaltInstruction( void )
{
    fprintf(code,"\r%*shalt", SPACE, "");
}