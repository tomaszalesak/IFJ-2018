/**
 * Predmet:  IFJ
 * Projekt:  Implementacia prekladaca jazyka IFJ18
 * Subor:   symtable.c
 *
 * Popis: Tabulka symbolov realizovana pomocou binarneho vyhledavacieho stromu
 *
 *
 *
*/

#include <stdbool.h>
//#include <string.h>
#include "symtable.h"
#include "errors.h"

/**
 * Initialization of LTS.
 * @param RootPtr - LTS
 */
void ltsInit(LTSNodePtr *RootPtr) {
    *RootPtr = NULL;
}

/**
 * Initialization of GTS.
 * @param RootPtr - GTS
 */
void gtsInit(GTSNodePtr *RootPtr) {
    *RootPtr = NULL;
}

/**
 * Search the variable K in LTS.
 * @param RootPtr - LTS
 * @param K - variable name
 * @return pointer on the node containing variable K
 * @return NULL if the LTS is empty or variable K wasn't found
 */
LTSNodePtr ltsSearch(LTSNodePtr RootPtr, string *K) {
    if (RootPtr == NULL) {
        return NULL;
    } else {
        if (strcmp(K->str, (RootPtr->key.str)) == 0) {
            //najdeny kluc
            return RootPtr;
        } else {
            //kluc vacsi, pokracujem doprava
            if (strcmp(K->str, (RootPtr->key.str)) > 0) {
                return ltsSearch(RootPtr->RPtr, K);
            }
                //kluc mensi, pokracujem dolava
            else {
                return ltsSearch(RootPtr->LPtr, K);
            }
        }
    }
}

/**
 * Insert variable K into LTS.
 * @param RootPtr - LTS
 * @param K - variable name
 * @return SUCCESS if the variable was inserted
 * @return -1 if the variable has already been inserted before
 * @return ERR_INTERNAL if there was an memory allocation problem
 */
int ltsInsert(LTSNodePtr *RootPtr, string *K) {//, int type, int* i) {

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
                return -1;
            }
        }
    }

    //vytvorenie noveho prvku
    LTSNodePtr newitem = malloc(sizeof(struct LTSNode));
    if (newitem == NULL) {
        fprintf(stderr, "CHYBA_SYM: Chyba alokacie pamate pre novu polozku tabulky symbolov.\n");
        return ERR_INTERNAL;
    }
    //inicializovanie kluca
    if (strInit(&(newitem->key)) == STR_ERROR) {
        fprintf(stderr, "CHYBA_SYM: Chyba inicializacie polozky kluca.\n");
        free(newitem);
        return ERR_INTERNAL;
    }
    //vlozenie hodnoty kluca
    if (strCopyString(&(newitem->key), K) == STR_ERROR) {
        fprintf(stderr, "CHYBA_SYM: Chyba kopirovania kluca do tabulky symbolov.\n");
        free(newitem);
        return ERR_INTERNAL;
    }

    //defaultne nastavenie nedefinovaneho typu
    newitem->type = -1;
    //inicializacia ukazatelov na podstromy
    newitem->LPtr = NULL;
    newitem->RPtr = NULL;

    //strom je prazdny, vkladame korenovy uzol
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

/**
 * Frees the memory allocated for LTS and strings
 * @param RootPtr - LTS
 */
