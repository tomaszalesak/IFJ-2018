/*
**	IFJ
**	Lexical analysis
**	---------------------------
**	Vojtěch Novotný - xnovot1f
*/

#ifndef LEX_H
#define LEX_H

#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "errors.h"

/// Enum defining different types of tokens.
typedef enum {          // token.data for the token.type (empty means no data)
    T_ERROR, // = 0,
	T_IDENTIFIER,       // string containing the name
	T_FUNCTION,         // string containing the name
	T_INT,              // int containing int literal value
	T_FLOAT,            // float containing float literal value
	T_STRING,           // char* containing string literal value
	T_LBRACKET,
	T_RBRACKET,
	T_COMMA,
	T_EOL,
	T_EOF,
	// Keywords:
    KW_DEF, // = 11,
    KW_DO,
    KW_ELSE,
    KW_END,
    KW_IF,
    KW_NOT,
    KW_NIL,
    KW_THEN,
    KW_WHILE,
    // Operators:
    OP_ADD, // = 20,
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
    BIF_INPUTS, // = 31,
    BIF_INPUTI,
    BIF_INPUTF,
    BIF_PRINT,
    BIF_LENGTH,
    BIF_SUBSTR,
    BIF_ORD,
    BIF_CHR,
    // Expression-Parser-Only Token types:
    PREC_E
} TokenType;


typedef enum {
    E_INVALID,
    E_MEMORY,
    E_NUMBER_TOO_BIG
} ErrorType;

/**
 * Token structure that is used as the return value of the the getToken function.
 */
typedef struct {
    /// @var type - Variable defining the type of the token.
	TokenType type;
    /// @var data - Pointer variable referencing any additional data a token might need.
	void * data;
} Token;

/**
 * Gets a sequence of characters from STDIN that corresponds to one word of the IFJ2018 programming language.
 * Function is based on a finite state machine.
 * @return - Token structure of information of about the word that is of the typedef struct Token type.
 * @return    Token contains int type that defines the type of token and
 * @return    void* data that contains any additional data about token.
 */
Token getToken();

void tester(Token token);

#endif // !LEX_H
