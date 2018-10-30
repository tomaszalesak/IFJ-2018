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

#include "symtable.h"
#include "instruction.h"
#include "error.c"



int Length(char *s)
{
  int len = 0;
  while (*s++)
  {
    len++;
  }
  return len;
}

char * SubStr(char *s, int i, int n)
{
  int str_len = Length(s);
  char * substr;

  if (s == NULL || i <= 0)
  {
    return NULL;
  }
  else if (n < 0 || (n > (str_len - i)))
  {
    i--;
    substr = malloc(str_len - i + 1);
    if (substr == NULL)
    {
      // E
    }
    strncpy(substr, s + i, str_len - i);
    substr[str_len - i] = '\0';
    return substr;
  }
  i--;
  substr = malloc(n + 1);
  if (substr == NULL)
  {
    // E
  }
  strncpy(substr, s + i, n);
  substr[n] = '\0';
  return substr;
}

int Asc(char *s, int i)
{
  return ((i < 0 || i >= Length(s)) ? 0 : (int)(*(s+i)));
}

char * Chr(int i)
{
  char *ret = malloc(2);
  ret[0] = (char)i;
  ret[1] = '\0';

  return ret;
}

void LTSInit (LTSNode **RootPtr){
    *RootPtr = NULL;
}

void GTSInit(GTSNode **RootPtr){
  *RootPtr = NULL;
}


LTSNode* LTSSearch (tLTS* RootPtr, tString* K){
   if(RootPtr == NULL) {
      return NULL;
   }
   else {
      if(strcmp(K->data, (RootPtr->key.data)) == 0) {
         // najdeny kluc
         return RootPtr;
      }
      else {
        //kluc vacsi, pokracujem doprava
         if(strcmp(K->data, (RootPtr->key.data)) > 0) {
            return LTSSearch(RootPtr->RPtr, K);
         }
             //kluc mensi, pokracujem dolava
         else {
            return LTSSearch(RootPtr->LPtr, K);
         }
      }
   }
}

GTSNode* GTSSearch (tGTS* RootPtr, tString* K){
    if(RootPtr == NULL) {
      return NULL;
   }
   else {
      if(strcmp(K->data, (RootPtr->key.data)) == 0) {
         // najdeny kluc
         return RootPtr;
      }
      else {
         //kluc vacsi, pokracujem doprava
         if(strcmp(K->data, (RootPtr->key.data)) > 0) {
            return GTSSearch(RootPtr->RPtr, K);
         }
             //kluc mensi, pokracujem dolava
         else {
            return GTSSearch(RootPtr->LPtr, K);
         }
      }
   }
}

// ERROR_MEM chyba pri praci s pamatou
// ERROR_INSERTED  identifikator uz bol vlozeny
// ERROR_SUCCESS uspesne vlozenie noveho identifikatora
int GTSInsert (GTSNode **RootPtr, tString* K, GTSNode** inserted){

    GTSNode *tmp = (*RootPtr);        //uloženie ukazateľa na BVS

    if ((*RootPtr) != NULL) {           //ak nie je prázdny hľadám
        int found = 0;
        while (found == 0) {

            //kluc vacsi, idem doprava
            if (strcmp(K->data, ((*RootPtr)->key.data)) > 0) {
                if ((*RootPtr)->RPtr != NULL) {
                    (*RootPtr) = (*RootPtr)->RPtr;
                } else found = 1;
            }
                //kluc mensi, idem dolava
            else if (strcmp(K->data, ((*RootPtr)->key.data)) < 0) {
                if ((*RootPtr)->LPtr != NULL) {
                    (*RootPtr) = (*RootPtr)->LPtr;
                } else found = 1;
            }
                //kluc najdeny
            else if (strcmp(K->data, ((*RootPtr)->key.data)) == 0) {
                //found = 1;
                return ERROR_INSERTED;
            }
        }
    }

    //vytvorenie noveho prvku
    GTSNode *newitem = malloc(sizeof(struct GTSNode));
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
    if (strCopyStr(&(newitem->key), K) == INT_ERR) {
        fprintf(stderr, "CHYBA_SYM: Chyba kopirovani klice do tabulky symbolu.\n");
        free(newitem);
        return INT_ERR;
    }

    //inicializacia ukazatelov na podstromy
    newitem->LPtr = NULL;
    newitem->RPtr = NULL;

    //inicializacia datovej struktury
    newitem->data.lts = NULL;
    newitem->data.def = 0;
    newitem->data.argCount = 0;
    newitem->data.retType = 0;
    newitem->data.varCount = 0;
    newitem->data.varReserved = 0;

    if (strInit(&(newitem->data.types)) == INT_ERR) {
        fprintf(stderr, "CHYBA_SYM: Chyba inicializace polozky pro typy parametru funkce.\n");
        free(newitem);
        return INT_ERR;
    }

    //strom je prazdny, vkladame korenovy uzol
    if ((*RootPtr) == NULL) {
        *RootPtr = newitem;
        *inserted = newitem;
    }
        //neprazny strom, vkladame ako syna
    else {
        //naviazeme doprava ak ma vacsi kluc
        if (strcmp(K->data, ((*RootPtr)->key.data)) > 0) {
            (*RootPtr)->RPtr = newitem;
            *inserted = newitem;
            //dolava ak je kluc mensi
        } else if (strcmp(K->data, ((*RootPtr)->key.data)) > 0) {
            (*RootPtr)->LPtr = newitem;
            *inserted = newitem;
        }
    }
    //navratenie ukazatela na BVS
    if (tmp != NULL){
        (*RootPtr) = tmp;
    }
    return SUCCESS;
}