void ltsDelete(LTSNodePtr* RootPtr) {   // TODO check for SIGSEGV
    if(*RootPtr != NULL) {
        // rekurzivne odstranenie oboch podstromov
        ltsDelete(&((*RootPtr)->LPtr));
        ltsDelete(&((*RootPtr)->RPtr));

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

/**
 * Insert variable K into GTS.
 * @param RootPtr - GTS
 * @param K - variable name
 * @return SUCCESS if the variable was inserted
 * @return -1 if the variable has already been inserted before
 * @return ERR_INTERNAL if there was an memory allocation problem
 */
int gtsInsert (GTSNodePtr *RootPtr, string* K){

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
                return -1;
            }
        }
    }

    //vytvorenie noveho prvku
    GTSNodePtr newitem = malloc(sizeof(struct GTSNode));
    if (newitem == NULL) {
        fprintf(stderr, "CHYBA_SYM: Chyba alokace pameti pro novou polozku globalni tabulky symbolu.\n");
        return ERR_INTERNAL;
    }
    //inicializovanie kluca
    if (strInit(&(newitem->key)) == STR_ERROR) {
        fprintf(stderr, "CHYBA_SYM: Chyba inicializace polozky klice.\n");
        free(newitem);
        return ERR_INTERNAL;
    }
    //vlozenie hodnoty kluca
    if (strCopyString(&(newitem->key), K) == STR_ERROR) {
        fprintf(stderr, "CHYBA_SYM: Chyba kopirovani klice do tabulky symbolu.\n");
        free(newitem);
        return ERR_INTERNAL;
    }

    //inicializacia ukazatelov na podstromy
    newitem->LPtr = NULL;
    newitem->RPtr = NULL;

    //inicializacia datovej struktury
    newitem->paramCount = 0;
    newitem->defined = 0;

    //strom je prazdny, vkladame korenovy uzol
    if ((*RootPtr) == NULL) {
        *RootPtr = newitem;//, GTSNode**);
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

/**
 * Search the variable K in GTS.
 * @param RootPtr - GTS
 * @param K - variable name
 * @return pointer on the node containing variable K
 * @return NULL if the GTS is empty or variable K wasn't found
 */
GTSNodePtr gtsSearch (GTSNodePtr RootPtr, string* K){
    if (RootPtr == NULL) {
        return NULL;
    } else {
        if (strcmp(K->str, (RootPtr->key.str)) == 0) {
            // najdeny kluc
            return RootPtr;
        } else {
            //kluc vacsi, pokracujem doprava
            if (strcmp(K->str, (RootPtr->key.str)) > 0) {
                return gtsSearch(RootPtr->RPtr, K);
            }
                //kluc mensi, pokracujem dolava
            else {
                return gtsSearch(RootPtr->LPtr, K);
            }
        }
    }
}

/**
 * Sets the type of variable K in LTS.
 * @param RootPtr - LTS
 * @param K - variable name
 * @param type - variable type
 * @return EXIT_FAILURE if the variable isn't in LTS and exits
 */
void ltsSetIdType (LTSNodePtr RootPtr, string* K, int type) {
    LTSNodePtr tmp = NULL;
    if ((tmp = ltsSearch(RootPtr, K)) == NULL){
        fprintf(stderr, "ERROR_SYMTAMBLE! Cannot assign type to a nonexisting variable!\n");
        exit(ERR_UNDEF_REDEF);
    }
    tmp->type = type;
}

/**
 * Returns the type of variable K in LTS.
 * @param RootPtr - LTS
 * @param K - variable name
 * @return type of variable (int)
 * @return EXIT_FAILURE if the variable isn't in LTS and exits
 */
int ltsGetIdType (LTSNodePtr RootPtr, string* K) {
    LTSNodePtr tmp = NULL;
    if ((tmp = ltsSearch(RootPtr, K)) == NULL){
        fprintf(stderr, "ERROR_SYMTAMBLE! Variable doesn't exist in LTS!\n");
        exit(ERR_UNDEF_REDEF);
    }
    return tmp->type;
}

/**
 * Sets the number of function K parameters in GTS.
 * @param RootPtr - GTS
 * @param K - function name
 * @param paramCount - number of function params
 * @returns EXIT_FAILURE if the function isn't in GTS and exits
 */
void gtsSetParamCount (GTSNodePtr RootPtr, string *K, int paramCount) {
    GTSNodePtr tmp = NULL;
    if ((tmp = gtsSearch(RootPtr, K)) == NULL) {
        fprintf(stderr, "ERROR_SYMTAMBLE! Cannot assign parameters count to a nonexisting function!\n");
        exit(ERR_UNDEF_REDEF);
    }
    tmp->paramCount = paramCount;
}

/**
 * Returns params count of function K in GTS.
 * @param RootPtr - GTS
 * @param K - name of function
 * @return function params count
 * @returns EXIT_FAILURE if the function isn't in GTS and exits
 */
int gtsGetParamCount (GTSNodePtr RootPtr, string *K) {
    GTSNodePtr tmp = NULL;
    if ((tmp = gtsSearch(RootPtr, K)) == NULL) {
        fprintf(stderr, "ERROR_SYMTAMBLE! Cannot assign parameters count to a nonexisting function!\n");
        exit(ERR_UNDEF_REDEF);
    }
    return tmp->paramCount;
}

/**
 * Sets the K function as defined.
 * @param RootPtr - GTS
 * @param K - function name
 */
void gtsSetDefined (GTSNodePtr RootPtr, string *K) {
    GTSNodePtr tmp = NULL;
    if ((tmp = gtsSearch(RootPtr, K)) == NULL) {
        fprintf(stderr, "ERROR_SYMTAMBLE! Cannot assign defined value to a nonexisting function!\n");
        exit(ERR_UNDEF_REDEF);
    }
    tmp->defined = 1;
}

/**
 * Returns the defined param of function K.
 * @param RootPtr - GTS
 * @param K - function name
 * @return 1 if the function was defined, otherwise 0
 */
int gtsCheckIfDefined (GTSNodePtr RootPtr, string *K) {
    GTSNodePtr tmp = NULL;
    if ((tmp = gtsSearch(RootPtr, K)) == NULL) {
        fprintf(stderr, "ERROR_SYMTAMBLE! Cannot assign defined value to a nonexisting function!\n");
        exit(ERR_UNDEF_REDEF);
    }
    printf("Function %s is %s!\n", tmp->key.str, tmp->defined ? "already defined" : "undefined");
    return tmp->defined;
}

/**
 * Frees the memory allocated for GTS and strings
 * @param RootPtr - GTS
 */
void gtsDelete(GTSNodePtr* RootPtr) {   //TODO check for SIGSEGV on merlin
    if(*RootPtr != NULL) {
        // rekurzivne odstranenie oboch podstromov
        gtsDelete(&((*RootPtr)->LPtr));
        gtsDelete(&((*RootPtr)->RPtr));

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

//TODO add to string.c maybe
/**
 * Transforms data from token.data into struct string.
 * @param token - token to transform
 * @return struct string
 */
string createString (Token token){
    string K;
    strInit(&K);
    strAddString(&K, (char *) token.data);
    return K;
}

/**
 * Inserts Built in Functions into gts.
 * @param RootPtr - GTS
 */
void insertBIF (GTSNodePtr *RootPtr) {
    string k;
    strInit(&k);

    //inputs()
    strAddString(&k, "inputs");
    gtsInsert(RootPtr, &k);
    gtsSetDefined(*RootPtr, &k);
    gtsSetParamCount(*RootPtr, &k, 0);
    strClear(&k);

    //inputi()
    strAddString(&k, "inputi");
    gtsInsert(RootPtr, &k);
    gtsSetDefined(*RootPtr, &k);
    gtsSetParamCount(*RootPtr, &k, 0);
    strClear(&k);

    //inputf()
    strAddString(&k, "inputf");
    gtsInsert(RootPtr, &k);
    gtsSetDefined(*RootPtr, &k);
    gtsSetParamCount(*RootPtr, &k, 0);
    strClear(&k);

    //TODO print

    //length(s)
    strAddString(&k, "length");
    gtsInsert(RootPtr, &k);
    gtsSetDefined(*RootPtr, &k);
    gtsSetParamCount(*RootPtr, &k, 1);
    strClear(&k);

    //substr(s, i, n)
    strAddString(&k, "substr");
    gtsInsert(RootPtr, &k);
    gtsSetDefined(*RootPtr, &k);
    gtsSetParamCount(*RootPtr, &k, 3);
    strClear(&k);

    //ord(s, i)
    strAddString(&k, "ord");
    gtsInsert(RootPtr, &k);
    gtsSetDefined(*RootPtr, &k);
    gtsSetParamCount(*RootPtr, &k, 2);
    strClear(&k);

    //chr(i)
    strAddString(&k, "chr");
    gtsInsert(RootPtr, &k);
    gtsSetDefined(*RootPtr, &k);
    gtsSetParamCount(*RootPtr, &k, 1);
    strClear(&k);

}
