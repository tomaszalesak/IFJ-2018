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
#include "sym_dl.h"

#define PH_MAIN_TOKEN (-1)        //MAIN already took TOKEN, dont take another one
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
Token returnValue;
int paramsCounter = 0;


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

        case T_EOF: // 7
            exit(SUCCESS);

            //Unique options for <main> checked, now goes into <stat> to check rest
        default :
            x = parse_st_list(PH_MAIN_TOKEN);
            break;
    }
    parse_main(x);//Calling myself, stopped by T_EOF or error EXIT
}

//Parse for <func> LL
void parse_function() {//3

    token = getToken();

    if ((token.type == T_IDENTIFIER || token.type == T_FUNCTION) && (getToken().type) == T_LBRACKET) {
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

        gen_label(token);
        gen_pushframe();
        gen_retval();

        ///semantic - creating new list for function definition semantic analysis
        //pointer to save main stack
        tltsDLList *tmpStack;
        tmpStack = ltsStack;
        //create stack for function definition
        tltsDLList defStack;
        ltsDLInitList(&defStack);
        ltsStack = &defStack;
        ///semantic - inserting temporary LTS
        LTSNodePtr tempTable;
        ltsInit(&tempTable);
        ltsDLInsertFirst(ltsStack, ltsMain);
        ltsDLFirst(ltsStack);

        parse_param_list_1();
        gtsSetParamCount(gts, &K, paramsCounter);
        paramsCounter = 0;
        if ((getToken().type) == T_EOL) {
            //Call function for <st-list>
            //printf("LOL1\n");
            parse_st_list(PH_END);
            //printf("LOL2\n");
            ///semantic remove temporary LTS
            //ltsDLPred(ltsStack);
            //ltsDLPostDelete(ltsStack);
            gen_popframe();
            gen_return();
            ltsStack = tmpStack;
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
        case T_FUNCTION:
        case T_IDENTIFIER:// 16 17 27

            /*//semantic
            string id;
            strInit(&id);
            id = createString(token);
            if (gtsSearch(gts, &id) != NULL) {
                fprintf(stderr, "Semantic Error! Can't redefine function %s as variable!\n", id.str);
                strFree(&id);
                compiler_exit(ERR_UNDEF_REDEF);
            }
            strFree(&id);
             */

            token = getToken();

            switch (token.type) {

                case OP_ASS://16 27
                    if(token_old.type == T_FUNCTION){
                    compiler_exit(ERR_SYNTAX);
                    }
                    //semantic
                    K = createString(token_old);
                    ///
                    if (gtsSearch(gts, &K) != NULL) {
                        //todo check error code
                        fprintf(stderr, "Semantic Error! Can't redefine function %s as variable!\n", K.str);
                        compiler_exit(ERR_UNDEF_REDEF);
                    }
                    if (ltsInsert(&ltsStack->Act->lts, &K) == SUCCESS)
                        gen_defvar(token_top);
                    //gtsSearch(gts, &K);
                    token = getToken();
                    token_old = token;
                    switch (token.type) {
                        case T_FUNCTION:
                        case T_IDENTIFIER: //27 -> 28 29 !TRAP! I do not know if its function or identifier
                            //semantic - get identifier name/value
                            K = createString(token);
                            if (gtsSearch(gts, &K) == NULL) {
                                //fprintf(stderr, "\n*** SYNTAX ERROR ***\nCannot use function %s in expression!\n", K.str);
                                //compiler_exit(ERR_SYNTAX);
                                ltsDLSearchPre(ltsStack, K);
                            }
                            //ltsDLSearchPre(ltsStack, K);
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
                                    gen_resultAss(token_top);
                                    returnValue = token_top;
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
                                    break;
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

                            //semantic for BIF
                        case BIF_PRINT://35 TODO PRINT!!!
                            K.str = "print";
                            token = getToken();
                            parse_arg_list_switcher(1);
                            parse_st_list(actual_position_helper);
                            break;

                        case BIF_LENGTH://37 //TODO Check STRING
                            //semantic set variable type
                            ltsSetIdType(ltsStack->Act->lts, &K, T_INT);
                            K.str = "length";
                            token = getToken();
                            parse_arg_list_switcher(0);
                            parse_st_list(actual_position_helper);
                            break;

                        case BIF_SUBSTR://38 //TODO Check STRING,INT,INT
                            //semantic set variable type
                            ltsSetIdType(ltsStack->Act->lts, &K, T_STRING);
                            K.str = "substr";
                            token = getToken();
                            parse_arg_list_switcher(0);
                            parse_st_list(actual_position_helper);
                            break;

                        case BIF_ORD://39 //TODO Check STRING,INT
                            //semantic set variable type
                            ltsSetIdType(ltsStack->Act->lts, &K, T_INT);
                            K.str = "ord";
                            token = getToken();
                            parse_arg_list_switcher(0);
                            parse_st_list(actual_position_helper);
                            break;

                        case BIF_CHR://40  //TODO Check INT
                            //semantic set variable type
                            ltsSetIdType(ltsStack->Act->lts, &K, T_STRING);
                            K.str = "chr";
                            token = getToken();
                            parse_arg_list_switcher(0);
                            parse_st_list(actual_position_helper);
                            break;

                        default: //16
                            ltsSetIdType(ltsStack->Act->lts, &K, token.type);
                            token_old = token;
                            token = getToken();
                            token = prec_anal(token_old, token, 1);
                            gen_resultAss(token_top);
                            returnValue = token_top;
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
                    break;*/

                case T_EOL:// 17
                    //token = getToken();
                    ///call without = or (
                    //semantic
                {
                    string k;
                    k = createString(token_old);
                    //check if it is function with zero params
                    if (gtsSearch(gts, &k) != NULL) {
                        if (gtsCheckIfDefined(gts, &k) == 1) {
                            if (gtsGetParamCount(gts, &k) != 0) {
                                semanticError(ERR_NO_OF_ARGS, k, paramsCounter, SYM_NONE);
                            }
                        } //todo add error and exit?
                    } else {
                        ltsDLSearchPre(ltsStack, k);
                    }


                    parse_st_list(actual_position_helper);
                    break;
                }

                case OP_ADD:
                case OP_SUB:
                case OP_MUL:
                case OP_DIV:
                    token = prec_anal(token_old, token, 1);
                    parse_st_list(actual_position_helper);
                    break;

                default://11 12
                    K = createString(token_old);
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
        {
            LTSNodePtr tempTable;
            ltsInit(&tempTable);
            ltsDLPostInsert(ltsStack, tempTable);
            ltsDLSucc(ltsStack);
            token = prec_anal(token, token, 0);
            //int x = i;
            gen_if_cmpResult();
            int elseID = gen_uniqueID_last();
            if ((token.type == KW_THEN) && (getToken().type) == T_EOL) {
                parse_st_list(PH_ELSE);

                ///semantic - release temporary LTS
                ltsDLPred(ltsStack);
                ltsDLPostDelete(ltsStack);
                ///semantic - inserting temporary LTS
                //LTSNodePtr tempTable;
                ltsInit(&tempTable);
                ltsDLPostInsert(ltsStack, tempTable);
                ltsDLSucc(ltsStack);

                gen_if_elseLabel(elseID);
                elseID = gen_uniqueID_last();
                parse_st_list(PH_END);
                gen_if_endLabel(elseID);
                //if (actual_position_helper != PH_MAIN) {
                parse_st_list(actual_position_helper);
                // }else {
                //    return 1;
                // }
            } else {
                compiler_exit(ERR_SYNTAX);
            }
            break;
        }

        case KW_WHILE:// 24
        {
            LTSNodePtr tempTable;
            ltsInit(&tempTable);
            ltsDLPostInsert(ltsStack, tempTable);
            ltsDLSucc(ltsStack);
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
        }
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
            ///semantic - release temporary LTS
            ltsDLPred(ltsStack);
            ltsDLPostDelete(ltsStack);
            //ltsDL
            break;

            //BIF Handling
        case BIF_INPUTS://32
        case BIF_INPUTI://33
        case BIF_INPUTF://34
            if (((token = getToken()).type) == T_LBRACKET) {
                if (((getToken().type) == T_RBRACKET) &&
                    ((((token = getToken()).type) == T_EOL) || ((token.type) == T_EOF))) {
                    parse_st_list(actual_position_helper);
                    break;
                } else {
                    compiler_exit(ERR_NO_OF_ARGS);
                }
            } else if (token.type == T_EOL) {
                parse_st_list(actual_position_helper);
                break;
            } else {// TODO SEMANTIC TODO, THIS DOES NOT COUNT WITH BAD NUMBER OF ARGS
                compiler_exit(ERR_SYNTAX);
            }

            //semantic for BIF
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
    //semantic check for function!=parameter
    K = createString(token);
    if (gtsSearch(gts, &K) != NULL) {
        fprintf(stderr, "Semantic Error! Cannot redefine function %s as variable!\n", K.str);
        compiler_exit(ERR_UNDEF_REDEF);
    }
    gen_parameter(token, GEN_COUNTER_ADD);
    ///semantic
    ltsInsert(&(ltsStack->Act->lts), &K);
    ltsSetIdType((ltsStack->Act->lts), &K, SYM_FUNC_PARAM); //todo replace with T_NIL
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
            compiler_exit(ERR_SYNTAX);
        }

    } else if (token.type != T_EOL) {
        gen_TF();
        //Solves <arg-list> LL
        parse_arg(token.type);//43
        gen_argument(token, 1);
        //Call function for <arg-list2>
        parse_arg_list2();//48,49

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

    if (token_type == PH_TAKE) {
        token = getToken();
    }

    switch (token.type) {
        case T_IDENTIFIER://51
            paramsCounter++;
            string id;
            strInit(&id);
            id = createString(token);
            ltsDLSearchPre(ltsStack, id);
            /*if (ltsGetIdType(ltsStack->Act->lts, &id) == -1) {
                fprintf(stderr, "Semantic Error! Variable %s is undeclared!\n", id.str);
                strFree(&id);
                compiler_exit(ERR_UNDEF_REDEF);
            }
             */
            strFree(&id);

            //semantic BIF params check TODO put it onto function maybe?!
            //BIF chr
            if (strcmp(K.str, "chr") == 0) {
                string a = createString(token);
                //chr accepts 1 integer parameter
                if (paramsCounter != 1) {
                    fprintf(stderr, "ERROR! Function %s!\n", K.str);
                    compiler_exit(ERR_NO_OF_ARGS);
                }
                if ((ltsGetIdType(ltsStack->Act->lts, &a) != T_INT) && (ltsGetIdType(ltsStack->Act->lts, &a) != T_FLOAT)) {
                    fprintf(stderr, "Semantic Error! Bad parameter type for function %s!\n", K.str);
                    compiler_exit(ERR_INCOMPATIBLE_TYPE);
                }
            }

            //BIF ord
            if (strcmp(K.str, "ord") == 0) {
                string a = createString(token);
                switch (paramsCounter) {
                    case 1:
                        if (ltsGetIdType(ltsStack->Act->lts, &a) != T_STRING) {
                            fprintf(stderr, "ERROR! Function %s!\n", K.str);
                            compiler_exit(ERR_INCOMPATIBLE_TYPE);
                        }
                        break;
                    case 2:
                        if ((ltsGetIdType(ltsStack->Act->lts, &a) != T_INT) && (ltsGetIdType(ltsStack->Act->lts, &a) != T_FLOAT)) {
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
                if (ltsGetIdType(ltsStack->Act->lts, &a) != T_STRING) {
                    fprintf(stderr, "ERROR! Function %s!\n", K.str);
                    compiler_exit(ERR_INCOMPATIBLE_TYPE);
                }
            }

            //BIF substr
            if (strcmp(K.str, "substr") == 0) {
                string a = createString(token);
                switch (paramsCounter) {
                    case 1:
                        if (ltsGetIdType(ltsStack->Act->lts, &a) != T_STRING) {
                            fprintf(stderr, "ERROR! Function %s!\n", K.str);
                            compiler_exit(ERR_INCOMPATIBLE_TYPE);
                        }
                        break;
                    case 2:
                        if (ltsGetIdType(ltsStack->Act->lts, &a) != T_INT && ltsGetIdType(ltsStack->Act->lts, &a) != T_FLOAT) {
                            fprintf(stderr, "ERROR! Function %s!\n", K.str);
                            compiler_exit(ERR_INCOMPATIBLE_TYPE);
                        }
                        break;
                    case 3:
                        if (ltsGetIdType(ltsStack->Act->lts, &a) != T_INT && ltsGetIdType(ltsStack->Act->lts, &a) != T_FLOAT) {
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
    insertBIF(&gts);
    // init stack
    tDLList func_stack;
    DLInitList(&func_stack);
    //init LTS stack
    tltsDLList mainStack;
    ltsDLInitList(&mainStack);

    ltsStack = &mainStack;
    ltsDLInsertFirst(ltsStack, ltsMain);
    ltsDLFirst(ltsStack);

    // generate IFJcode2018 file header
    gen_code_header();

    // start code analysis
    parse_main(0);
    return 0;
}
