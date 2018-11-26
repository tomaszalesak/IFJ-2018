/*
**	IFJ
**	Syntax analysis
**	---------------------------
**	Robin Skaličan - xskali19
*/

#include <string.h>
#include "lexicalanalyzer.h"
#include "symtable.h"
#include "prec_anal.h"
#include "errors.h"

void parse_main();

void parse_function();

void parse_st_list(int position_helper);

int parse_param();

void parse_param_list_1();

void parse_param_list_2();

int parse_arg(int token_type);

void parse_arg_list2();

void parse_arg_list2b();

void parse_arg_list_switcher(int print_checker);

Token token;// TODO extend to my .h or not?
int paramsCounter = 0;
GTSNodePtr gts;
string K;
LTSNodePtr ltsMain;
LTSNodePtr ltsFunc;

//Parse for <main> LL
void parse_main() {
    token = getToken();

    switch (token.type) {

        case KW_DEF://3
            parse_function();
            break;

        case T_EOL:
            break;

        case T_EOF: // 7 TODO End of INPUT, check SEM Analysis etc.
            exit(0);

            //Unique options for <main> checked, now goes into <stat> to check rest
        default :
            parse_st_list(4);
            break;
    }
    parse_main();//Calling myself, stopped by T_EOF or error EXIT
}

//Parse for <func> LL
void parse_function() {//3// TODO Define function with no brackets?

    token = getToken();

    if ((token.type == T_IDENTIFIER) && (getToken().type) == T_LBRACKET) {
        //Call function for <param-l>

        //semantic
        string K = createString(token);
        //check if function was already inserted into gts
        if (gtsSearch(gts, &K) != NULL) {
            //if it was inserted, it cannot be defined
            if (gtsCheckIfDefined(gts, &K)) {
                compiler_exit(ERR_UNDEF_REDEF);
            }
        }
            //if it wasn't inserted
        else {
            gtsInsert(&gts, &K);
        }
        gtsSetDefined(gts, &K);

        parse_param_list_1();
        gtsSetParamCount(gts, &K, paramsCounter);
        paramsCounter = 0;
        if ((getToken().type) == T_EOL) {
            //Call function for <st-list>
            parse_st_list(2);//TODO Check this
        } else {
            compiler_exit(ERR_SYNTAX);
        }
    } else {
        compiler_exit(ERR_SYNTAX);
    }
}

