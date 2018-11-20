/*
**	IFJ
**	Syntax analysis
**	---------------------------
**	Robin Skaličan - xskali19
*/

#include "lexicalanalyzer.h"

void parse_main();

void parse_function();

void parse_st_list(int position_helper);

void parse_param();

void parse_param_list_1();

void parse_param_list_2();

void parse_arg(int token_type);

void parse_arg_list2();

void parse_arg_list2b();

void parse_arg_list_switcher();

int precedence_table_fake(int fake);

Token token;


//TODO Changes old pointer value to actual token and gets new token for actual token, use instead of global and for prec table calls
void parse_old_token(){

}

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
void parse_function() {//3

    token = getToken();

    if ((token.type == T_IDENTIFIER) && (getToken().type) == T_LBRACKET) {
        //Call function for <param-l>
        parse_param_list_1();
        if ((getToken().type) == T_EOL) {
            //Call function for <st-list>
            parse_st_list(2);//TODO Quick fixed, check the logic
        } else {
            exit(2);
        }
    } else {
        exit(2);
    }
}

//Parse for <st-list> LL
//position_helper used for check if get new token and go back to main(4) and if its in if(0) or in else(2) part
void parse_st_list(int position_helper) {

    if (position_helper != 4) {//TODO Change to bool check or?
        token = getToken();
    }

    switch (token.type) {

        case T_IDENTIFIER:// 16 17 27
            token = getToken();

            switch (token.type) {

                case OP_ASS://16 27
                    token = getToken();

                    if (token.type == T_IDENTIFIER) {//27 -> 28 29 !TRAP! I do not know if its function or identifier
                        token = getToken();
                        parse_arg_list_switcher();
                    } else {//16
                        (precedence_table_fake(token.type)) ? exit(4) : 0;
                        token = getToken();
                    }
                    /* if (token.type != T_EOL) {//Solved by precedence right?
                         exit(2);
                     }*/
                    //If position_helper is 4, which means its call from main, it goes back
                    if (position_helper == 0) {
                        parse_st_list(0);
                    } else if (position_helper == 2){
                        parse_st_list(2);
                    }
                    break;

                case T_EOL:// 17
                    break;

                default://11 12
                    parse_arg_list_switcher();
                    break;
            }
            break;

        case T_EOL:// 6 TODO Check this if legit both main and stat returning
            parse_st_list(position_helper);
            break;

        case KW_IF:// 19
            token = getToken();
            (precedence_table_fake(token.type)) ? exit(4) : 0;
            token = getToken();
            if ((token.type == KW_THEN) && (getToken().type) == T_EOL) {
                parse_st_list(0);
                parse_st_list(2);
            } else {
                exit(2);
            }
            break;

        case KW_WHILE:// 24
            token = getToken();
            (precedence_table_fake(token.type)) ? exit(4) : 0;
            token = getToken();
            if ((token.type == KW_DO) && (getToken().type) == T_EOL) {
                parse_st_list(2);
                if ((getToken().type) != T_EOL) {
                    exit(2);
                }
            } else {
                exit(2);
            }
            break;

        case KW_ELSE:
            if (position_helper != 0) {
                exit(2);
            } else if (getToken().type != T_EOL) {
                exit(2);
            }
            break;

        case KW_END:
            if (position_helper <= 0) {
                exit(2);
            } else if (getToken().type != T_EOL) {
                exit(2);
            }
            break;

            //BIF Handling
        case BIF_INPUTS://32
        case BIF_INPUTI://33
        case BIF_INPUTF://34
        case BIF_SUBSTR://38
        case BIF_ORD://39
        case BIF_CHR://40
            break;//TODO Think this up

        case BIF_LENGTH://37
            parse_arg_list_switcher();//TODO Test if legit
            break;

        case BIF_PRINT://35
            break;//TODO Think this up

        default:
            exit(2);
    }
}

//Parse for <param-l> LL
void parse_param_list_1() {//58
    parse_param();
    parse_param_list_2();
}

//Parse for <param-l2> LL
void parse_param_list_2() {
    token = getToken();
    if (token.type == T_COMMA) {//59
        parse_param();
        parse_param_list_2();
    } else if (token.type != T_RBRACKET) {
        exit(2);
    }//60
}

//Parse for <param> LL
void parse_param() {//61
    token = getToken();
    if (token.type != T_IDENTIFIER) {
        exit(2);
    }
}

//Function for choosing, if send it to <arg-list2> or <arg-list2b>
void parse_arg_list_switcher() {
    //token=getToken();// 19.11 ?
    if (token.type == T_LBRACKET) {
        //Solves <arg-listb> LL
        token = getToken();
        parse_arg(token.type);//47
        //Call function for <arg-list2b>
        parse_arg_list2b();//44,45

        if (getToken().type != T_EOL) {
            exit(2);//TODO .. Legit?
        }

    } else if (token.type != T_EOL) {
        //Solves <arg-list> LL
        parse_arg(token.type);//43
        //Call function for <arg-list2>
        parse_arg_list2();//48,49
    }
}

//Parse for <arg> LL
void parse_arg(int token_type) {
    //TODO Make this one better / Checks if token is taken or should take
    if (token_type == 399) {
        token = getToken();
    }

    switch (token.type) {
        case T_IDENTIFIER://51
        case T_INT://52
        case T_FLOAT://53
        case T_STRING://54
            //free(token.data);//TODO Check if free is legit
            break;
        default:
            exit(2);
    }
}

//Parse for <arg-list2> LL
void parse_arg_list2() {
    token = getToken();
    if (token.type == T_COMMA) {//44
        parse_arg(399);
        parse_arg_list2();
    } else if (token.type != T_EOL) {//45
        exit(2);
    }
}

//Parse for <arg-list2b> LL
void parse_arg_list2b() {
    token = getToken();
    if (token.type == T_COMMA) {//48
        parse_arg(399);
        parse_arg_list2b();
    } else if (token.type != T_RBRACKET) {//49
        exit(2);
    }
}

//Just for testing purposes, will be replaced by real precedence table
int precedence_table_fake(int fake) {
    return 0;
}

//Just main, nothing special
int main() {
    parse_main();
    return 0;
}
