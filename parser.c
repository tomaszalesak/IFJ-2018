/*
**	IFJ
**	Syntax analysis
**	---------------------------
**	Robin Skaličan - xskali19
*/

#include "lexicalanalyzer.h"
#include "symtable.h"
#include "prec_anal.h"
#include "errors.h"
#include "generator.h"

#define PH_MAIN_TOKEN -1        //MAIN already took TOKEN, dont take another one
#define PH_ELSE 0               //ELSE is valid now
#define PH_END 2                //END is valid now
#define PH_MAIN 8               //Get back to main
#define PH_TAKE 399             //Take TOKEN

void parse_main(int x);

void parse_function();

int parse_st_list(int actual_position_helper);

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
int i=0;

//Parse for <main> LL
void parse_main(int x) {
    if (!x) {
        token = getToken();
        x = 0;
    }
    switch (token.type) {

        case KW_DEF://3
            parse_function();
            break;

        case T_EOL:
            break;

        case T_EOF: // 7 TODO End of INPUT, check SEM Analysis etc.
            exit(SUCCESS);

            //Unique options for <main> checked, now goes into <stat> to check rest
        default :
            x=parse_st_list(PH_MAIN_TOKEN);
            break;
    }
    parse_main(x);//Calling myself, stopped by T_EOF or error EXIT
}

//Parse for <func> LL
void parse_function() {//3// TODO Define function with no brackets?

    token = getToken();

    if ((token.type == T_IDENTIFIER) && (getToken().type) == T_LBRACKET) {
        //Call function for <param-l>

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

        gen_label(token);
        gen_pushframe();
        gen_retval();

        parse_param_list_1();
        gtsSetParamCount(gts, &K, paramsCounter);
        paramsCounter = 0;
        if ((getToken().type) == T_EOL) {
            //Call function for <st-list>
            //printf("LOL1\n");
            parse_st_list(PH_END);//TODO Check this
            //printf("LOL2\n");
            gen_popframe();
            gen_return();
        } else {
            compiler_exit(ERR_SYNTAX);
        }
    } else {
        compiler_exit(ERR_SYNTAX);
    }
}