//Parse for <st-list> LL
//position_helper used for check if get new token and go back to main(4) and if its in if(0) or in else(2) part
void parse_st_list(int position_helper) {

    if (position_helper != 4) {
        token = getToken();
    } else {
        position_helper = 0;
    }
    Token token_old = token;
    switch (token.type) {

        case T_IDENTIFIER:// 16 17 27

            //semantic
            K = createString(token);
            ltsInsert(&ltsMain, &K);
            //gtsSearch(gts, &K);

            token = getToken();

            switch (token.type) {

                case OP_ASS://16 27
                    token = getToken();
                    token_old = token;
                    switch (token.type) {

                        case T_IDENTIFIER: //27 -> 28 29 !TRAP! I do not know if its function or identifier
                            //semantic - get identifier name/value
                            K = createString(token);
                            token = getToken();
                            switch (token.type) {

                                case T_LBRACKET:
                                case T_IDENTIFIER:
                                case T_EOL:
                                    parse_arg_list_switcher(0);
                                    break;

                                default:
                                    token = prec_anal(token_old, token, 1);
                                    break;
                            }
                            break;

                            //BIF Handling
                        case BIF_INPUTS://32
                        case BIF_INPUTI://33
                        case BIF_INPUTF://34
                            if ((token = getToken()).type == T_LBRACKET) {
                                if ((getToken().type) == T_RBRACKET) {
                                    break;//TODO Check this
                                } else {
                                    compiler_exit(ERR_SYNTAX);
                                }
                            } else if (token.type == T_EOL) {
                                break;
                            } else {
                                compiler_exit(ERR_SYNTAX);
                            }

                            //semantic for BIF
                        case BIF_PRINT://35 TODO PRINT!!!
                            K.str = "print";
                            token = getToken();
                            parse_arg_list_switcher(1);
                            break;

                        case BIF_LENGTH://37 //TODO Check STRING
                            //semantic set variable type
                            ltsSetIdType(ltsMain, &K, T_INT);
                            K.str = "length";
                            token = getToken();
                            parse_arg_list_switcher(0);
                            break;

                        case BIF_SUBSTR://38 //TODO Check STRING,INT,INT
                            //semantic set variable type
                            ltsSetIdType(ltsMain, &K, T_STRING);
                            K.str = "substr";
                            token = getToken();
                            parse_arg_list_switcher(0);
                            break;

                        case BIF_ORD://39 //TODO Check STRING,INT
                            //semantic set variable type
                            ltsSetIdType(ltsMain, &K, T_INT);
                            K.str = "ord";
                            token = getToken();
                            parse_arg_list_switcher(0);
                            break;

                        case BIF_CHR://40  //TODO Check INT
                            //semantic set variable type
                            ltsSetIdType(ltsMain, &K, T_STRING);
                            K.str = "chr";
                            token = getToken();
                            parse_arg_list_switcher(0);
                            break;

                        default: //16
                            ltsSetIdType(ltsMain, &K, token.type);
                            token_old = token;
                            token = getToken();
                            token = prec_anal(token_old, token, 1);
                    }

                    /* if (token.type != T_EOL) {//Solved by precedence right?
                         compiler_exit(ERR_SYNTAX);
                     }*/
                    //If position_helper is 4, which means its call from main, it goes back
                    if (position_helper == 0) {
                        parse_st_list(position_helper);
                    } else if (position_helper == 2) {
                        parse_st_list(position_helper);
                    }
                    break;

                case T_EOL:// 17
                    //token = getToken(); //TODO What is this and why doesn't it check KW_END?
                    break;

                case OP_ADD:
                case OP_SUB:
                case OP_MUL:
                case OP_DIV:
                    token = prec_anal(token_old, token, 1);
                    break;

                default://11 12
                    parse_arg_list_switcher(0);
                    break;
            }
            break;

        case T_EOL:// 6
            break;

        case KW_IF:// 19
            token = prec_anal(token, token, 0);
            if ((token.type == KW_THEN) && (getToken().type) == T_EOL) {
                parse_st_list(0);
                parse_st_list(2);
            } else {
                compiler_exit(ERR_SYNTAX);
            }
            break;

        case KW_WHILE:// 24
            token = prec_anal(token, token, 0);
            if ((token.type == KW_DO) && (getToken().type) == T_EOL) {
                parse_st_list(2);
                if ((getToken().type) != T_EOL) {
                    compiler_exit(ERR_SYNTAX);
                }
            } else {
                compiler_exit(ERR_SYNTAX);
            }
            break;

        case KW_ELSE:
            if (position_helper != 0) {
                compiler_exit(ERR_SYNTAX);
            } else if (getToken().type != T_EOL) {
                compiler_exit(ERR_SYNTAX);
            }
            break;

        case KW_END:
            if (position_helper <= 0) {
                compiler_exit(ERR_SYNTAX);
            } else if (getToken().type != T_EOL) {
                compiler_exit(ERR_SYNTAX);
            }
            break;

            //BIF Handling
        case BIF_INPUTS://32
        case BIF_INPUTI://33
        case BIF_INPUTF://34
            if (((token = getToken()).type) == T_LBRACKET) {
                if (((getToken().type) == T_RBRACKET) &&
                    ((((token = getToken()).type) == T_EOL) || ((token.type) == T_EOF))) {
                    parse_st_list(position_helper);
                    break;//TODO Check this
                } else {
                    compiler_exit(ERR_NO_OF_ARGS);
                }
            } else if (token.type == T_EOL) {
                parse_st_list(position_helper);
                break;
            } else {// TODO SEMANTIC TODO, THIS DOES NOT COUNT WITH BAD NUMBER OF ARGS
                compiler_exit(ERR_SYNTAX);
            }

            //semantic for BIF
        case BIF_PRINT://35 TODO PRINT!!!
            K.str = "print";
            token = getToken();
            parse_arg_list_switcher(1);
            break;

        case BIF_LENGTH://37 //TODO Check STRING
            K.str = "length";
            token = getToken();
            parse_arg_list_switcher(0);
            break;

        case BIF_SUBSTR://38 //TODO Check STRING,INT,INT
            K.str = "substr";
            token = getToken();
            parse_arg_list_switcher(0);
            break;

        case BIF_ORD://39 //TODO Check STRING,INT
            K.str = "ord";
            token = getToken();
            parse_arg_list_switcher(0);
            break;

        case BIF_CHR://40  //TODO Check INT
            K.str = "chr";
            token = getToken();
            parse_arg_list_switcher(0);
            break;

        default:
            token = getToken();
            token = prec_anal(token_old, token, 1);
    }
}

