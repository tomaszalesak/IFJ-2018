/*
**	IFJ
**	Stack - Double-Linked List
**	---------------------------
**	Tomáš Zálešák - xzales13
*/

/* **********************************************************}
**
**      ltsDLInitList ...... inicializace seznamu před prvním použitím,
**      ltsDLDisposeList ... zrušení všech prvků seznamu,
**      ltsDLInsertFirst ... vložení prvku na začátek seznamu,
**      ltsDLInsertLast .... vložení prvku na konec seznamu,
**      ltsDLFirst ......... nastavení aktivity na první prvek,
**      ltsDLLast .......... nastavení aktivity na poslední prvek,
**      ltsDLCopyFirst ..... vrací hodnotu prvního prvku,
**      ltsDLCopyLast ...... vrací hodnotu posledního prvku,
**      ltsDLDeleteFirst ... zruší první prvek seznamu,
**      ltsDLDeleteLast .... zruší poslední prvek seznamu,
**      ltsDLPostDelete .... ruší prvek za aktivním prvkem,
**      ltsDLPreDelete ..... ruší prvek před aktivním prvkem,
**      ltsDLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      ltsDLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      ltsDLCopy .......... vrací hodnotu aktivního prvku,
**      ltsDLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      ltsDLSucc .......... posune aktivitu na další prvek seznamu,
**      ltsDLPred .......... posune aktivitu na předchozí prvek seznamu,
**      ltsDLActive ........ zjišťuje aktivitu seznamu.
**
**/

#include "sym_dl.h"
#include "errors.h"
#include "symtable.h"

void ltsDLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    //fprintf(stderr, "*ERROR* The program has performed an illegal operation.\n");
    exit(ERR_INTERNAL);
}

void ltsDLInitList(tltsDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    if (L != NULL) {
        L->Act = NULL;
        L->First = NULL;
        L->Last = NULL;
    }
}

void ltsDLInsertFirst(tltsDLList *L, LTSNodePtr val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    if (L != NULL) {
        // alokace pameti pro prvek
        tltsDLElemPtr p = (tltsDLElemPtr) malloc(sizeof(struct tltsDLElem));

        // nedostatek pameti
        if (p == NULL) {
            ltsDLError();
            return;
        }

        p->lts = val;
        p->rptr = L->First;
        p->lptr = NULL;

        // prvni prvek
        if (L->First == NULL) {
            L->Last = p;
        }
            // neni prvni
        else {
            L->First->lptr = p;
        }

        // prvek je nyni prvni
        L->First = p;
    }
}

void ltsDLFirst(tltsDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->First;
}

void ltsDLLast(tltsDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->Last;
}

void ltsDLPostDelete(tltsDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
    if (L == NULL) {
        return;
    }

    if (L->First == NULL) {
        return;
    }

    if (L->Act == NULL) {
        return;
    }

    if (L->Last == L->Act) {
        return;
    }

    // prvek, ktery rusime
    tltsDLElemPtr p;
    p = L->Act->rptr;

    L->Act->rptr = p->rptr;

    // posledni prvek
    if (p->rptr == NULL) {
        L->Last = L->Act;
    }
        // neni posledni
    else {
        // nasledujici ma odkaz na predchozi
        p->rptr->lptr = L->Act;
    }

    free(p);
}

void ltsDLPostInsert(tltsDLList *L, LTSNodePtr val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci ltsDLError().
**/
    // spatny ukazatel
    if (L == NULL) {
        return;
    }

    // neni aktivni
    if (L->Act == NULL) {
        return;
    }

    // prvek, ktery vkladame
    tltsDLElemPtr p;
    // alokace pameti pro prvek
    p = (tltsDLElemPtr) malloc(sizeof(struct tltsDLElem));

    // nedostatek pameti
    if (p == NULL) {
        ltsDLError();
        return;
    }

    p->lptr = L->Act;
    p->lts = val;

    // bude posledni
    if (L->Act->rptr == NULL) {
        p->rptr = NULL;
        L->Last = p;
    }
        // je za nim dalsi prvek
    else {
        L->Act->rptr->lptr = p;
        p->rptr = L->Act->rptr;
    }

    // aktivni ukazuje na pridany
    L->Act->rptr = p;
}

void ltsDLSucc(tltsDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
    // spatny ukazatel
    if (L == NULL) {
        return;
    }

    // neni aktivni
    if (L->Act == NULL) {
        return;
    }

    // posuneme aktivitu doprava
    L->Act = L->Act->rptr;
}

void ltsDLPred(tltsDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
    // spatny ukazatel
    if (L == NULL) {
        return;
    }

    // neni aktivni
    if (L->Act == NULL) {
        return;
    }

    // posuneme aktivitu doleva ak nie je aktualny prvok zaroven aj prvym
    if (L->Act->lptr != NULL)
        L->Act = L->Act->lptr;
}

/**
 * Checks in all previous LTS's for a declaration of variable.
 * @param L - LTS stack
 * @param K - variable to search for
 */
void ltsDLSearchPre(tltsDLList *L, string K) {
    int found = 0;
    if (ltsSearch(ltsStack->Act->lts, &K) != NULL) {
        found = 1;
    }
    if (!found) {
        do {
            ltsDLPred(L);
            if (ltsSearch(L->Act->lts, &K) != NULL) {
                found = 1;
            }
            //if (L->Act != L->First)
        } while (L->Act != L->First && found == 0);
        if (found == 0) {
            semanticError(ERR_UNDEF_REDEF, K, SYM_NONE, SYM_VAR);
        }
    }
    ltsDLLast(L);
}

