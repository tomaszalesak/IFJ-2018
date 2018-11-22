//
// Created by zales on 22.11.2018.
//

#ifndef PREC_AN_ERRORS_H
#define PREC_AN_ERRORS_H

// Error definitions

#define SUCCESS                 0
#define ERR_LEXICAL             1   // chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému)
#define ERR_SYNTAX              2   // chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu)
#define ERR_UNDEF_REDEF         3   // sémantická chyba v programu – nedefinovaná funkce/proměnná, pokus o redefinici funkce/proměnné, atp.
#define ERR_INCOMPATIBLE_TYPE   4   // sémantická/běhová chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech
#define ERR_NO_OF_ARGS          5   // sémantická chyba v programu – špatný počet parametrů u volání funkce
#define ERR_OTHER_SEMANTIC      6   // ostatní sémantické chyby
#define ERR_ZERO_DIVISION       9   // běhová chyba dělení nulou
#define ERR_INTERNAL            99  // interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alokace paměti, atd.)

void compiler_exit(int);

#endif //PREC_AN_ERRORS_H