int LTSInsert (LTSNode **RootPtr, tString* K, int type, int* i) {

    LTSNode *tmp = (*RootPtr);        //ulozenie ukazatela na BVS

    if ((*RootPtr) != NULL) {           //ak nie je prazdny, hladam
        int found = 0;
        while (found == 0) {

            //kluc vacsi, idem doprava
            if (strcmp(K->data, ((*RootPtr)->key.data)) > 0) {
                if ((*RootPtr)->RPtr != NULL) {
                    (*RootPtr) = (*RootPtr)->RPtr;
                } else found = 1;
            }
                //kluc mensi, idem dolava
            else if (strcmp(K->data, ((*RootPtr)->key.data)) < 0) {
                if ((*RootPtr)->LPtr != NULL) {
                    (*RootPtr) = (*RootPtr)->LPtr;
                } else found = 1;
            }
                //kluc najdeny
            else if (strcmp(K->data, ((*RootPtr)->key.data)) == 0) {
                //found = 1;
                return ERROR_INSERTED;
            }
        }
    }

    //vytvorenie noveho prvku
    LTSNode *newitem = malloc(sizeof(struct LTSNode));
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
    if (strCopyStr(&(newitem->key), K) == INT_ERR) {
        fprintf(stderr, "CHYBA_SYM: Chyba kopirovani klice do tabulky symbolu.\n");
        free(newitem);
        return INT_ERR;
    }

    //vlozenie datoveho typu premennej
    newitem->data.varType = type;

    //nastavenie indexu
    newitem->data.varOffset = *i;
    *i += 1;

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

    //inicializacia ukazatelov na podstromy
    newitem->LPtr = NULL;
    newitem->RPtr = NULL;


    //strom je prazdny, vkladame korenovy uzol
    if ((*RootPtr) == NULL) {
        *RootPtr = newitem;
    }
        //neprazny strom, vkladame ako syna
    else {
        //naviazeme doprava ak ma vacsi kluc
        if (strcmp(K->data, ((*RootPtr)->key.data)) > 0) {
            (*RootPtr)->RPtr = newitem;
            (*RootPtr) = newitem;
            //dolava ak je kluc mensi
        } else if (strcmp(K->data, ((*RootPtr)->key.data)) > 0) {
            (*RootPtr)->LPtr = newitem;
            (*RootPtr) = newitem;
        }
    }
    //navratenie ukayatela na BVS
    if (tmp != NULL){
        (*RootPtr) = tmp;
    }
    return SUCCESS;
}


