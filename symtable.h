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

/* namiesto scanner.h */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexicalanalyzer.h"

//#include "string.c"


/*#include "scanner.h"*/

/* chybove hlasenia */
#define ERROR_INSERTED             1  // chyba pri vkladani do TS 

#define  SUCCESS                   0  //  překlad proběhl bez chyb 1 
// chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému)
#define  ERROR_LEX                 1
// chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu)
#define  SYN_ERR                   2
#define  SEM_ERR                   3   // sémantická chyba v programu 
// sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech
#define  ERR_INCOMPATIBLE_TYPE     4
#define  ERR_SEMANTIC              6   // ostatne sémantické chyby
#define  INT_ERR                   99  // interna chyba prekladača

#define SIZE            10


typedef struct ts_var{
   int varType;        // dat. typ promenne
   int varOffset;      // offset v prog. zasobniku
   //tValue varValue;    // unie bude obsahovat data
} TSVar;


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

//typedef  LTSNode tLTS;

typedef struct {
   int argCount;        // pocet parametru fce
   int retType;         // dat. typ navratove hodnoty fce
   int varCount;        // pocet lokalnich promennych fce
   LTSNodePtr* lts;       // ukazatel na lokani TS fce
   //struct listItem* fInstr;   // ukazatel na prvni instrukci fce
   int def;             // po deklaraci fce = 0, po definici fce = 1
   int varReserved;     // po vlozeni rezervace jmena lokalni promenne = 1
   string types;        // string s typy parametru - pro kontrolu pri volani funkce
} TSFunction;

//typedef  GTSNode tGTS;

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
