/****************************************************/
/* File: symtab.h                                   */
/* Symbol table interface for the C- compiler       */
/* (allows only one symbol table)                   */
/* Project for CES41: Compiladores                  */
/* Prof. Ana Carolina Lorena                        */
/* Code writted by Gabriel Gobi                     */
/*                 Samuel Vendramini                */
/*                 Thiago Lopes                     */
/****************************************************/

#ifndef _SYMTAB_H_
#define _SYMTAB_H_

/* the list of line numbers of the source 
 * code in which a variable is referenced
 */
typedef struct LineListRec
   { int lineno;
     struct LineListRec * next;
   } * LineList;

/* The record in the bucket lists for
 * each variable, including name, scope,
 * ID type, data type and the list
 * of line numbers in which it
 * appears in the source code
 */
typedef struct BucketListRec
   { char * name;
     LineList lines;
     char * scope;
     IdType idType;
     ExpType dataType;
     struct BucketListRec * next;
   } * BucketList;

/* Procedure st_insert inserts line numbers,
 * memory locations, scope information, data
 * and id types into the symbol table
 * It is inserted only the
 * first time, otherwise ignored
 */
void st_insert( char * name, char * scopeName,
    IdType idType, ExpType dataType, int lineno);

/* Function st_lookup returns the symbol table location with a pointer.
 * NULL if not found.
 */
BucketList st_lookup ( char * name );

/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
void printSymTab(FILE * listing);

#endif
