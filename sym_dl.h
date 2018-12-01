//
// Created by xsmado00 on 29.11.2018.
//

#ifndef SYM_DL_H
#define SYM_DL_H


#include<stdio.h>
#include<stdlib.h>
#include "symtable.h"


typedef struct tltsDLElem {                 /* prvek dvousměrně vázaného seznamu */
    LTSNodePtr lts;                     /* Local SymTable */
    struct tltsDLElem *lptr;          /* ukazatel na předchozí prvek seznamu */
    struct tltsDLElem *rptr;        /* ukazatel na následující prvek seznamu */
} *tltsDLElemPtr;

typedef struct {                                  /* dvousměrně vázaný seznam */
    tltsDLElemPtr First;                      /* ukazatel na první prvek seznamu */
    tltsDLElemPtr Act;                     /* ukazatel na aktuální prvek seznamu */
    tltsDLElemPtr Last;                    /* ukazatel na posledni prvek seznamu */
} tltsDLList;

tltsDLList *ltsStack;

/* prototypy jednotlivých funkcí */
void ltsDLInitList (tltsDLList *);
void ltsDLDisposeList (tltsDLList *);
void ltsDLInsertFirst (tltsDLList *, LTSNodePtr val);
void ltsDLInsertLast(tltsDLList *, int);
void ltsDLFirst (tltsDLList *);
void ltsDLLast (tltsDLList *);
void ltsDLCopyFirst (tltsDLList *, int *);
void ltsDLCopyLast (tltsDLList *, int *);
void ltsDLDeleteFirst (tltsDLList *);
void ltsDLDeleteLast (tltsDLList *);
void ltsDLPostDelete (tltsDLList *);
void ltsDLPreDelete (tltsDLList *);
void ltsDLPostInsert (tltsDLList *, LTSNodePtr val);
void ltsDLPreInsert (tltsDLList *, int);
void ltsDLCopy (tltsDLList *, int *);
void ltsDLActualize (tltsDLList *, int);
void ltsDLSucc (tltsDLList *);
void ltsDLPred (tltsDLList *);
int ltsDLActive (tltsDLList *);

int ltsDLSearchPre (tltsDLList *L, string K);

void lts_print_elements_of_list(tltsDLList);

#endif //SYM_DL_H
