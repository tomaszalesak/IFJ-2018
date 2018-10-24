#pragma once
/*
**	IFJ
**	Lexical analysis
**	---------------------------
**	Vojtìch Novotný - xnovot1f
*/

#include <stdlib.h>
#include <stdio.h>

#ifndef LEX_H
#define LEX_H

/// Enum defining different types of tokens.
typedef enum {
	T_IDENTIFIER,       // data jsou string s názvem
	T_KEYWORD,          // data jsou enum toho slova KeywordType
	T_INT,              // data jsou int s hodnotou literálu
	T_FLOAT,            // data jsou float s hodnotou literálu
	T_STRING,           // data jsou string s hodnotou literálu
	T_OPERATOR,         // data jsou enum operátorù OperatorType
	T_LBRACKET,         // no data
	T_RBRACKET,         // no data
	T_NIL,              // no data
	T_EOL,              // no data
	T_EOF,              // no data
	T_ERROR             // no data
} TokenType;

/// Enum defining different types of keywords for the T_KEYWORD token type.
typedef enum {
    KW_DEF,
    KW_DO,
    KW_ELSE,
    KW_END,
    KW_IF,
    KW_NOT,
    KW_NIL,
    KW_THEN,
    KW_WHILE
} KeywordType;

/// Enum defining different types of operators for the T_OPERATOR token type.
typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_LT,
    OP_GT,
    OP_LTE,
    OP_GTE,
    OP_EQ,
    OP_NEQ,
    OP_ASS
} OperatorType;

/// Token structure that is used as the return value of the the getToken function.
typedef struct {
	TokenType type;// typy tokenù
	void * data;
} Token;

/// Reads a sequence of characters from STDIN that corresponds to one word of the IFJ2018 language.
/// returns     Token structure that contains information about the word.
Token getToken();

#endif // !LEX_H
