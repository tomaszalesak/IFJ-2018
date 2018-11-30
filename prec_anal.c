/*
**	IFJ
**	Precedence analysis
**	---------------------------
**	Tomáš Zálešák - xzales13
*/


#include "prec_anal.h"
#include "lexicalanalyzer.h"
#include "errors.h"
#include "symtable.h"


Token prec_anal(Token t, Token t2, int give_me_old_tokens) {
    // init stack
    tDLList stack;
    DLInitList(&stack);

    ///semantic
    int ints = 0;
    int strings = 0;
    int *p_ints = &ints;
    int *p_strings = &strings;

    // push dollar
    DLInsertFirst(&stack, PR_DOLLAR);

    // a = top
    tDLElemPtr top_term = topElem(&stack);
    if (top_term == NULL) {
        // dispose stack
        DLDisposeList(&stack);
        compiler_exit(ERR_INTERNAL);
    }
    int a = top_term->data;

    // get token
    int token_no = 0;
    Token token = giveMeToken(give_me_old_tokens, &token_no, t, t2);

    // if first token type is EOF or EOL or THEN, exit
    if (firstTokenTypeEnd(token.type)) {
        // dispose stack
        DLDisposeList(&stack);
        compiler_exit(ERR_SYNTAX);
    }

    ///semantic
    ltsPrecVarCheck (token, p_strings, p_ints);

    // b = token from scanner
    int b = tokenTypeToInt(token.type);
    if (b == -1) {
        // dispose stack
        DLDisposeList(&stack);
        compiler_exit(ERR_SYNTAX);
    }

    // if '=' there cannot be comparing, exit
    if (give_me_old_tokens) {
        if (isComparing(b)) {
            // dispose stack
            DLDisposeList(&stack);
            compiler_exit(ERR_INCOMPATIBLE_TYPE);
        }
    }

    tDLElemPtr handle = NULL;

    while ((a != PR_DOLLAR) || (b != PR_DOLLAR)) {

        switch (prec_table[a][b]) {
            case '=':
                // push b
                DLInsertFirst(&stack, b);

                // get token
                token = giveMeToken(give_me_old_tokens, &token_no, t, t2);

                // todo tabulka
                ///semantic
                ltsPrecVarCheck (token, p_strings, p_ints);
                // end of todo

                break;
            case '<':
                // a -> a<
                stack.Act = top_term;
                DLPreInsert(&stack, PR_HANDLE);

                // push b
                DLInsertFirst(&stack, b);

                // get token
                token = giveMeToken(give_me_old_tokens, &token_no, t, t2);

                // todo tabulka
                ///semantic
                ltsPrecVarCheck (token, p_strings, p_ints);
                // end of todo

                break;
            case '>':
                handle = findHandle(&stack);
                if (handle != NULL) {
                    if (handle->lptr != NULL) {
                        if (handle->lptr->data == PR_TERM) {
                            if (handle->lptr == stack.First) {
                                fprintf(stderr, "E -> T\n");
                                DLDeleteFirst(&stack);
                                DLDeleteFirst(&stack);
                                DLInsertFirst(&stack, PR_E);
                            } else {
                                // dispose stack
                                DLDisposeList(&stack);
                                compiler_exit(ERR_SYNTAX);
                            }
                        } else {
                            if ((handle->lptr->lptr != NULL) && (handle->lptr->lptr->lptr == stack.First)) {
                                switch (handle->lptr->data) {
                                    case PR_LEFTBRACKET:
                                        if ((handle->lptr->lptr->data == PR_E) &&
                                            (handle->lptr->lptr->lptr->data == PR_RIGHTBRACKET)) {
                                            fprintf(stderr, "E -> ( E )\n");
                                            DLDeleteFirst(&stack);
                                            DLDeleteFirst(&stack);
                                            DLDeleteFirst(&stack);
                                            DLDeleteFirst(&stack);
                                            DLInsertFirst(&stack, PR_E);
                                        } else {
                                            // dispose stack
                                            DLDisposeList(&stack);
                                            compiler_exit(ERR_SYNTAX);
                                        }
                                        break;
                                    case PR_E:
                                        if (handle->lptr->lptr->lptr->data == PR_E) {
                                            switch (handle->lptr->lptr->data) {
                                                case PR_MULTIPLAY:
                                                    fprintf(stderr, "E -> E * E\n");
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLInsertFirst(&stack, PR_E);
                                                    break;
                                                case PR_DIVISION:
                                                    fprintf(stderr, "E -> E / E\n");
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLInsertFirst(&stack, PR_E);
                                                    break;
                                                case PR_PLUS:
                                                    fprintf(stderr, "E -> E + E\n");
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLInsertFirst(&stack, PR_E);
                                                    break;
                                                case PR_MINUS:
                                                    fprintf(stderr, "E -> E - E\n");
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLInsertFirst(&stack, PR_E);
                                                    break;
                                                case PR_EQUAL:
                                                    fprintf(stderr, "E -> E == E\n");
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLInsertFirst(&stack, PR_E);
                                                    break;
                                                case PR_NOTEQUAL:
                                                    fprintf(stderr, "E -> E != E\n");
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLInsertFirst(&stack, PR_E);
                                                    break;
                                                case PR_LESS:
                                                    fprintf(stderr, "E -> E < E\n");
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLInsertFirst(&stack, PR_E);
                                                    break;
                                                case PR_LESSEQUAL:
                                                    fprintf(stderr, "E -> E <= E\n");
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLInsertFirst(&stack, PR_E);
                                                    break;
                                                case PR_GREATEREQUAL:
                                                    fprintf(stderr, "E -> E >= E\n");
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLInsertFirst(&stack, PR_E);
                                                    break;
                                                case PR_GREATER:
                                                    fprintf(stderr, "E -> E > E\n");
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLDeleteFirst(&stack);
                                                    DLInsertFirst(&stack, PR_E);
                                                    break;
                                                default:
                                                    // dispose stack
                                                    DLDisposeList(&stack);
                                                    compiler_exit(ERR_SYNTAX);
                                            }
                                        } else {
                                            // dispose stack
                                            DLDisposeList(&stack);
                                            compiler_exit(ERR_SYNTAX);
                                        }

                                        break;
                                    default:
                                        // dispose stack
                                        DLDisposeList(&stack);
                                        compiler_exit(ERR_SYNTAX);
                                }
                            } else {
                                // dispose stack
                                DLDisposeList(&stack);
                                compiler_exit(ERR_SYNTAX);
                            }
                        }
                    } else {
                        // dispose stack
                        DLDisposeList(&stack);
                        compiler_exit(ERR_SYNTAX);
                    }
                } else {
                    // dispose stack
                    DLDisposeList(&stack);
                    compiler_exit(ERR_SYNTAX);
                }
                break;
            default:
                // dispose stack
                DLDisposeList(&stack);
                compiler_exit(ERR_SYNTAX);
        }

        // a = top
        top_term = topElem(&stack);
        if (top_term == NULL) {
            // dispose stack
            DLDisposeList(&stack);
            compiler_exit(ERR_INTERNAL);
        }
        a = top_term->data;

        // b = token from scanner
        b = tokenTypeToInt(token.type);
        if (b == -1) {
            // dispose stack
            DLDisposeList(&stack);
            compiler_exit(ERR_SYNTAX);
        }

        // if '=' there cannot be comparing, exit
        if (give_me_old_tokens) {
            if (isComparing(b)) {
                // dispose stack
                DLDisposeList(&stack);
                compiler_exit(ERR_INCOMPATIBLE_TYPE);
            }
        }
    }

    // dispose stack
    DLDisposeList(&stack);
    fprintf(stderr, "SUCCESS\n");
    return token;
}

