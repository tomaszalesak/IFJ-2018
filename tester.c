
#include "lexicalanalyzer.h"

void tester(Token token)
{
        switch (token.type) {
            case T_INT:
                printf("%d ", *(int *) (token.data));
                free(token.data);
                break;
            case T_FLOAT:
                printf("%f ", *(float *) (token.data));
                free(token.data);
                break;
            case T_STRING:
                printf("%s ", (char *) (token.data));
                free(token.data);
                break;
            case T_IDENTIFIER:
                printf("%s(id) ", (char *) (token.data));
                free(token.data);
                break;
            case T_OPERATOR:
                printf("%d ", *(int *) (token.data));
                free(token.data);
                break;
            case T_LBRACKET:
                printf("( ");
                break;
            case T_RBRACKET:
                printf(") ");
                break;
            case T_COMMA:
                printf(", ");
                break;
            case T_EOL:
                printf("EOL\n");
                break;
            case T_EOF:
                printf("EOF\n");
                break;
            case T_ERROR:
                printf("\n!!!LEX SENDED T_ERROR TOKEN!!!\n");
                break;
            case KW_DEF:
                printf("def ");
                break;
            case KW_DO:
                printf("do ");
                break;
            case KW_ELSE:
                printf("else ");
                break;
            case KW_END:
                printf("end ");
                break;
            case KW_IF:
                printf("if ");
                break;
            case KW_NOT:
                printf("not ");
                break;
            case KW_NIL:
                printf("NIL ");
                break;
            case KW_THEN:
                printf("then ");
                break;
            case KW_WHILE:
                printf("while ");
                break;
            case OP_ADD:
                printf("+ ");
                break;
            case OP_SUB:
                printf("- ");
                break;
            case OP_MUL:
                printf("* ");
                break;
            case OP_DIV:
                printf("/ ");
                break;
            case OP_LT:
                printf("< ");
                break;
            case OP_GT:
                printf("> ");
                break;
            case OP_LTE:
                printf("<= ");
                break;
            case OP_GTE:
                printf(">= ");
                break;
            case OP_EQ:
                printf("== ");
                break;
            case OP_NEQ:
                printf("!= ");
                break;
            case OP_ASS:
                printf("= ");
                break;
            default:
                printf("other value\n");
                break;
        }
}