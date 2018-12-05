/**
 * Předmět: IFJ
 * Projekt: Implementace překladače jazyka IFJ18
 * Soubor:  errors.c
 *
 * Popis:   Generátor chybového výstupu
 *
 * Autoři:  Vojtěch Novotný     xnovot1f@stud.fit.vutbr.cz
 *          Tomáš Zálešák       xzales13@stud.fit.vutbr.cz
 *          Robin Skaličan      xskali19@stud.fit.vutbr.cz
 *          Tomáš Smädo         xsmado00@stud.fit.vutbr.cz
*/

#include <stdlib.h>
#include "errors.h"

void compiler_exit(int error_code)
{
    // todo
    // clean everything

    // exit with given error code
    exit(error_code);
}
