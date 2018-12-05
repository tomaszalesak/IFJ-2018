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
#include "errors.h"

#define GEN_COUNTER_ADD -1
#define GEN_COUNTER_RESET -2

/// @def GEN_GLOBAL - Parameter value for function gen_set_frame(int frame). Represents global frame GF.
#define GEN_GLOBAL 0
/// @def GEN_LOCAL - Parameter value for function gen_set_frame(int frame). Represents local frame LF.
#define GEN_LOCAL 1

// Generation Control Functions
// ============================

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
 * Sets the frame to either local or global. The frame is then used in variable operations.
 * @param frame_type - Int representing the frame. Can be either GEN_GLOBAL or GEN_LOCAL.
 */
void gen_set_frame(int frame_type);


// Function And Control Structure Code Generation
// ==============================================

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
void gen_retval_def();

/**
 * Generates code for assingning return value into the return value.
 * @param token - Token that contains the return value.
 */
void gen_retval_ass(Token token);

/**
 * Generates code for return statement.
 */
void gen_return();

/**
 * Generates code that moves return value of a function to the target variable.
 * Called after the end of function call.
 * @param token - Token representing the target variable.
 */
void gen_retval_get(Token token);

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

/**
 * Generates IFJcode2018 file header.
 */
void gen_code_header();


int gen_jumparound_jump();

void gen_jumparound_label(int label);


// Expression Code Generation
// ==========================

/**
 * Generates code to move a value from one variable to another variable.
 * @param from - Token representing source variable.
 * @param to   - Token representing target variable.
 */
void gen_exp_MOV(Token from, Token to);

/**
 * Generates code for one argument of an expression.
 * @param token - Token containing information about the argument.
 */
void gen_exp_putArg(Token token);

/**
 * Generates a newline at the end of an expression.
 */
void gen_exp_finalize();

/**
 * Generates beginning code for a "x = y * z" type expression.
 * Defines variable for storing the result and defines the operation.
 * @return     UID of variable x, that will store the result of the expression.
 */
int gen_exp_MUL();

/**
 * Generates beginning code for a "x = y / z" type expression.
 * Defines variable for storing the result and defines the operation.
 * @return     UID of variable x, that will store the result of the expression.
 */
int gen_exp_DIV();

/**
 * Generates beginning code for a "x = y + z" type expression.
 * Defines variable for storing the result and defines the operation.
 * @return     UID of variable x, that will store the result of the expression.
 */
int gen_exp_ADD(Token t1, Token t2);

/**
 * Generates beginning code for a "x = y - z" type expression.
 * Defines variable for storing the result and defines the operation.
 * @return     UID of variable x, that will store the result of the expression.
 */
int gen_exp_SUB();

/**
 * Generates the final result variable of one line of code.
 */
void gen_exp_result(Token token);

/**
 * Generates beginning code for a "x = y == z" type expression.
 * Defines variable for storing the result and defines the operation.
 * @return     UID of variable x, that will store the result of the expression.
 */
int gen_exp_EQ();

/**
 * Generates beginning code for a "x = !z" type expression.
 * Defines variable for storing the result and defines the operation.
 * @return     UID of variable x, that will store the result of the expression.
 */
int gen_exp_NOT();

/**
 * Generates beginning code for a "x = y < z" type expression.
 * Defines variable for storing the result and defines the operation.
 * @return     UID of variable x, that will store the result of the expression.
 */
int gen_exp_LT();

/**
 * Generates beginning code for a "x = y > z" type expression.
 * Defines variable for storing the result and defines the operation.
 * @return     UID of variable x, that will store the result of the expression.
 */
int gen_exp_GT();

/**
 * Generates code that assigns result of an expression to the target variable.
 * Called after end of expression analysis.
 * @param token    - Token representing the target variable.
 * @var   resultID - ID of the result.
 */
void gen_result_ass(Token token);


// Built-in Function Code Generation
// =================================

/**
 * Generates code for built-in function INPUTS().
 */
void gen_bif_inputs();

/**
 * Generates code for built-in function INPUTI().
 */
void gen_bif_inputi();

/**
 * Generates code for built-in function INPUTF().
 */
void gen_bif_inputf();

/**
 * Generates code for built-in function PRINT().
 * @param args_count - Int with number of arguments of the print function.
 */
void gen_bif_print(int args_count);

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
