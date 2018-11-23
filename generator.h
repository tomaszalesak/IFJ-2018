#pragma once
/*
**	IFJ
**	Code generation
**	---------------------------
**	Vojtěch Novotný - xnovot1f
*/

#include <stdlib.h>
#include <stdio.h>
#include "lexicalanalyzer.h"

#ifndef GEN_H
#define GEN_H

#define GEN_COUNTER_ADD -1

/// Generates code for 1 function argument.
/// token           Contains the token that is used as the argument.
/// argumentNumber  Contains the position of the argument.
///                     Send positive number to set the last position to a positive number.
///                     Send GEN_COUNTER_ADD to increment the last used number and use it as position number.
inline void gen_argument(Token token, int argumentNumber);

/// Generates code to create new temporary frame.
inline void gen_TF();

/// Generates code to define a new label.
/// token           token.data character string is used as the name for the label
inline void gen_label(Token token);

/// Generates code for 1 function parameter.
/// token           Contains the token that is used as the parameter.
/// parameterNumber  Contains the position of the parameter.
///                     Send positive number to set the last position to a positive number.
///                     Send GEN_COUNTER_ADD to increment the last used number and use it as position number.
inline void gen_parameter(Token token, int parameterNumber);

/// Generates code for return value variable definition.
inline void get_retval();

#endif // !GEN_H
