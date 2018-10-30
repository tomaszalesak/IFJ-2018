#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexicalanalyzer.h"

int main()
{
    Token token = getToken();

    while (token.type != T_EOF) {

        switch (token.type) {
        case T_INT:
            printf("integer:      %d\n", *(int*)(token.data));
            free(token.data);
            break;
        case T_FLOAT:
            printf("float:        %f\n", *(float*)(token.data));
            free(token.data);
            break;
        case T_STRING:
            printf("string:       %s\n", (char*)(token.data));
            free(token.data);
            break;
        case T_IDENTIFIER:
            printf("identifier:   %s\n", (char*)(token.data));
            free(token.data);
            break;
        case T_OPERATOR:
            printf("operator:     %d\n", *(int*)(token.data));
            free(token.data);
            break;
        case T_LBRACKET:
            printf("left_bracket\n");
            break;
        case T_RBRACKET:
            printf("right_bracket\n");
            break;
        case T_COMMA:
            printf("comma\n");
            break;
        case T_EOL:
            printf("EOL\n");
            break;
        case T_EOF:
            printf("EOF\n");
            break;
        case T_ERROR:
            return 1;
        default:
            printf("other value\n");
            break;
        }

        token = getToken();
    }

    return 0;
}
