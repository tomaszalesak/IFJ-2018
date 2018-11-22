
#ifndef PREC_AN_PREC_ANAL_H
#define PREC_AN_PREC_ANAL_H

#include <string.h>
#include "prec_dl.h"
#include "lexicalanalyzer.h"
#define PREC_TABLE_SIZE 14

#define PR_PLUS 0
#define PR_MINUS 1
#define PR_MULTIPLAY 2
#define PR_DIVISION 3
#define PR_LESS 4
#define PR_LESSEQUAL 5
#define PR_GREATER 6
#define PR_GREATEREQUAL 7
#define PR_EQUAL 8
#define PR_NOTEQUAL 9
#define PR_LEFTBRACKET 10
#define PR_RIGHTBRACKET 11
#define PR_TERM 12
#define PR_DOLLAR 13

#define PR_HANDLE 20
#define PR_E 30




static const char prec_table [PREC_TABLE_SIZE][PREC_TABLE_SIZE] = {

/*      		 +   -   *   /   <  <=   >  >=  ==  !=   (   )   T   $
/* 0 +  */	{ 	'>','>','<','<','>','>','>','>','>','>','<','>','<','>'	},
/* 1 -  */	{	'>','>','<','<','>','>','>','>','>','>','<','>','<','>'	},
/* 2 *  */	{	'>','>','>','>','>','>','>','>','>','>','<','>','<','>'	},
/* 3 /  */	{	'>','>','>','>','>','>','>','>','>','>','<','>','<','>'	},
/* 4 <  */	{	'<','<','<','<',' ',' ',' ',' ',' ',' ','<','>','<','>'	},
/* 5 <= */	{	'<','<','<','<',' ',' ',' ',' ',' ',' ','<','>','<','>'	},
/* 6 >  */	{	'<','<','<','<',' ',' ',' ',' ',' ',' ','<','>','<','>'	},
/* 7 >= */	{	'<','<','<','<',' ',' ',' ',' ',' ',' ','<','>','<','>'	},
/* 8 == */  {   '<','<','<','<',' ',' ',' ',' ',' ',' ','<','>','<','>' },
/* 9 != */	{	'<','<','<','<',' ',' ',' ',' ',' ',' ','<','>','<','>'	},
/* 10 ( */	{	'<','<','<','<','<','<','<','<','<','<','<','=','<',' '	},
/* 11 ) */	{	'>','>','>','>','>','>','>','>','>','>',' ','>',' ','>'	},
/* 12 T */	{	'>','>','>','>','>','>','>','>','>','>',' ','>',' ','>'	},
/* 13 $ */	{	'<','<','<','<','<','<','<','<','<','<','<',' ','<',' '	}
};


Token prec_anal(Token, Token, int);
tDLElemPtr topElem(tDLList *);
int isTerminal(int);
int tokenTypeToInt(TokenType);
tDLElemPtr findHandle(tDLList *);
Token giveMeToken(int, int *, Token, Token);
int firstTokenTypeEnd(TokenType);

#endif //PREC_AN_PREC_ANAL_H