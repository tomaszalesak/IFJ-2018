/*
**	IFJ
**	Syntax analysis
**	---------------------------
**	Robin Skaličan - xskali19
*/

#include "lexicalanalyzer.h"

void parse_main();

void parse_statement();

void parse_function();

void parse_st_list(int position_helper);

void parse_param();

void parse_param_list_1();

void parse_param_list_2();

void parse_arg();

void parse_arg_list(int token);

void parse_arg_list2();

void parse_arg_list_b();

void parse_arg_list2_b();

void parse_arg_list_switcher();

int precedence_table_fake(int fake);

Token token;
int i = 0;

//Parse for <main> LL
void parse_main() {
    token = getToken();

    switch (token.type) {
        case KW_DEF://3
            parse_function();
            break;
        case T_IDENTIFIER:// 11 12 16 17 27
            token = getToken();
            switch (token.type) {
                case OP_EQ://16 27
                    token = getToken();
                    if (token.type == T_IDENTIFIER) {//27 -> 28 29
                        parse_arg_list_switcher();//TODO Check if working
                    } else {//16
                        (precedence_table_fake(token.type)) ? exit(4) : 0;//TODO Legit?
                        //TODO EXPR - PRECEDENCE TABLE + GIVE TOKEN + CHECK EXIT CODE
                    }
                    break;
                case T_EOL:// 17
                    break;
                default://11 12
                    parse_arg_list_switcher();
                    break;
            }

            break;
        case T_EOL:// 6
            break;
        case KW_IF:// 19
            token = getToken();
            (precedence_table_fake(token.type)) ? exit(4) : 0;
            //TODO Will precedence give me/steal token? if y, change this
            token = getToken();
            if ((token.type == KW_THEN) && (getToken().type) == T_EOL) {
                parse_st_list(0);//TODO Check if working
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
        case T_EOF: // 7 TODO End of EOL, check SEM Analysis etc.
            exit(0);
        default :
            token = getToken();
            (precedence_table_fake(token.type)) ? exit(4) : 0;
            break;
    }//TODO Cases for functions (inputs,..._)
    parse_main();//Calling myself, cycle
}

void parse_function() {//3

    token = getToken();

    if ((token.type == T_IDENTIFIER) && (getToken().type) == T_LBRACKET) {
        parse_param_list_1();
        if ((getToken().type) == T_EOL) {
            i++;
            parse_st_list(2);//TODO Quick fixed, check the logic
        } else {
            exit(2);
        }
    } else {
        exit(2);
    }
}

void parse_st_list(int position_helper) {//TODO implement position_helper properly

    token = getToken();
    switch (token.type) {
        case T_IDENTIFIER:// 16 17 27
            token = getToken();
            switch (token.type) {
                case OP_ASS://16 27
                    token = getToken();
                    if (token.type == T_IDENTIFIER) {//27 -> 28 29 !TRAP! I do not know if its function or identifier

                        parse_arg_list_switcher();
                    } else {//16
                        (precedence_table_fake(token.type)) ? exit(4) : 0;
                        token = getToken();
                    }
                    if(token.type != T_EOL){
                        exit(2);
                    }
                    if(position_helper == 0){
                        parse_st_list(0);
                    }else{
                        parse_st_list(2);
                    }
                    break;
                case T_EOL:// 17
                    break;
                default:
                    exit(2);
            }
            break;
            /*case T_EOL:// 6 //TODO Legit???
                break;*/
        case KW_IF:// 19
            token = getToken();
            (precedence_table_fake(token.type)) ? exit(4) : 0;
            //TODO Will precedence give me/steal token? if y, change this
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
            if(position_helper != 0){
                exit(2);
            }else if(getToken().type != T_EOL){
                exit(2);
            }
            break;
        case KW_END:
            if(position_helper <= 0){
                exit(2);
            }else if(getToken().type != T_EOL){
                exit(2);
            }
            break;
        default:
            exit(2);
    }
    i--;
}

void parse_param_list_1() {//58
    parse_param();
    parse_param_list_2();
}

void parse_param_list_2() {
    token = getToken();
    if (token.type == T_COMMA) {//59
        parse_param();
        parse_param_list_2();
    } else if (token.type != T_RBRACKET) {
        exit(2);
    }//60
}

void parse_param() {//61
    token = getToken();
    if (token.type != T_IDENTIFIER) {
        exit(2);
    }
}

void parse_arg_list_switcher() {
    token = getToken();
    if (token.type == T_LBRACKET) {
        parse_arg_list_b();
        parse_arg_list2_b();
        if (getToken().type != T_EOL) {
            exit(2);//TODO .. Legit?
        }
    } else if(token.type != T_EOL) {
        parse_arg_list(token.type);
    }
}

void parse_arg(int token_type) {
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
        default://TODO For real like this? Check
            exit(2);
    }
}


void parse_arg_list(int token_type) {//43
    parse_arg(token_type);
    parse_arg_list2();
}

void parse_arg_list2() {
    token = getToken();
    if (token.type == T_COMMA) {//48
        parse_arg(399);
        parse_arg_list2();
    } else if (token.type != T_EOL) {//45
        exit(2);
    }
}

void parse_arg_list_b() {//47
    token = getToken();
    parse_arg(token.type);
    parse_arg_list2();
}

void parse_arg_list2_b() {
    token = getToken();
    if (token.type == T_COMMA) {//48
        parse_arg(399);
        parse_arg_list2_b();
    } else if (token.type != T_RBRACKET) {//49
        exit(2);
    }
}

int precedence_table_fake(int fake) {
    return 0;
}

int main() {
    parse_main();
    return 0;
}