//Parse for <st-list> LL
//actual_position_helper used for check if get new token and go back to main(4) and if its in if(0) or in else(2) part
int parse_st_list(int actual_position_helper) {

     if (actual_position_helper == PH_MAIN) {
         return 0;
     }
    if (actual_position_helper != PH_MAIN_TOKEN) {
        token = getToken();
    } else {
        actual_position_helper = PH_MAIN;
    }
    Token token_old = token;
    Token token_top = token;
    switch (token.type) {

        case T_IDENTIFIER:// 16 17 27

            K = createString(token);
            if (ltsInsert(&ltsMain, &K) == SUCCESS)
                gen_defvar(token_top);
            //gtsSearch(gts, &K);
            token = getToken();

            switch (token.type) {

                case OP_ASS://16 27
                    token = getToken();
                    token_old = token;
                    switch (token.type) {

                        case T_IDENTIFIER: //27 -> 28 29 !TRAP! I do not know if its function or identifier
                            K = createString(token);
                            token = getToken();
                            switch (token.type) {

                                case T_LBRACKET:
                                case T_IDENTIFIER:
                                case T_EOL:
                                    parse_arg_list_switcher(0);
                                    gen_call(token_old);
                                    gen_getretval(token_top);
                                    parse_st_list(actual_position_helper);
                                    break;

                                default:
                                    token = prec_anal(token_old, token, 1);
                                    parse_st_list(actual_position_helper);
                                    break;
                            }
                            break;

                            //BIF Handling
                        case BIF_INPUTS://32
                        case BIF_INPUTI://33
                        case BIF_INPUTF://34
                            if (((token = getToken()).type) == T_LBRACKET) {
                                if (((getToken().type) == T_RBRACKET) &&
                                    ((((token = getToken()).type) == T_EOL) || ((token.type) == T_EOF))) {
                                    parse_st_list(actual_position_helper);
                                    break;//TODO Check this
                                } else {
                                    compiler_exit(ERR_NO_OF_ARGS);
                                }
                            } else if (token.type == T_EOL) {
                                parse_st_list(actual_position_helper);
                                break;
                            } else {// TODO SEMANTIC TODO, THIS DOES NOT COUNT WITH BAD NUMBER OF ARGS
                                compiler_exit(ERR_SYNTAX);
                            }
                            break;

                        case BIF_PRINT://35 TODO PRINT!!!
                            K.str = "print";
                            token = getToken();
                            parse_arg_list_switcher(1);
                            parse_st_list(actual_position_helper);
                            break;

                        case BIF_LENGTH://37 //TODO Check STRING
                            K.str = "length";
                            token = getToken();
                            parse_arg_list_switcher(0);
                            parse_st_list(actual_position_helper);
                            break;

                        case BIF_SUBSTR://38 //TODO Check STRING,INT,INT
                            K.str = "substr";
                            token = getToken();
                            parse_arg_list_switcher(0);
                            parse_st_list(actual_position_helper);
                            break;

                        case BIF_ORD://39 //TODO Check STRING,INT
                            K.str = "ord";
                            token = getToken();
                            parse_arg_list_switcher(0);
                            parse_st_list(actual_position_helper);
                            break;

                        case BIF_CHR://40  //TODO Check INT
                            K.str = "chr";
                            token = getToken();
                            parse_arg_list_switcher(0);
                            parse_st_list(actual_position_helper);
                            break;

                        default: //16
                            token_old = token;
                            token = getToken();
                            token = prec_anal(token_old, token, 1);
                            parse_st_list(actual_position_helper);
                            break;
                    }
                    break;

                    /* if (token.type != T_EOL) {//Solved by precedence right?
                         compiler_exit(ERR_SYNTAX);
                     }*/
                    //If actual_position_helper is 4, which means its call from main, it goes back
                    /*if (actual_position_helper == 0) {
                        parse_st_list(actual_position_helper,old_position_helper);
                    } else if (actual_position_helper == 2) {
                        parse_st_list(actual_position_helper,old_position_helper);
                    }
                    break;*///TODO CHANGED THIS

                case T_EOL:// 17
                    //token = getToken();
                    parse_st_list(actual_position_helper);
                    break;

                case OP_ADD:
                case OP_SUB:
                case OP_MUL:
                case OP_DIV:
                    token = prec_anal(token_old, token, 1);
                    parse_st_list(actual_position_helper);
                    break;

                default://11 12
                    parse_arg_list_switcher(0);
                    gen_call(token_old);
                    gen_getretval(token_top);
                    parse_st_list(actual_position_helper);
                    break;
            }
            break;

        case T_EOL:// 6
            parse_st_list(actual_position_helper);
            break;

        case KW_IF:// 19
            token = prec_anal(token, token, 0);
            i++;
            //int x = i;
            gen_if_cmpResult();
            int elseID = gen_uniqueID_last();
            if ((token.type == KW_THEN) && (getToken().type) == T_EOL) {
                parse_st_list(PH_ELSE);
                gen_if_elseLabel(elseID);
                elseID = gen_uniqueID_last();
                parse_st_list(PH_END);
                gen_if_endLabel(elseID);
                //if (actual_position_helper != PH_MAIN) {
                parse_st_list(actual_position_helper);//TODO FIX IT HERE
               // }else {
                //    return 1;
               // }
            } else {
                compiler_exit(ERR_SYNTAX);
            }
            break;

        case KW_WHILE:// 24
            gen_while_doLabel();
            int doID = gen_uniqueID_last();
            token = prec_anal(token, token, 0);
            gen_while_cmpResult();
            int endID = gen_uniqueID_last();
            if ((token.type == KW_DO) && (getToken().type) == T_EOL) {
                parse_st_list(PH_END);
                gen_while_endLabel(endID, doID);
                parse_st_list(actual_position_helper);
            } else {
                compiler_exit(ERR_SYNTAX);
            }
            break;

        case KW_ELSE:
            if (actual_position_helper != 0) {
                compiler_exit(ERR_SYNTAX);
            } else if (getToken().type != T_EOL) {
                compiler_exit(ERR_SYNTAX);
            }
            //parse_st_list(PH_END);
            break;

        case KW_END:
            if (actual_position_helper != 2) {
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
                    parse_st_list(actual_position_helper);
                    break;//TODO Check this
                } else {
                    compiler_exit(ERR_NO_OF_ARGS);
                }
            } else if (token.type == T_EOL) {
                parse_st_list(actual_position_helper);
                break;
            } else {// TODO SEMANTIC TODO, THIS DOES NOT COUNT WITH BAD NUMBER OF ARGS
                compiler_exit(ERR_SYNTAX);
            }

        case BIF_PRINT://35 TODO PRINT!!!
            K.str = "print";
            token = getToken();
            parse_arg_list_switcher(1);
            parse_st_list(actual_position_helper);
            break;

        case BIF_LENGTH://37 //TODO Check STRING
            K.str = "length";
            token = getToken();
            parse_arg_list_switcher(0);
            parse_st_list(actual_position_helper);
            break;

        case BIF_SUBSTR://38 //TODO Check STRING,INT,INT
            K.str = "substr";
            token = getToken();
            parse_arg_list_switcher(0);
            parse_st_list(actual_position_helper);
            break;

        case BIF_ORD://39 //TODO Check STRING,INT
            K.str = "ord";
            token = getToken();
            parse_arg_list_switcher(0);
            parse_st_list(actual_position_helper);
            break;

        case BIF_CHR://40  //TODO Check INT
            K.str = "chr";
            token = getToken();
            parse_arg_list_switcher(0);
            parse_st_list(actual_position_helper);
            break;

        default:
            token = getToken();
            token = prec_anal(token_old, token, 1);
            parse_st_list(actual_position_helper);
            break;
    }
    //printf("Actually ending\n");
    return 0;
    /* if(actual_position_helper != PH_MAIN) {
         parse_st_list(actual_position_helper);
     }*/
}