//Parse for <param-l> LL
void parse_param_list_1() {//58
    //Check for if ( ) validation
    if (!parse_param()) {
        parse_param_list_2();
    }
}

//Parse for <param-l2> LL
void parse_param_list_2() {
    token = getToken();
    if (token.type == T_COMMA) {//59
        parse_param();
        parse_param_list_2();
    } else if (token.type != T_RBRACKET) {
        compiler_exit(ERR_SYNTAX);
    }//60
}

//Parse for <param> LL
int parse_param() {//61
    token = getToken();
    if (token.type == T_RBRACKET) {
        return 1;
    } else if (token.type != T_IDENTIFIER) { //TODO add checks for string int float if(ifValid)
        compiler_exit(ERR_SYNTAX);
    }
    paramsCounter++;
    return 0;
}

//Function for choosing, if send it to <arg-list2> or <arg-list2b>
//int print_checker used for check if called from print, then it cant be without arguments
void parse_arg_list_switcher(int print_checker) {

    if (token.type == T_LBRACKET) {
        //Solves <arg-listb> LL
        token = getToken();
        if (!parse_arg(token.type)) {//47

            //semantic control of param types
            //if (token.type == T_INT)


            //Call function for <arg-list2b>
            parse_arg_list2b();//44,45
        }

        //added for semantic analysis
        //check if the function call has correct number of parameters
        if (print_checker != 1) {
            if (gtsGetParamCount(gts, &K) != paramsCounter) {
                fprintf(stderr,
                        "ERROR! Bad number of arguments for function %s!\nExpected %d parameters but %d have been inserted.\n",
                        K.str, gtsGetParamCount(gts, &K), paramsCounter);
                exit(ERR_NO_OF_ARGS);
            }
        }
        //reset params counter for another func. check
        paramsCounter = 0;

        if (getToken().type != T_EOL) {
            compiler_exit(ERR_SYNTAX);//TODO Check this
        }

    } else if (token.type != T_EOL) {
        //Solves <arg-list> LL
        parse_arg(token.type);//43
        //Call function for <arg-list2>
        parse_arg_list2();//48,49
        //solves BIF without brackets (hopefully)
        paramsCounter = 0;
    } else if (print_checker == 1) {//35
        compiler_exit(ERR_SYNTAX);
    } else {
//semantic analysis for function without brackets/variable
        if (gtsSearch(gts, &K) == NULL) {
//if (ltsSearch()) TODO add LTS support
        } else {
            if (gtsGetParamCount(gts, &K) != paramsCounter) {
                fprintf(stderr,
                        "ERROR! Bad number of arguments for function %s!\nExpected %d parameters but %d have been inserted.\n",
                        K.str, gtsGetParamCount(gts, &K), paramsCounter);
                exit(ERR_NO_OF_ARGS);
            }
        }
    }
}

