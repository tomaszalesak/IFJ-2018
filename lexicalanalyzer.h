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
typedef enum {          // token.data for the token.type (empty means no data)
    T_ERROR, // = 0,
	T_IDENTIFIER,       // string containing the name
	T_INT,              // int containing int literal value
	T_FLOAT,            // float containing float literal value
	T_STRING,           // char* containing string literal value
	T_LBRACKET,
	T_RBRACKET,
	T_COMMA,
	T_EOL,
	T_EOF,
	// Keywords:
    KW_DEF, // = 10,
    KW_DO,
    KW_ELSE,
    KW_END,
    KW_IF,
    KW_NOT,
    KW_NIL,
    KW_THEN,
    KW_WHILE,
    // Operators:
    OP_ADD, // = 19,
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
    BIF_INPUTS, // = 30,
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
