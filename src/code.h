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

/* Procedure getNewBranchLabel returns a new label 
 * to a branch of code to build the intermediate code
 */
char * getNewBranchLabel( void );

/* Procedure emitComment prints a comment line 
 * with comment c in the code file
 */
void emitComment( char * c );

/* Function emitLabel emits a code label
 * before the intermediate code instruction
 * label = the label for the line of code
 */
void emitLabel( char * label);

/* Procedure emitAssignInstruction emits a assign instruction
 * that can be le = ld or le = ld op ld2
 * op = the numeric operator
 * le = the left target variable
 * ld = 1st right source variable
 * ld2 = 2nd right source variable
 */
void emitAssignInstruction( char *op, char* le, char* ld, char* ld2);

/* Procedure emitBranchInstruction emits a branch instruction
 * that can be goto L or if_true x goto L
 * L = the target label
 * x = the test variable
 * checkTrue = TRUE, if the test is if_true or FALSE, if the test is if_false
 */
void emitBranchInstruction(char* x,  char * L, int checkTrue);

/* Procedure emitReturnInstruction emits a return instruction
 * that is return v
 * v = the variable to be returned by the subroutine
 */
void emitReturnInstruction( char* v);

/* Procedure emitParamInstruction emits a param instruction
 * that is param x
 * x = the variable to be used as a param to subroutine activation
 */
void emitParamInstruction( char* x);

/* Procedure emitCallInstruction emits a activation instruction
 * that is x = call f, n when the subroutine has a return value
 * or call f, n when it doesn't
 * x = the target variable to the return of the function
 * f = the name of the function
 * n = the number of parameters of the function
 */
void emitCallInstruction( char* x, char *f, int n);

/* Procedure emitArrayAccessInstruction emits array access instructions
 * that are t_temp = i * n and t = v[t_temp]
 * t_temp = the target variable to store the memory offset
 * t = the target variable
 * i = the index of the array member
 * n = the byte size of the type of the array members
 * v = the array to be accessed
 */
void emitArrayAccessInstruction( char* t, char* i, int n);

/* Procedure emitHaltInstruction prints a halt instruction 
 * in the code file
 */
void emitHaltInstruction( void );

#endif
