/****************************************************/
/* File: code.h                                     */
/* Intermediate code emitting utilities for         */
/* the C- compiler                                  */
/* Project for CES41: Compiladores                  */
/* Prof. Ana Carolina Lorena                        */
/* Code writted by Gabriel Gobi                     */
/*                 Samuel Vendramini                */
/*                 Thiago Lopes                     */
/****************************************************/

#ifndef _CODE_H_
#define _CODE_H_

/* intermediate code emitting utilities */

/* Procedure getNewVariable returns a new temporary 
 * variable to build the intermediate code
 */
char* getNewVariable( void );

/* Procedure emitComment prints a comment line 
 * with comment c in the code file
 */
void emitComment( char * c );

/* Procedure emitAssignInstruction emits a assign instruction
 * that can be le = ld or le = ld op ld2
 * op = the numeric operator
 * le = the left target variable
 * ld = 1st right source variable
 * ld2 = 2nd right source variable
 * c = a comment to be printed if TraceCode is TRUE
 */
void emitAssignInstruction( char *op, char* le, char* ld, char* ld2, char *c);

/* Procedure emitBranchInstruction emits a branch instruction
 * that can be goto L or if_true x op y goto L
 * L = the target label
 * op = the relational operator
 * x = 1st right source variable
 * y = 2nd right source variable
 * c = a comment to be printed if TraceCode is TRUE
 */
void emitBranchInstruction( char * L, char* op, char* x, char* y, char *c);

/* Function emitLabel emits a code label
 * before the intermediate code instruction
 * label = the label for the line of code
 * c = a comment to be printed if TraceCode is TRUE
 */
void emitLabel( char * label, char *c);

/* Procedure emitReturnInstruction emits a return instruction
 * that is return v
 * v = the variable to be returned by the subroutine
 * c = a comment to be printed if TraceCode is TRUE
 */
void emitReturnInstruction( char* v, char *c );

/* Procedure emitParamInstruction emits a param instruction
 * that is param x
 * x = the variable to be used as a param to subroutine activation
 * c = a comment to be printed if TraceCode is TRUE
 */
void emitParamInstruction( char* x, char *c );

/* Procedure emitCallInstruction emits a activation instruction
 * that is x = call f, n when the subroutine has a return value
 * or call f, n when it doesn't
 * x = the target variable to the return of the function
 * f = the name of the function
 * n = the number of parameters of the function
 * c = a comment to be printed if TraceCode is TRUE
 */
void emitCallInstruction( char* x, char *f, int n, char *c );

/* Procedure emitArrayAccessInstruction emits array access instructions
 * that are t_temp = i * n and t = v[t_temp]
 * t_temp = the target variable to store the memory offset
 * t = the target variable
 * i = the index of the array member
 * n = the byte size of the type of the array members
 * v = the array to be accessed
 * c = a comment to be printed if TraceCode is TRUE
 */
void emitArrayAccessInstruction( char* t_temp, char* t, int i, int n, char* v, char *c );

#endif
