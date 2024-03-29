/****************************************************/
/* File: symtab.c                                   */
/* Symbol table implementation for the C- compiler  */
/* (allows only one symbol table)                   */
/* Symbol table is implemented as a chained         */
/* hash table                                       */
/* Project for CES41: Compiladores                  */
/* Prof. Ana Carolina Lorena                        */
/* Code writted by Gabriel Gobi                     */
/*                 Samuel Vendramini                */
/*                 Thiago Lopes                     */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "symtab.h"

/* SIZE is the size of the hash table */
#define SIZE 211

/* SHIFT is the power of two used as multiplier
   in hash function  */
#define SHIFT 4

/* the hash function */
static int hash ( char * key )
{ int temp = 0;
  int i = 0;
  while (key[i] != '\0')
  { temp = ((temp << SHIFT) + key[i]) % SIZE;
    ++i;
  }
  return temp;
}

/* the hash table */
static BucketList hashTable[SIZE];

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * It is inserted only the first time, otherwise ignored
 */
void st_insert( char * name, char * scopeName,
    IdType idType, ExpType dataType, int lineno )
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while ((l != NULL) && ((strcmp(name,l->name) != 0) 
                     || (strcmp(scopeName,l->scope) != 0) ))
    l = l->next;
  if (l == NULL) /* variable not yet in table */
  { l = (BucketList) malloc(sizeof(struct BucketListRec));
    l->name = name;
    l->idType = idType;
    l->dataType = dataType;
    l->scope = scopeName;
    if (lineno != -1) {
      l->lines = (LineList) malloc(sizeof(struct LineListRec));
      l->lines->lineno = lineno;
      l->lines->next = NULL;
    }
    l->next = hashTable[h];
    hashTable[h] = l; }
  else /* found in table, so just add line number */
  { if (l->lines == NULL) {
      l->lines = (LineList) malloc(sizeof(struct LineListRec));
      l->lines->lineno = lineno;
      l->lines->next = NULL;
    } else {
      LineList t = l->lines;
      while (t->next != NULL) {
          if (t->lineno == lineno) return; /* already appeared in that line*/
          t = t->next;
      }
      if (t->lineno == lineno) return; /* already appeared in that line*/
      t->next = (LineList) malloc(sizeof(struct LineListRec));
      t->next->lineno = lineno;
      t->next->next = NULL;
    }
  }
} /* st_insert */

/* Function st_lookup returns a pointer to the
 * symbol entry in the table or NULL if not found
 */
BucketList st_lookup ( char * name )
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  return l;
}

/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
void printSymTab(FILE * listing)
{ int i;
  fprintf(listing,"Variable Name  Scope     ID Type  Data Type  Line Numbers             \n");
  fprintf(listing,"-------------  --------  -------  ---------  -------------------------\n");
  for (i=0;i<SIZE;++i)
  { if (hashTable[i] != NULL)
    { BucketList l = hashTable[i];
      while (l != NULL)
      { LineList t = l->lines;
        fprintf(listing,"%-14s ",l->name);
        fprintf(listing,"%-8s  ",l->scope);
        fprintf(listing,"%-7s  ",idTypeNames[l->idType]);
        fprintf(listing,"%-10s ",expTypeNames[l->dataType]);
        while (t != NULL)
        { fprintf(listing,"%2d ",t->lineno);
          t = t->next;
        }
        fprintf(listing,"\n");
        l = l->next;
      }
    }
  }
} /* printSymTab */
