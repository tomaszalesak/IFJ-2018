#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LexicalAnalyzer.h"

int main()
{
    Token token = getToken();

    while (token.type != T_EOF) {
        printf("Token type is: %d\t", token.type);

        switch (token.type) {
        case T_INT:
            printf("message is: %d\n", *(int*)(token.data));
            free(token.data);
            break;
        case T_FLOAT:
            printf("message is: %f\n", *(float*)(token.data));
            free(token.data);
            break;
        case T_STRING:
            printf("message is: %s\n", (char*)(token.data));
            free(token.data);
            break;
        case T_IDENTIFIER:
            printf("message is: %s\n", (char*)(token.data));
            free(token.data);
            break;
        case T_OPERATOR:
            printf("message is: %d\n", *(int*)(token.data));
            free(token.data);
            break;
        case T_LBRACKET:
            printf("message is: (\n");
            break;
        case T_RBRACKET:
            printf("message is: )\n");
            break;
        case T_COMMA:
            printf("message is: ,\n");
            break;
        case T_EOL:
            printf("message is: EOL\n");
            break;
        case T_EOF:
            printf("message is: EOF\n\n");
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
