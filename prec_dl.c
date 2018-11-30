/*
**	IFJ
**	Stack - Double-Linked List
**	---------------------------
**	Tomáš Zálešák - xzales13
*/

/* **********************************************************}
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
**/

#include "prec_dl.h"
#include "errors.h"

void print_elements_of_list(tDLList TL)	{

    tDLList TempList=TL;

    fprintf(stderr, "-----------------");

    while (TempList.First!=NULL)	{
        fprintf(stderr, "\n \t%d",TempList.First->data);
        if ((TempList.First==TL.Act) && (TL.Act!=NULL))
            fprintf(stderr, "\t <= toto je aktivní prvek ");
        TempList.First=TempList.First->rptr;
    }
    fprintf(stderr, "\n-----------------\n");
}

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    fprintf(stderr, "*ERROR* The program has performed an illegal operation.\n");
    exit(ERR_INTERNAL);
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    if (L != NULL)
    {
        L->Act = NULL;
        L->First = NULL;
        L->Last = NULL;
    }
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free.
**/
    if (L != NULL)
    {
        if (L->First != NULL)
        {
            tDLElemPtr p;

            while (L->First != NULL)
            {
                p = L->First;
                L->First = L->First->rptr;
                free(p);
            }

            L->Act = NULL;
            L->Last = NULL;
        }
    }
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    if (L != NULL)
    {
        // alokace pameti pro prvek
        tDLElemPtr p = (tDLElemPtr) malloc(sizeof(struct tDLElem));

        // nedostatek pameti
        if (p == NULL)
        {
            DLError();
            return;
        }

        p->data = val;
        p->rptr = L->First;
        p->lptr = NULL;

        // prvni prvek
        if (L->First == NULL)
        {
            L->Last = p;
        }
            // neni prvni
        else
        {
            L->First->lptr = p;
        }

        // prvek je nyni prvni
        L->First = p;
    }
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    if (L != NULL)
    {
        // alokace pameti pro prvek
        tDLElemPtr p = (tDLElemPtr) malloc(sizeof(struct tDLElem));

        // nedostatek pameti
        if (p == NULL)
        {
            DLError();
            return;
        }

        p->data = val;
        p->rptr = NULL;
        p->lptr = L->Last;

        // prvni prvek
        if (L->First == NULL)
        {
            L->First = p;
        }
            // neni prvni
        else
        {
            L->Last->rptr = p;
        }

        // prvek je nyni posledni
        L->Last = p;
    }
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if (L != NULL)
    {
        if (L->First == NULL)
        {
            DLError();
        }

        else
        {
            *val = L->First->data;
        }
    }
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if (L != NULL)
    {
        if (L->Last == NULL)
        {
            DLError();
        }

        else
        {
            *val = L->Last->data;
        }
    }
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
    if (L == NULL)
    {
        return;
    }

    if (L->First == NULL)
    {
        return;
    }

    if (L->First == L->Act)
    {
        L->Act = NULL;
    }

    tDLElemPtr p;
    p = L->First;

    // jeden prvek
    if (L->First == L->Last)
    {
        L->Act = NULL;
        L->First = NULL;
        L->Last = NULL;
    }
        // vice prvku
    else
    {
        L->First = L->First->rptr;
        L->First->lptr = NULL;
    }

    free(p);
}

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
    if (L == NULL)
    {
        return;
    }

    if (L->Last == NULL)
    {
        return;
    }

    if (L->Last == L->Act)
    {
        L->Act = NULL;
    }

    tDLElemPtr p;
    p = L->Last;

    // jeden prvek
    if (L->First == L->Last)
    {
        L->Act = NULL;
        L->First = NULL;
        L->Last = NULL;
    }
        // vice prvku
    else
    {
        L->Last = L->Last->lptr;
        L->Last->rptr = NULL;
    }

    free(p);
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
    if (L == NULL)
    {
        return;
    }

    if (L->First == NULL)
    {
        return;
    }

    if (L->Act == NULL)
    {
        return;
    }

    if (L->Last == L->Act)
    {
        return;
    }

    // prvek, ktery rusime
    tDLElemPtr p;
    p = L->Act->rptr;

    L->Act->rptr = p->rptr;

    // posledni prvek
    if (p->rptr == NULL)
    {
        L->Last = L->Act;
    }
        // neni posledni
    else
    {
        // nasledujici ma odkaz na predchozi
        p->rptr->lptr = L->Act;
    }

    free(p);
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
    // spatny ukazatel
    if (L == NULL)
    {
        return;
    }

    // prazdny
    if (L->First == NULL)
    {
        return;
    }

    // neni aktivni
    if (L->Act == NULL)
    {
        return;
    }

    // aktivni == prvni
    if (L->First == L->Act)
    {
        return;
    }

    // prvek, ktery rusime
    tDLElemPtr p;
    p = L->Act->lptr;

    L->Act->lptr = p->lptr;

    // prvni prvek
    if (p->lptr == NULL)
    {
        L->First = L->Act;
    }
        // neni prvni

    else
    {
        // nasledujici ma odkaz na predchozi
        p->lptr->rptr = L->Act;
    }

    free(p);
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    // spatny ukazatel
    if (L == NULL)
    {
        return;
    }

    // neni aktivni
    if (L->Act == NULL)
    {
        return;
    }

    // prvek, ktery vkladame
    tDLElemPtr p;
    // alokace pameti pro prvek
    p = (tDLElemPtr) malloc(sizeof(struct tDLElem));

    // nedostatek pameti
    if (p == NULL)
    {
        DLError();
        return;
    }

    p->lptr = L->Act;
    p->data = val;

    // bude posledni
    if (L->Act->rptr == NULL)
    {
        p->rptr = NULL;
        L->Last = p;
    }
        // je za nim dalsi prvek
    else
    {
        L->Act->rptr->lptr = p;
        p->rptr = L->Act->rptr;
    }

    // aktivni ukazuje na pridany
    L->Act->rptr = p;
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    // spatny ukazatel
    if (L == NULL)
    {
        return;
    }

    // neni aktivni
    if (L->Act == NULL)
    {
        return;
    }

    // prvek, ktery vkladame
    tDLElemPtr p;
    // alokace pameti pro prvek
    p = (tDLElemPtr) malloc(sizeof(struct tDLElem));

    // nedostatek pameti
    if (p == NULL)
    {
        DLError();
        return;
    }

    p->rptr = L->Act;
    p->data = val;

    // pokud je prvni
    if (L->Act->lptr == NULL)
    {
        p->lptr = NULL;
        L->First = p;
    }
        // je pred nim dalsi prvek
    else
    {
        L->Act->lptr->rptr = p;
        p->lptr = L->Act->lptr;
    }

    // aktivni ukazuje na pridany
    L->Act->lptr = p;
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
    // spatny ukazatel
    if (L == NULL)
    {
        return;
    }

    // neni aktivni, chyba
    if (L->Act == NULL)
    {
        DLError();
        return;
    }

    // copy
    *val = L->Act->data;

}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
    // spatny ukazatel
    if (L == NULL)
    {
        return;
    }

    // neni aktivni
    if (L->Act == NULL)
    {
        return;
    }

    // aktualizujeme hodnotu
    L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
    // spatny ukazatel
    if (L == NULL)
    {
        return;
    }

    // neni aktivni
    if (L->Act == NULL)
    {
        return;
    }

    // posuneme aktivitu doprava
    L->Act = L->Act->rptr;
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
    // spatny ukazatel
    if (L == NULL)
    {
        return;
    }

    // neni aktivni
    if (L->Act == NULL)
    {
        return;
    }

    // posuneme aktivitu doleva
    L->Act = L->Act->lptr;
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
    return (L->Act == NULL) ? 0 : 1;
}