int GTSInsertVar (GTSNode **RootPtr, tString* K){

    GTSNode *tmp = (*RootPtr);        //uloženie ukazateľa na BVS

    if ((*RootPtr) != NULL) {           //ak nie je prázdny hľadám
        int found = 0;
        while (found == 0) {

            //kluc vacsi, idem doprava
            if (strcmp(K->data, ((*RootPtr)->key.data)) > 0) {
                if ((*RootPtr)->RPtr != NULL) {
                    (*RootPtr) = (*RootPtr)->RPtr;
                } else found = 1;
            }
                //kluc mensi, idem dolava
            else if (strcmp(K->data, ((*RootPtr)->key.data)) < 0) {
                if ((*RootPtr)->LPtr != NULL) {
                    (*RootPtr) = (*RootPtr)->LPtr;
                } else found = 1;
            }
                //kluc najdeny
            else {
                if ((*RootPtr)->data.varReserved) {
                    //found = 1;
                    return SUCCESS;
                }
            }
        }
    }

    //vytvorenie noveho prvku
    GTSNode *newitem = malloc(sizeof(struct GTSNode));
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
    if (strCopyStr(&(newitem->key), K) == INT_ERR) {
        fprintf(stderr, "CHYBA_SYM: Chyba kopirovani klice do tabulky symbolu.\n");
        free(newitem);
        return INT_ERR;
    }

    //inicializacia ukazatelov na podstromy
    newitem->LPtr = NULL;
    newitem->RPtr = NULL;

    //inicializacia datovej struktury
    newitem->data.lts = NULL;
    newitem->data.def = 0;
    newitem->data.argCount = 0;
    newitem->data.retType = 0;
    newitem->data.varCount = 0;
    newitem->data.varReserved = 1;

    if (strInit(&(newitem->data.types)) == INT_ERR) {
        fprintf(stderr, "CHYBA_SYM: Chyba inicializace polozky pro typy parametru funkce.\n");
        free(newitem);
        return INT_ERR;
    }

    //strom je prazdny, vkladame korenovy uzol
    if ((*RootPtr) == NULL) {
        *RootPtr = newitem;
    }
        //neprazny strom, vkladame ako syna
    else {
        //naviazeme doprava ak ma vacsi kluc
        if (strcmp(K->data, ((*RootPtr)->key.data)) > 0) {
            (*RootPtr)->RPtr = newitem;
            (*RootPtr) = newitem;
            //dolava ak je kluc mensi
        } else if (strcmp(K->data, ((*RootPtr)->key.data)) > 0) {
            (*RootPtr)->LPtr = newitem;
            (*RootPtr) = newitem;
        }
    }
    //navratenie ukazatela na BVS
    if (tmp != NULL){
        (*RootPtr) = tmp;
    }
    return SUCCESS;
}



void LTSDelete(LTSNode** RootPtr) {
   if(*RootPtr != NULL) {
      // rekurzivne odstranenie oboch podstromov
      LTSDelete(&((*RootPtr)->LPtr));
      LTSDelete(&((*RootPtr)->RPtr));
        
      // uvolnenie pamate kluca
      strFree(&((*RootPtr)->key));
      
      // ak je v strukture string
      if((*RootPtr)->data.varType == KW_STRING) {
         strFree(&((*RootPtr)->data.varValue.sVal));
      }

      // uvolnenie pamate uzla, uvolnenie pamate
      free(*RootPtr);
      *RootPtr = NULL;
   }
}

void GTSDelete(GTSNode** RootPtr) {
   if(*RootPtr != NULL) {
       // rekurzivne odstranenie oboch podstromov
      GTSDelete(&((*RootPtr)->LPtr));
      GTSDelete(&((*RootPtr)->RPtr));

       // uvolnenie pamate kluca
      strFree(&((*RootPtr)->key)); 
      
      // uvolnenie typu parametra, ak sa jedna o f-ciu
      if((*RootPtr)->data.varReserved == 0) {
         strFree(&((*RootPtr)->data.types));
      }

      // uvolnenie pamate LTS
      LTSDelete(&((*RootPtr)->data.lts)); 
      
      // uvolnenie pamate uzlu a nastavenie na NULL
      free(*RootPtr);
      *RootPtr = NULL;
   }
}


LTSNode* lts_getVarNode(GTSNode * ptrInGTS, tString* K){
    if(ptrInGTS != NULL) {
      LTSNode* lts = LTSSearch(ptrInGTS->data.lts, K);
      return lts;
   }
   else {
      return NULL;
   }
}





// int gts_setVariable(tGTS** rootPtr, TSNode* ptrInGTS, int argType, string* key)
// argType datovy typ promenne (ocekava konstanty KW_INTEGER, KW_DOUBLE, KW_STRING)
int gts_setVariable(GTSNode** RootPtr, GTSNode* ptrInGTS, int argType, tString* K){
    if(ptrInGTS == NULL) {
        fprintf(stderr, "CHYBA_SYM: Ukazatel na uzel glob. tabulky symbolu je NULL.\n");
        return INT_ERR;
   }
   
   int result;   

   // kontrola ci moze byt id parametra pouzity, tj. ze nie je rezervovany
   // pokus o vlozenie/rezervovanie identifikatora v GTS, pri nepodareni sa -> SEM_ERR
   if((result = GTSInsertVar(RootPtr, K)) != SUCCESS) {
      if(result == INT_ERR) {
         return INT_ERR;      
      }
      else {
         return SEM_ERR;
      }
   }
   
   int offset = ptrInGTS->data.argCount + ptrInGTS->data.varCount;
   
   // pridanie informacie o premennej do LTS a zvacsanie poctu parametrov
   if((result = LTSInsert(&(ptrInGTS->data.lts), K, argType, &offset)) == SUCCESS) {
      ptrInGTS->data.varCount += 1;
      return SUCCESS;  
   }

   // ak uz premenna/parameter je v LTS
   else if(result == ERROR_INSERTED) {
      return SEM_ERR;
   }
   // chyba pamate pri vkladani
   else {
      return INT_ERR;
   }
}