//Parse for <param-l> LL
void parse_param_list_1() {//58
    //Check for if ( ) validation
    gen_parameter(token, GEN_COUNTER_RESET);
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
    gen_parameter(token, GEN_COUNTER_ADD);
    paramsCounter++;
    return 0;
}

//Function for choosing, if send it to <arg-list2> or <arg-list2b>
//int print_checker used for check if called from print, then it cant be without arguments
void parse_arg_list_switcher(int print_checker) {

    if (token.type == T_LBRACKET) {
        gen_TF();
        //Solves <arg-listb> LL
        token = getToken();
        if (!parse_arg(token.type)) {//47
            gen_argument(token, 1);

            //Call function for <arg-list2b>
            parse_arg_list2b();//44,45
        }

        //added for semantic analysis
        //check if the function call has correct number of parameters
        if (gtsGetParamCount(gts, &K) != paramsCounter) {
            fprintf(stderr,
                    "ERROR! Bad number of arguments for function %s!\nExpected %d parameters but %d have been inserted.\n",
                    K.str, gtsGetParamCount(gts, &K), paramsCounter);
            exit(ERR_NO_OF_ARGS);
        }
        //reset params counter for another func. check
        paramsCounter = 0;

        if (getToken().type != T_EOL) {
            compiler_exit(ERR_SYNTAX);//TODO Check this
        }

    } else if (token.type != T_EOL) {
        gen_TF();
        //Solves <arg-list> LL
        parse_arg(token.type);//43
        gen_argument(token, 1);
        //Call function for <arg-list2>
        parse_arg_list2();//48,49
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
    if (token_type == PH_TAKE) {
        token = getToken();
    }

    switch (token.type) {
        case T_IDENTIFIER://51
        case T_INT://52
        case T_FLOAT://53
        case T_STRING://54
            //free(token.data) //TODO can be done here??
            //increment counter for NO. of parameters
            paramsCounter++;
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
        parse_arg(PH_TAKE);
        gen_argument(token, GEN_COUNTER_ADD);
        parse_arg_list2();
    } else if (token.type != T_EOL) {//45
        compiler_exit(ERR_SYNTAX);
    }
}

//Parse for <arg-list2b> LL
void parse_arg_list2b() {
    token = getToken();
    if (token.type == T_COMMA) {//48
        parse_arg(PH_TAKE);
        gen_argument(token, GEN_COUNTER_ADD);
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
    // init stack
    tDLList func_stack;
    DLInitList(&func_stack);
    parse_main(0);
    return 0;
}
