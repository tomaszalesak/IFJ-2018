/*
**	IFJ
**	Code generation
**	---------------------------
**	Vojtěch Novotný - xnovot1f
*/

#include "generator.h"

/// Generates code for 1 function argument.
/// token           Contains the token that is used as the argument.
/// argumentNumber  Contains the position of the argument.
///                     Send positive number to set the last position to a positive number.
///                     Send GEN_COUNTER_ADD to increment the last used number and use it as position number.
inline void gen_argument(Token token, int argumentNumber) {

    static int argumentCounter = 0;

    if (argumentNumber == GEN_COUNTER_ADD)
        argumentCounter++;
    else
        argumentCounter = argumentNumber;


    printf("DEFVAR TF@%d\nMOVE TF@%d ", argumentNumber, argumentNumber);

    switch (token.type) {
        case T_IDENTIFIER:
            printf("neaka hodnota z TS\n");
            break;
        case T_INT:
            printf("int@%d\n", *(int*)(token.data));
            free(token.data);
            break;
        case T_FLOAT:
            printf("int@%f\n", *(float*)(token.data));
            free(token.data);
            break;
        case T_STRING:
            printf("int@%s\n", (char*)(token.data));
            free(token.data);
            break;
        default:
            printf("WHEEE\n");
            break;
    }
}

/// Generates code to create new temporary frame.
inline void gen_TF() {
    printf("CREATE FRAME\n");
}

/// Generates code to define a new label.
/// token           token.data character string is used as the name for the label
inline void gen_label(Token token) {
    printf("LABEL $%s\n", (char*)token.data);
}


inline void gen_parameter(Token token, int parameterNumber) {

    static int parameterCounter = 0;

    if (parameterNumber == GEN_COUNTER_ADD)
        parameterCounter++;
    else
        parameterCounter = parameterNumber;

    printf("DEFVAR LF@%s\nMOVE LF@%s LF@%d ", (char*)(token.data), (char*)(token.data), parameterNumber);
}

/// Generates code for return value variable definition.
inline void get_retval() {
    printf("DEFVAR LF@%retval\nMOVE LF@%retval nil@nil");
}
