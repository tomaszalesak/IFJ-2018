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

#include "string.c"


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


typedef struct LTSNode{
    string key;                                           
    //TSVar data;                                           
    struct LTSNode * LPtr, *RPtr;
} *LTSNodePtr;

//typedef  LTSNode tLTS;

/*
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
*/

typedef struct GTSNode{
    string key;                                           
    int argCount;
    int defined;
    struct GTSNode * LPtr, *RPtr;
} *GTSNodePtr;

//typedef  GTSNode tGTS;

void LTSInit (LTSNodePtr *);
LTSNodePtr LTSSearch (LTSNodePtr, string*);
int LTSInsert (LTSNodePtr *, string*);
void LTSDelete(LTSNodePtr*);

void GTSInit(GTSNodePtr *);
GTSNodePtr GTSSearch (GTSNodePtr, string*);
int GTSInsert (GTSNodePtr *, string*, int);//, GTSNode**);

#endif