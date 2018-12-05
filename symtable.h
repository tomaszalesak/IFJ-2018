/**
 * Predmet:  IFJ
 * Projekt:  Implementacia prekladaca jazyka IFJ18
 * Soubor:   symtable.h
 *
 * Popis: hlavicka do suboru symtable.c
 *
 *
 *
 */

#ifndef IFJ_SYMTABLE_H
#define IFJ_SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexicalanalyzer.h"

/* chybove hlasenia */

#define  SUCCESS                   0
#define  ERR_INCOMPATIBLE_TYPE     4

#define SYM_FUNC_PARAM (-2)  //set for function parameters into lts
#define SYM_NONE (-1)  //no need for additional info
#define SYM_FUNC_UNDEF 0 //for semantic error messages of functions -> undefined
#define SYM_FUNC_REDEF 1 //for semantic error messages of functions -> redefined
#define SYM_VAR 2  //for semantic error messages of variables


typedef struct LTSNode{
    string key;     ///node (variable) name
    int type;       ///variable type
    struct LTSNode * LPtr, *RPtr;   ///pointers to child nodes
} *LTSNodePtr;

typedef struct GTSNode{
    string key;     ///node (variable) name
    int paramCount; ///number of function parameters
    int defined;    /// 0 for undefined, 1 for defined function
    struct GTSNode * LPtr, *RPtr;   ///pointers to child nodes
} *GTSNodePtr;

LTSNodePtr* ltsAct;
GTSNodePtr gts;
string K;

/**
 * Functions for LTS.
 */
void ltsInit (LTSNodePtr *);
LTSNodePtr ltsSearch (LTSNodePtr, string*);
int ltsInsert (LTSNodePtr *, string*);
void ltsDelete(LTSNodePtr*);
void ltsSetIdType (LTSNodePtr RootPtr, string* K, int type);
int ltsGetIdType (LTSNodePtr RootPtr, string* K);

/**
 * Functions for GTS.
 */
void gtsInit(GTSNodePtr *);
GTSNodePtr gtsSearch (GTSNodePtr, string*);
int gtsInsert (GTSNodePtr *, string*);
void gtsSetParamCount (GTSNodePtr RootPtr, string *K, int paramCount);
int gtsGetParamCount (GTSNodePtr RootPtr, string *K);
void gtsSetDefined (GTSNodePtr RootPtr, string *K);
int gtsCheckIfDefined (GTSNodePtr RootPtr, string *K);
void gtsDelete(GTSNodePtr* RootPtr);

void insertBIF (GTSNodePtr *RootPtr);

void ltsPrecVarCheck (Token token, int *strings, int *ints);

void semanticError(int errorCode, string k, int paramsCounter, int info);

string createString (Token token);

/**TODO check and delete
//int GTSInsertVar(tGTS**, string*);
void GTSDelete(GTSNodePtr*);
int lts_getVariableType(GTSNodePtr, string*);
LTSNodePtr lts_getVarNode(GTSNodePtr , string*);
int gts_setVariable(GTSNodePtr*, GTSNodePtr, int, string*);
LTSNodePtr lts_setVarNode(GTSNodePtr*, GTSNodePtr, int);
//tLTS* lts_setConst(GTSNodePtr*, GTSNodePtr, int, string*);
int gts_setArgument(GTSNodePtr*, GTSNodePtr, int, string*);
int gts_checkArgs(GTSNodePtr, int, string*, int);
int gts_setReturnValue(GTSNodePtr, int);
int gts_checkReturnValue(GTSNodePtr, int);
void gts_setDefinition(GTSNodePtr);
int gts_checkDefinition(GTSNodePtr);
int gts_checkAllDefinitions(GTSNodePtr);
//int lts_getVariableType(GTSNode *, string*);
int Length(char *s);
char* SubStr(char *, int, int);
int Asc(char *, int);
char* Chr(int);
*/

#endif