//Parse for <arg> LL
int parse_arg(int token_type) {
    //TODO Make this one better / Checks if token is taken or should take
    if (token_type == 399) {
        token = getToken();
    }

    switch (token.type) {
        case T_IDENTIFIER://51
            paramsCounter++;

            //semantic BIF params check TODO put it onto function maybe?!
            //BIF chr
            if (strcmp(K.str, "chr") == 0) {
                string a = createString(token);
                //chr accepts 1 integer parameter
                if (paramsCounter != 1) {
                    fprintf(stderr, "ERROR! Function %s!\n", K.str);
                    compiler_exit(ERR_NO_OF_ARGS);
                }
                if ((ltsGetIdType(ltsMain, &a) != T_INT) && (ltsGetIdType(ltsMain, &a) != T_FLOAT)) {
                    fprintf(stderr, "Semantic Error! Bad parameter type for function %s!\n", K.str);
                    compiler_exit(ERR_INCOMPATIBLE_TYPE);
                }
            }

            //BIF ord
            if (strcmp(K.str, "ord") == 0) {
                string a = createString(token);
                switch (paramsCounter) {
                    case 1:
                        if (ltsGetIdType(ltsMain, &a) != T_STRING) {
                            fprintf(stderr, "ERROR! Function %s!\n", K.str);
                            compiler_exit(ERR_INCOMPATIBLE_TYPE);
                        }
                        break;
                    case 2:
                        if ((ltsGetIdType(ltsMain, &a) != T_INT) && (ltsGetIdType(ltsMain, &a) != T_FLOAT)) {
                            fprintf(stderr, "ERROR! Function %s!\n", K.str);
                            compiler_exit(ERR_INCOMPATIBLE_TYPE);
                        }
                        break;
                    default:
                        fprintf(stderr, "ERROR! Function %s! Expected %d parameters but %d have been inserted!\n",
                                K.str, 2, paramsCounter);
                        compiler_exit(ERR_NO_OF_ARGS);
                        break;
                }
            }

            //BIF length
            if (strcmp(K.str, "length") == 0) {
                string a = createString(token);
                //chr accepts 1 integer parameter
                if (paramsCounter != 1) {
                    fprintf(stderr, "ERROR! Function %s!\n", K.str);
                    compiler_exit(ERR_NO_OF_ARGS);
                }
                if (ltsGetIdType(ltsMain, &a) != T_STRING) {
                    fprintf(stderr, "ERROR! Function %s!\n", K.str);
                    compiler_exit(ERR_INCOMPATIBLE_TYPE);
                }
            }

            //BIF substr
            if (strcmp(K.str, "substr") == 0) {
                string a = createString(token);
                switch (paramsCounter) {
                    case 1:
                        if (ltsGetIdType(ltsMain, &a) != T_STRING) {
                            fprintf(stderr, "ERROR! Function %s!\n", K.str);
                            compiler_exit(ERR_INCOMPATIBLE_TYPE);
                        }
                        break;
                    case 2:
                        if (ltsGetIdType(ltsMain, &a) != T_INT && ltsGetIdType(ltsMain, &a) != T_FLOAT) {
                            fprintf(stderr, "ERROR! Function %s!\n", K.str);
                            compiler_exit(ERR_INCOMPATIBLE_TYPE);
                        }
                        break;
                    case 3:
                        if (ltsGetIdType(ltsMain, &a) != T_INT && ltsGetIdType(ltsMain, &a) != T_FLOAT) {
                            fprintf(stderr, "ERROR! Function %s!\n", K.str);
                            compiler_exit(ERR_INCOMPATIBLE_TYPE);
                        }
                        break;
                    default:
                        fprintf(stderr, "ERROR! Function %s! Expected %d parameters but %d have been inserted!\n",
                                K.str, 3, paramsCounter);
                        compiler_exit(ERR_NO_OF_ARGS);
                        break;
                }
            }
            break;
        case T_INT://52
            paramsCounter++;

            //BIF ord
            if (strcmp(K.str, "ord") == 0) {
                //string a = createString(token);
                switch (paramsCounter) {
                    case 1:
                        fprintf(stderr, "ERROR! Function %s!\n", K.str);
                        compiler_exit(ERR_INCOMPATIBLE_TYPE);
                        break;
                    case 2:
                        break;
                    default:
                        fprintf(stderr, "ERROR! Function %s! Expected %d parameters but %d have been inserted!\n",
                                K.str, 2, paramsCounter);
                        compiler_exit(ERR_NO_OF_ARGS);
                        break;
                }
            }

            //BIF length
            if (strcmp(K.str, "length") == 0) {
                fprintf(stderr, "ERROR! Function %s!\n", K.str);
                compiler_exit(ERR_INCOMPATIBLE_TYPE);
            }

            //BIF substr
            if (strcmp(K.str, "substr") == 0) {
                //string a = createString(token);
                switch (paramsCounter) {
                    case 1:
                        fprintf(stderr, "ERROR! Function %s!\n", K.str);
                        compiler_exit(ERR_INCOMPATIBLE_TYPE);
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    default:
                        fprintf(stderr, "ERROR! Function %s! Expected %d parameters but %d have been inserted!\n",
                                K.str, 3, paramsCounter);
                        compiler_exit(ERR_NO_OF_ARGS);
                        break;
                }
            }
            break;

        case T_FLOAT://53 TODO
            paramsCounter++;

            //BIF ord
            if (strcmp(K.str, "ord") == 0) {
                //string a = createString(token);
                switch (paramsCounter) {
                    case 1:
                        fprintf(stderr, "ERROR! Function %s!\n", K.str);
                        compiler_exit(ERR_INCOMPATIBLE_TYPE);
                        break;
                    case 2:
                        break;
                    default:
                        fprintf(stderr, "ERROR! Function %s! Expected %d parameters but %d have been inserted!\n",
                                K.str, 2, paramsCounter);
                        compiler_exit(ERR_NO_OF_ARGS);
                        break;
                }
            }

            //BIF length
            if (strcmp(K.str, "length") == 0) {
                fprintf(stderr, "ERROR! Function %s!\n", K.str);
                compiler_exit(ERR_INCOMPATIBLE_TYPE);
            }

            //BIF substr
            if (strcmp(K.str, "substr") == 0) {
                //string a = createString(token);
                switch (paramsCounter) {
                    case 1:
                        fprintf(stderr, "ERROR! Function %s!\n", K.str);
                        compiler_exit(ERR_INCOMPATIBLE_TYPE);
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    default:
                        fprintf(stderr, "ERROR! Function %s! Expected %d parameters but %d have been inserted!\n",
                                K.str, 3, paramsCounter);
                        compiler_exit(ERR_NO_OF_ARGS);
                        break;
                }
            }
            break;
        case T_STRING://54
            //free(token.data) //TODO can be done here??
            //increment counter for NO. of parameters
            paramsCounter++;

            //BIF chr
            if (strcmp(K.str, "chr") == 0) {
                fprintf(stderr, "ERROR! Function %s!\n", K.str);
                compiler_exit(ERR_INCOMPATIBLE_TYPE);
            }

            //BIF ord
            if (strcmp(K.str, "ord") == 0) {
                switch (paramsCounter) {
                    case 1:
                        break;
                    case 2:
                        fprintf(stderr, "ERROR! Function %s!\n", K.str);
                        compiler_exit(ERR_INCOMPATIBLE_TYPE);
                        break;
                    default:
                        fprintf(stderr, "ERROR! Function %s! Expected %d parameters but %d have been inserted!\n",
                                K.str, 2, paramsCounter);
                        compiler_exit(ERR_NO_OF_ARGS);
                        break;
                }
            }

            //BIF substr
            if (strcmp(K.str, "substr") == 0) {
                switch (paramsCounter) {
                    case 1:
                        break;
                    case 2:
                        fprintf(stderr, "ERROR! Function %s!\n", K.str);
                        compiler_exit(ERR_INCOMPATIBLE_TYPE);
                        break;
                    case 3:
                        fprintf(stderr, "ERROR! Function %s!\n", K.str);
                        compiler_exit(ERR_INCOMPATIBLE_TYPE);
                        break;
                    default:
                        fprintf(stderr, "ERROR! Function %s! Expected %d parameters but %d have been inserted!\n",
                                K.str, 3, paramsCounter);
                        compiler_exit(ERR_NO_OF_ARGS);
                        break;
                }
            }
            break;
        case T_RBRACKET:
            return 1;
        default:
            compiler_exit(ERR_SYNTAX);
    }
    return 0;
}

//Parse for <arg-list2> LL
void parse_arg_list2() {
    token = getToken();
    if (token.type == T_COMMA) {//44
        parse_arg(399);
        parse_arg_list2();
    } else if (token.type != T_EOL) {//45
        compiler_exit(ERR_SYNTAX);
    }
}

//Parse for <arg-list2b> LL
void parse_arg_list2b() {
    token = getToken();
    if (token.type == T_COMMA) {//48
        parse_arg(399);
        parse_arg_list2b();
    } else if (token.type != T_RBRACKET) {//49
        compiler_exit(ERR_SYNTAX);
    }
}

//Just main, nothing special
int main() {
    gtsInit(&gts);
    ltsInit(&ltsMain);
    ltsInit(&ltsFunc);
    insertBIF(&gts);
    parse_main();
    return 0;
}