// tLTS* lts_setVarNode(tGTS** rootPtr, TSNode* ptrInGTS, int type);
// *ptrInGTS ukazatel na zaznam v GTS (uzel GTS)
LTSNode* lts_setVarNode(GTSNode** RootPtr, GTSNode* ptrInGTS, int type){

   tString new_str;
   strInit(&new_str);
   strAdd(&new_str, '$');
   int i = ptrInGTS->data.argCount + ptrInGTS->data.varCount;
   
   // vygenerovanie unikatneho nazvu v ramci funkcie
   while(i != 0) {
      strAdd(&new_str, (char) (i%10 + '0'));
      i = i/10;
   }
   // vlozenie noveho zaznamu o premennej do LTS danej f-cie
   gts_setVariable(RootPtr, ptrInGTS, type, &new_str);
   
   LTSNode* result = lts_getVarNode(ptrInGTS, &new_str);

   strFree(&new_str);
   
   // vracia ukazatel na novy uzol
   return result;
}




LTSNode* lts_setConst(tGTS** RootPtr,GTSNode* ptrInGTS, int type, tString* attr) {
   int tmpType;
   tString newitem;
   LTSNode* tmpNode;
   strInit(&newitem);
   strAdd(&newitem, '$');
   int i = ptrInGTS->data.argCount + ptrInGTS->data.varCount;

    // vygenerovanie unikatneho nazvu v ramci funkcie
   while(i != 0) {
      strAdd(&newitem, (char) (i%10 + '0'));
      i = i/10;
   }

   switch(type) {
      case LEX_NUMBER:
         tmpType = KW_INTEGER;
         break;
      case LEX_REAL:
         tmpType = KW_DOUBLE;
         break;
      case LEX_STRING:
         tmpType = KW_STRING;
         break;
   }


   // vlozenie noveho zaznamu o premenne do LTS
   gts_setVariable(RootPtr, ptrInGTS, tmpType, &newitem);
   
   tmpNode = lts_getVarNode(ptrInGTS, &newitem);
   
   char* tmpChar = getStr(attr);
   // pridani hodnoty
   switch(type) {
      case LEX_NUMBER:
         tmpNode->data.varValue.iVal = (int) strtol(tmpChar, NULL, 10);
         break;
      case LEX_REAL:
         tmpNode->data.varValue.dVal = strtod(getStr(attr), NULL);
         break;
      case LEX_STRING:
         strCopyStr(&(tmpNode->data.varValue.sVal), attr);
         break;
   }

   strFree(&newitem);

   return tmpNode;
}

// int gts_setVariable(tGTS** RootPtr, TSNode* ptrInGTS, int argType, string* key)
// argType datovy typ promenne (ocekava konstanty KW_INTEGER, KW_DOUBLE, KW_STRING)
int gts_setArgument(GTSNode** RootPtr, GTSNode* ptrInGTS, int argType, tString* K){
    if( ptrInGTS== NULL) {
        fprintf(stderr, "CHYBA_SYM: Ukazatel na uzel glob. tabulky symbolu je NULL.\n");
        return INT_ERR;
   }
   
   int result;   
   
   // kontrola jestli muze byt identifikator parametru pouzit (neni jiz rezervovan pro identif. fce)
   // pokus o vlozeni/rezervaci identif. v globalni TS, pri nezdaru - neni mozne ID pouzit => SEM_ERR
   if((result = GTSInsertVar(RootPtr, K)) != SUCCESS) {
      if(result == INT_ERR) {
         return INT_ERR;      
      }
      else {
         return SEM_ERR;
      }
   }
   
   // pridani informace o parametru do lokalni tabulky symbolu + navyseni citace parametru
   if((result = LTSInsert(&(ptrInGTS->data.lts), K, argType, &(ptrInGTS->data.argCount))) == SUCCESS) {
      // pridani informace o typu
      switch(argType) {
         case KW_INTEGER:
            strAdd(&(ptrInGTS->data.types), 'i');
            break;
         case KW_DOUBLE:
            strAdd(&(ptrInGTS->data.types), 'd');
            break;
         case KW_STRING:
            strAdd(&(ptrInGTS->data.types), 's');
            break;
      }
      return SUCCESS;  
   }
   // pokud uz dana promenna/parametr je v lokalni tabulce symbolu
   else if(result == ERROR_INSERTED) {
      return SEM_ERR;
   }
   // chyby pameti pri vkladani
   else {
      return INT_ERR;
   }
}