tDLElemPtr topElem(tDLList *s) {
    DLFirst(s);
    int i = -1;

    while (DLActive(s)) {
        DLCopy(s, &i);
        if (isTerminal(i)) {
            return s->Act;
        }
        DLSucc(s);
    }

    fprintf(stderr, "NO TOP FOUND\n");
    return NULL;

}

tDLElemPtr findHandle(tDLList *s) {
    DLFirst(s);
    int i = -1;

    while (DLActive(s)) {
        DLCopy(s, &i);
        if (i == PR_HANDLE) {
            return s->Act;
        }
        DLSucc(s);
    }

    fprintf(stderr, "NO HANDLE FOUND\n");
    return NULL;
}

int isTerminal(int t) {
    switch (t) {
        case PR_PLUS:
        case PR_MINUS:
        case PR_MULTIPLAY:
        case PR_DIVISION:
        case PR_LESS:
        case PR_LESSEQUAL:
        case PR_GREATER:
        case PR_GREATEREQUAL:
        case PR_EQUAL:
        case PR_NOTEQUAL:
        case PR_LEFTBRACKET:
        case PR_RIGHTBRACKET:
        case PR_TERM:
        case PR_DOLLAR:
            return 1;
        default:
            return 0;
    }
}

int isComparing(int t) {
    switch (t) {
        case PR_LESS:
        case PR_LESSEQUAL:
        case PR_GREATER:
        case PR_GREATEREQUAL:
        case PR_EQUAL:
        case PR_NOTEQUAL:
            return 1;
        default:
            return 0;
    }
}

int tokenTypeToInt(TokenType t) {
    switch (t) {
        case OP_ADD:
            return PR_PLUS;
        case OP_SUB:
            return PR_MINUS;
        case OP_MUL:
            return PR_MULTIPLAY;
        case OP_DIV:
            return PR_DIVISION;
        case OP_LT:
            return PR_LESS;
        case OP_LTE:
            return PR_LESSEQUAL;
        case OP_GT:
            return PR_GREATER;
        case OP_GTE:
            return PR_GREATEREQUAL;
        case OP_EQ:
            return PR_EQUAL;
        case OP_NEQ:
            return PR_NOTEQUAL;
        case T_LBRACKET:
            return PR_LEFTBRACKET;
        case T_RBRACKET:
            return PR_RIGHTBRACKET;
        case T_IDENTIFIER:
        case T_INT:
        case T_FLOAT:
        case T_STRING:
        case KW_NIL:
            return PR_TERM;
        case KW_THEN:
        case T_EOL:
        case T_EOF:
        case KW_DO:
            return PR_DOLLAR;
        default:
            return -1;
    }

}

int firstTokenTypeEnd(TokenType t) {
    switch (t) {
        case KW_THEN:
        case T_EOL:
        case T_EOF:
        case KW_DO:
            return 1;
        default:
            return 0;
    }

}

Token giveMeToken(int give_old_token, int *no_of_token, Token t, Token t2) {
    if (give_old_token) {
        switch (*no_of_token) {
            case 0:
                *no_of_token = *no_of_token + 1;
                return t;
            case 1:
                *no_of_token = *no_of_token + 1;
                return t2;
            default:
                return getToken();
        }

    } else {
        return getToken();
    }
}
