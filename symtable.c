/**
 * Predmet:  IFJ
 * Projekt:  Implementacia prekladaca jazyka IFJ18
 * Soubor:   symtable.c
 *
 * Popis: Tabulka symbolov realizovana pomocou binarneho vyhledavacieho stromu
 *
 *
 *
*/
#include <stdbool.h>
#include <string.h>
#include "symtable.h"

/* inicializacia lokalnej TS (works)*/
void LTSInit(LTSNodePtr *RootPtr) {
    *RootPtr = NULL;
}

/* inicializacia globalnej TS */
void GTSInit(GTSNodePtr *RootPtr) {
    *RootPtr = NULL;
}

/* vyhladanie prvku K v LTS (works)*/
LTSNodePtr LTSSearch(LTSNodePtr RootPtr, string *K) {
    if (RootPtr == NULL) {
        return NULL;
    } else {
        if (strcmp(K->str, (RootPtr->key.str)) == 0) {
            // najdeny kluc
            return RootPtr;
        } else {
            //kluc vacsi, pokracujem doprava
            if (strcmp(K->str, (RootPtr->key.str)) > 0) {
                return LTSSearch(RootPtr->RPtr, K);
            }
                //kluc mensi, pokracujem dolava
            else {
                return LTSSearch(RootPtr->LPtr, K);
            }
        }
    }
}

/* vlozenie prvku K do LTS (works?)*/
int LTSInsert(LTSNodePtr *RootPtr, string *K) {//, int type, int* i) {

    LTSNodePtr tmp = (*RootPtr);        //ulozenie ukazatela na BVS
    if ((*RootPtr) != NULL) {
        //tmp = (*RootPtr);
        // ak nie je prazdny, hladam
        int found = 0;
        while (found == 0) {

            //kluc vacsi, idem doprava
            if (strcmp(K->str, ((*RootPtr)->key.str)) > 0) {
                if ((*RootPtr)->RPtr != NULL) {
                    (*RootPtr) = (*RootPtr)->RPtr;
                } else found = 1;
            }
                //kluc mensi, idem dolava
            else if (strcmp(K->str, ((*RootPtr)->key.str)) < 0) {
                if ((*RootPtr)->LPtr != NULL) {
                    (*RootPtr) = (*RootPtr)->LPtr;
                } else found = 1;
            }
                //kluc najdeny, fail
            else if (strcmp(K->str, ((*RootPtr)->key.str)) == 0) {
                //found = 1;
                return ERROR_INSERTED;
            }
        }
    }

    //vytvorenie noveho prvku
    LTSNodePtr newitem = malloc(sizeof(struct LTSNode));
    if (newitem == NULL) {
        fprintf(stderr, "CHYBA_SYM: Chyba alokacie pamate pre novu polozku tabulky symbolov.\n");
        return INT_ERR;
    }
    //inicializovanie kluca
    if (strInit(&(newitem->key)) == INT_ERR) {
        fprintf(stderr, "CHYBA_SYM: Chyba inicializacie polozky kluca.\n");
        free(newitem);
        return INT_ERR;
    }
    //vlozenie hodnoty kluca
    if (strCopyString(&(newitem->key), K) == INT_ERR) {
        fprintf(stderr, "CHYBA_SYM: Chyba kopirovania kluca do tabulky symbolov.\n");
        free(newitem);
        return INT_ERR;
    }
    //vlozenie datoveho typu premennej
    //newitem->data.varType = type;
    /*
    switch(type) {
        case KW_INTEGER:
            newitem->data.varValue.iVal = 0;
            break;
        case KW_DOUBLE:
            newitem->data.varValue.dVal = 0.0;
            break;
        case KW_STRING:
            strInit(&(newitem->data.varValue.sVal));
            break;
    }
    */

    //inicializacia ukazatelov na podstromy
    newitem->LPtr = NULL;
    newitem->RPtr = NULL;

    //strom je prazdny, vkladame korenovy uzol
    //change *RootPtr == NULL a aj pri newitem
    if ((*RootPtr) == NULL) {
        *RootPtr = newitem;
    }
        //neprazdny strom, vkladame ako syna
    else {
        //naviazeme doprava ak ma vacsi kluc
        if (strcmp(K->str, ((*RootPtr)->key.str)) > 0) {
            (*RootPtr)->RPtr = newitem;
            (*RootPtr) = newitem;
            //dolava ak je kluc mensi
        } else if (strcmp(K->str, ((*RootPtr)->key.str)) < 0) {
            (*RootPtr)->LPtr = newitem;
            (*RootPtr) = newitem;
        }
    }
    //navratenie ukazatela na BVS
    if (tmp != NULL) {
        (*RootPtr) = tmp;
    }
    return SUCCESS;
}

