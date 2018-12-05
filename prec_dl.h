/**
 * Předmět: IFJ
 * Projekt: Implementace překladače jazyka IFJ18
 * Soubor:  prec_dl.h
 *
 * Popis:   Precedenční analýza - práce se stackem - hlavičkový soubor
 *
 * Autoři:  Vojtěch Novotný     xnovot1f@stud.fit.vutbr.cz
 *          Tomáš Zálešák       xzales13@stud.fit.vutbr.cz
 *          Robin Skaličan      xskali19@stud.fit.vutbr.cz
 *          Tomáš Smädo         xsmado00@stud.fit.vutbr.cz
*/

#ifndef PREC_DL_H
#define PREC_DL_H

#include<stdio.h>
#include<stdlib.h>
#include "lexicalanalyzer.h"


typedef struct tDLElem {                 /* prvek dvousměrně vázaného seznamu */
    int precData;
    Token expressionToken;                                          /* užitečná data */
    struct tDLElem *lptr;          /* ukazatel na předchozí prvek seznamu */
    struct tDLElem *rptr;        /* ukazatel na následující prvek seznamu */
} *tDLElemPtr;

typedef struct {                                  /* dvousměrně vázaný seznam */
    tDLElemPtr First;                      /* ukazatel na první prvek seznamu */
    tDLElemPtr Act;                     /* ukazatel na aktuální prvek seznamu */
    tDLElemPtr Last;                    /* ukazatel na posledni prvek seznamu */
} tDLList;

/* prototypy jednotlivých funkcí */
void DLInitList (tDLList *);
void DLDisposeList (tDLList *);
void DLInsertFirst (tDLList *, int);
void DLInsertLast(tDLList *, int);
void DLFirst (tDLList *);
void DLLast (tDLList *);
void DLCopyFirst (tDLList *, int *);
void DLCopyLast (tDLList *, int *);
void DLDeleteFirst (tDLList *);
void DLDeleteLast (tDLList *);
void DLPostDelete (tDLList *);
void DLPreDelete (tDLList *);
void DLPostInsert (tDLList *, int);
void DLPreInsert (tDLList *, int);
void DLCopy (tDLList *, int *);
void DLActualize (tDLList *, int);
void DLSucc (tDLList *);
void DLPred (tDLList *);
int DLActive (tDLList *);

void print_elements_of_list(tDLList);

#endif
