/*
**	IFJ
**	Code generation
**	---------------------------
**	Vojtěch Novotný - xnovot1f
*/
#ifndef GEN_H
#define GEN_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "lexicalanalyzer.h"

#define GEN_COUNTER_ADD -1
#define GEN_COUNTER_RESET -2

/**
 * Generates a unique INT number. The numbers are not random, but they come in a sequence starting at INT_MIN.
 * @return A unique INT number.
 */
int gen_uniqueID_next();

/**
 * Gets the last uniqueUI that was generated.
 * @return The INT number.
 */
int gen_uniqueID_last();

/**
 * Generates code for 1 function argument.
 * @param token          - Contains the token that is used as the argument.
 * @param argumentNumber - Contains the position of the argument.
 * @param                   - Send positive number to set the last position to a positive number.
 * @param                   - Send GEN_COUNTER_ADD to increment the last used number and use it as position number.
 */
void gen_argument(Token token, int argumentNumber);

/**
 * Generates code to create a new temporary frame.
 */
void gen_TF();

/**
 * Generates code to define a new label.
 * @param token - token.data character string is used as the name for the label
 */
void gen_label(Token token);

/**
 * Generates code for 1 function parameter.
 * @param token           - Contains the token that is used as the parameter.
 * @param parameterNumber - Contains the position of the parameter.
 * @param                    - Send positive number to set the last position to a positive number.
 * @param                    - Send GEN_COUNTER_ADD to increment the last used number and use it as position number.
 */
void gen_parameter(Token token, int parameterNumber);

/**
 * Generates code that calls function.
 * @param token - Represents the function.
 */
void gen_call(Token token);

/**
 * Generates code for return value variable definition.
 */
void gen_retval();

/**
 * Generates code for return statement.
 */
void gen_return();

/**
 * Generates code that moves return value to the left-side variable of assignment.
 * @param token - Token representing the left-side variable.
 */
void gen_getretval(Token token);

/**
 * Generates code for pushing temporary frame onto the frame stack.
 */
void gen_pushframe();

/**
 * Generates code for poping top of the frame stack onto the temporary frame.
 */
void gen_popframe();

/**
 * Generates code that defines a new variable.
 * @param token - Token representing the variable that is to be defined.
 */
void gen_defvar(Token token);

/**
 * Generates control code based on the result of IF comparison.
 */
void gen_if_cmpResult();

/**
 * Generates else control label.
 * @param elseID - id used for the else label when generating if control code.
 */
void gen_if_elseLabel(int elseID);

/**
 * Generates end control label.
 * @param endID - id used for the end label when generating else control label.
 */
void gen_if_endLabel(int endID);

/**
 * Generates label to check while cycle condition.
 */
void gen_while_doLabel();

/**
 * Generates control code based on the result of WHILE comparison.
 */
void gen_while_cmpResult();

/**
 * Generates end control label.
 * @param endID - id used for the end label when generating while control code.
 * @param doID  - id of the do label made when generating while control code.
 */
void gen_while_endLabel(int endID, int doID);

// Built-in Function Code Generation
// =================================

/**
 * Generates code for built-in function INPUTS().
 */
void gen_bif_inputs(Token token);

/**
 * Generates code for built-in function INPUTI().
 */
void gen_bif_inputi(Token token);

/**
 * Generates code for built-in function INPUTF().
 */
void gen_bif_inputf(Token token);

/**
 * Generates code for built-in function PRINT().
 */
void gen_bif_print();

/**
 * Generates code for built-in function LENGTH().
 */
void gen_bif_length();

/**
 * Generates code for built-in function SUBSTR().
 */
void gen_bif_substr();

/**
 * Generates code for built-in function ORD().
 */
void gen_bif_ord();

/**
 * Generates code for built-in function CHR().
 */
void gen_bif_chr();

#endif // !GEN_H