// TODO check for SIGSEGV
void LTSDelete(LTSNodePtr* RootPtr) {
    if(*RootPtr != NULL) {
        // rekurzivne odstranenie oboch podstromov
        LTSDelete(&((*RootPtr)->LPtr));
        LTSDelete(&((*RootPtr)->RPtr));

        // uvolnenie pamate kluca
        strFree(&((*RootPtr)->key));

        // ak je v strukture string
        //if((RootPtr)->data.varType == KW_STRING) {
        //    strFree(&((RootPtr)->data.varValue.sVal));
        //}

        // uvolnenie pamate uzla, uvolnenie pamate
        free(*RootPtr);
        *RootPtr = NULL;
    }
}

/* vlozenie prvku K do GTS */
int GTSInsert (GTSNodePtr *RootPtr, string* K, int argCount){

    GTSNodePtr tmp = (*RootPtr);        //uloženie ukazateľa na BVS

    if ((*RootPtr) != NULL) {           //ak nie je prázdny hľadám
        int found = 0;
        while (found == 0) {

            //kluc vacsi, idem doprava
            if (strcmp(K->str, ((*RootPtr)->key.str)) > 0) {
                if ((*RootPtr)->RPtr != NULL) {
                    (*RootPtr) = (*RootPtr)->RPtr;
                } else found = 1;
            }
                //kluc mensi, idem dolava
            else if (strcmp(K->str, ((*RootPtr)->key.str)) < 0) {
                if ((*RootPtr)->LPtr != NULL) {
                    (*RootPtr) = (*RootPtr)->LPtr;
                } else found = 1;
            }
                //kluc najdeny
            else if (strcmp(K->str, ((*RootPtr)->key.str)) == 0) {
                //found = 1;
                return ERROR_INSERTED;
            }
        }
    }

    //vytvorenie noveho prvku
    GTSNodePtr newitem = malloc(sizeof(struct GTSNode));
    if (newitem == NULL) {
        fprintf(stderr, "CHYBA_SYM: Chyba alokace pameti pro novou polozku globalni tabulky symbolu.\n");
        return INT_ERR;
    }
    //inicializovanie kluca
    if (strInit(&(newitem->key)) == INT_ERR) {
        fprintf(stderr, "CHYBA_SYM: Chyba inicializace polozky klice.\n");
        free(newitem);
        return INT_ERR;
    }
    //vlozenie hodnoty kluca
    if (strCopyString(&(newitem->key), K) == INT_ERR) {
        fprintf(stderr, "CHYBA_SYM: Chyba kopirovani klice do tabulky symbolu.\n");
        free(newitem);
        return INT_ERR;
    }

    //inicializacia ukazatelov na podstromy
    newitem->LPtr = NULL;
    newitem->RPtr = NULL;

    //inicializacia datovej struktury
    newitem->argCount = argCount;
    newitem->defined = 0;

    //strom je prazdny, vkladame korenovy uzol
    if ((*RootPtr) == NULL) {
        *RootPtr = newitem;
        //*inserted = newitem;
    }
        //neprazny strom, vkladame ako syna
    else {
        //naviazeme doprava ak ma vacsi kluc
        if (strcmp(K->str, ((*RootPtr)->key.str)) > 0) {
            (*RootPtr)->RPtr = newitem;
            //*inserted = newitem;

            //dolava ak je kluc mensi
        } else if (strcmp(K->str, ((*RootPtr)->key.str)) < 0) {
            (*RootPtr)->LPtr = newitem;
            //*inserted = newitem;
        }
    }
    //navratenie ukazatela na BVS
    if (tmp != NULL){
        (*RootPtr) = tmp;
    }
    return SUCCESS;
}

GTSNodePtr GTSSearch (GTSNodePtr RootPtr, string* K){
    if (RootPtr == NULL) {
        return NULL;
    } else {
        if (strcmp(K->str, (RootPtr->key.str)) == 0) {
            // najdeny kluc
            return RootPtr;
        } else {
            //kluc vacsi, pokracujem doprava
            if (strcmp(K->str, (RootPtr->key.str)) > 0) {
                return GTSSearch(RootPtr->RPtr, K);
            }
                //kluc mensi, pokracujem dolava
            else {
                return GTSSearch(RootPtr->LPtr, K);
            }
        }
    }
}
