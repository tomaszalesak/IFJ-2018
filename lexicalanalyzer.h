#pragma once
/*
**	IFJ
**	Lexical analysis
**	---------------------------
**	Vojtěch Novotný - xnovot1f
*/

#include <stdlib.h>
#include <stdio.h>
#include "string.h"

#ifndef LEX_H
#define LEX_H

/// Enum defining different types of tokens.
typedef enum {
	T_IDENTIFIER,       // data jsou string s názvem                X
	T_INT,              // data jsou int s hodnotou literálu        X
	T_FLOAT,            // data jsou float s hodnotou literálu      X
	T_STRING,           // data jsou string s hodnotou literálu     X
	T_LBRACKET,         // no data      X
	T_RBRACKET,         // no data      X
	T_COMMA,
	T_EOL,              // no data      X
	T_EOF,              // no data      X
	T_ERROR,             // no data
	// Keywords:
    KW_DEF,
    KW_DO,
    KW_ELSE,
    KW_END,
    KW_IF,
    KW_NOT,
    KW_NIL,
    KW_THEN,
    KW_WHILE,
    // Operators:
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
    OP_ASS,
    // Built-in Functions
    BIF_INPUTS,
    BIF_INPUTI,
    BIF_INPUTF,
    BIF_PRINT,
    BIF_LENGTH,
    BIF_SUBSTR,
    BIF_ORD,
    BIF_CHR
} TokenType;


typedef enum {
    E_INVALID,
    E_MEMORY,
    E_NUMBER_TOO_BIG,
} ErrorType;

/// Token structure that is used as the return value of the the getToken function.
typedef struct {
	TokenType type;// typy tokenů
	void * data;
} Token;

/// Reads a sequence of characters from STDIN that corresponds to one word of the IFJ2018 language.
/// returns     Token structure that contains information about the word.
Token getToken();

#endif // !LEX_H