//int gts_checkArgs(TSNode* ptrInGTS, int argType, string* key, int i) 
// argType datovy typ promenne (ocekava konstanty KW_INTEGER, KW_DOUBLE, KW_STRING)
// i poradi parametru (offset v prog. zasobniku)
int gts_checkArgs(GTSNode* ptrInGTS, int argType, tString* K, int i){
     if(ptrInGTS == NULL) {
        fprintf(stderr, "CHYBA_SYM: Ukazatel na uzel glob. tabulky symbolu je NULL.\n");
        return INT_ERR;
   }
    
   LTSNode* tmp = LTSSearch(ptrInGTS->data.lts, K);
   
   if(tmp == NULL || tmp->data.varType != argType || tmp->data.varOffset != i) {
      return SEM_ERR;
   }
   else {
      return SUCCESS;
   }
}



// type datovy typ navratove hodnoty (ocekava konstanty KW_INTEGER, KW_DOUBLE, KW_STRING)
int gts_setReturnValue(GTSNode* RootPtr, int type){
    if(RootPtr == NULL) {
        fprintf(stderr, "CHYBA_SYM: Ukazatel na uzel glob. tabulky symbolu je NULL.\n");
        return INT_ERR;
   }
   RootPtr->data.retType = type;
   return SUCCESS;
}


//Funkce kontroluje, zda se je spravny navratovy typ funkce, ktera jiz  ma zaznam v globalni tabulce symbolu.
// type datovy typ navratove hodnoty (ocekava konstanty KW_INTEGER, KW_DOUBLE, KW_STRING)
// return SUCCESS, INT_ERR, SEM_ERR (vic parser.c)
// int gts_checkReturnValue(TSNode* ptrInGTS, int type) 
int gts_checkReturnValue(GTSNode* RootPtr, int type){
    if(RootPtr == NULL) {
        fprintf(stderr, "ial.c gts_check_retval(): Ukazatel na uzel glob. tabulky symbolu je NULL.\n");
        return INT_ERR;
   }
  
   if(RootPtr->data.retType == type)
      return SUCCESS;
   else
      return SEM_ERR;
}






// Nastavi do globalni TS priznak, ze dana funkce jiz byla definovana.
void gts_setDefinition(GTSNode* RootPtr){
    if(RootPtr == NULL) {
        fprintf(stderr, "CHYBA_SYM: Ukazatel na uzel glob. tabulky symbolu je NULL.\n");
        return;
   }
   RootPtr->data.def = 1;
}


// Zkontroluje, zda ma dana funkce nastaven priznak, ze jiz byla definovana.
int gts_checkDefinition(GTSNode* RootPtr){
    if(RootPtr == NULL) {
        fprintf(stderr, "CHYBA_SYM: Ukazatel na uzel glob. tabulky symbolu je NULL.\n");
        return INT_ERR;
   } 
   return RootPtr->data.def;
}

// Rekurzivne projde celou globalni tabulku a kontroluje, zda jsou vsechny vlozene funkce definovane.
int gts_checkAllDefinitions(GTSNode* RootPtr){
    int result;
    if(RootPtr != NULL) {
      // rekurzivni zanoreni do leveho a potom praveho podstromu
      if((result = gts_checkAllDefinitions(RootPtr->LPtr)) != SUCCESS)
         return result;
      if((result = gts_checkAllDefinitions(RootPtr->RPtr)) != SUCCESS)
         return result;
      
      // pokud se jedna o zaznam funkce a neni nastaven priznak definice funkce
      if(RootPtr->data.varReserved == 0 && RootPtr->data.def == 0) {
         return SEM_ERR;
      }      
          
      return SUCCESS;
   }
   return SUCCESS;
}



int lts_getVariableType(GTSNode *ptrInGTS, tString* key){
  //printf("Jsem v lts_getVarType\n");
    if(ptrInGTS != NULL) {
        LTSNode* localTS = LTSSearch(ptrInGTS->data.lts, key);
        if(localTS != NULL){
           int x = localTS->data.varType;
           //printf("VarType is %d\n", x);
           return localTS->data.varType;
        }
        else 
           return SEM_ERR;
    }
    else 
       return INT_ERR;
}

