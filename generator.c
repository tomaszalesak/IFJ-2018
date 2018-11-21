/*
**	IFJ
**	Code generation
**	---------------------------
**	Vojtěch Novotný - xnovot1f
*/

#include "generator.h"

inline int gen_parameter(Token token, int cisloParametru) {
    printf("DEFVAR TF@%d\nMOVE TF@%d ", cisloParametru, cisloParametru);

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
            printf("int@%s\n", (char*)token.data);
            free(token.data);
            break;
        default:
            printf("WHEEE\n");
            break;
    }

    return 0;
}

inline int gen_TF() {
    printf("CREATE FRAME\n");

    return 0;
}
