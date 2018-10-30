/**
 * Predmet:  IFJ
 * Projekt:  Implementacia prekladaca jazyka IFJ18
 * Soubor:   symtable.h
 *
 * Popis: hlavicka do souboru symtable.c
 *
 *
 *
 */


#ifndef IFJ_SYMTABLE_H
#define IFJ_SYMTABLE_H


#include "scanner.h"

#define ERROR_INSERTED 1


typedef struct {
   int iVal;
   double dVal;
   tString sVal;
}  tValue;


typedef struct ts_var{
   int varType;        // dat. typ promenne
   int varOffset;      // offset v prog. zasobniku
   tValue varValue;    // unie bude obsahovat data 
} TSVar;


typedef struct LTSNode{
    tString key;                                           
    TSVar data;                                           
    struct LTSNode * LPtr, *RPtr;
} LTSNode;

typedef  LTSNode tLTS;

typedef struct {
   int argCount;        // pocet parametru fce
   int retType;         // dat. typ navratove hodnoty fce
   int varCount;        // pocet lokalnich promennych fce
   LTSNode* lts;       // ukazatel na lokani TS fce
   //struct listItem* fInstr;   // ukazatel na prvni instrukci fce
   int def;             // po deklaraci fce = 0, po definici fce = 1
   int varReserved;     // po vlozeni rezervace jmena lokalni promenne = 1
   tString types;        // string s typy parametru - pro kontrolu pri volani funkce
} TSFunction;

typedef struct GTSNode{
    tString key;                                           
    TSFunction data;                                           
    struct GTSNode * LPtr, *RPtr;
} GTSNode;


typedef  GTSNode tGTS;


void LTSInit (LTSNode **);
void GTSInit(GTSNode **);
LTSNode* LTSSearch (tLTS*, tString*);
GTSNode* GTSSearch (tGTS*, tString*);
int GTSInsert (GTSNode **, tString*, GTSNode**);
int LTSInsert (LTSNode **, tString*, int, int*);
int GTSInsertVar(tGTS**, tString*);
void LTSDelete(LTSNode**);
void GTSDelete(GTSNode**);
int lts_getVariableType(GTSNode *, tString*);
LTSNode* lts_getVarNode(GTSNode *, tString*);
int gts_setVariable(GTSNode**, GTSNode*, int, tString*);
LTSNode* lts_setVarNode(GTSNode**, GTSNode*, int);
tLTS* lts_setConst(GTSNode**, GTSNode*, int, tString*);
int gts_setArgument(GTSNode**, GTSNode*, int, tString*);
int gts_checkArgs(GTSNode*, int, tString*, int);
int gts_setReturnValue(GTSNode*, int);
int gts_checkReturnValue(GTSNode*, int);
void gts_setDefinition(GTSNode*);
int gts_checkDefinition(GTSNode*);
int gts_checkAllDefinitions(GTSNode*);
int lts_getVariableType(GTSNode *, tString*);
int Length(char *s);
char* SubStr(char *, int, int);
int Asc(char *, int);
char* Chr(int);


#endif