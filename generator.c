/*
**	IFJ
**	Code generation
**	---------------------------
**	Vojtěch Novotný - xnovot1f
*/

#include "generator.h"

int UID = INT_MIN;

/*
 * Generates a unique INT number. The numbers are not random, but they come in a sequence starting at INT_MIN.
 * @return A unique INT number.
 */
int gen_uniqueID_next() {
    UID++;
    return UID;
}

/*
 * Gets the last uniqueUI that was generated.
 * @return The INT number.
 */
int gen_uniqueID_last() {
    return UID;
}

/*
 * Generates code for 1 function argument.
 * @param token          - Contains the token that is used as the argument.
 * @param argumentNumber - Contains the position of the argument.
 * @param                   - Send positive number to set the last position to a positive number.
 * @param                   - Send GEN_COUNTER_ADD to increment the last used number and use it as position number.
 */
void gen_argument(Token token, int argumentNumber) {

    static int argumentCounter = 0;

    if (argumentNumber == GEN_COUNTER_RESET) {
        argumentCounter = 0;
        return;
    }
    else if (argumentNumber == GEN_COUNTER_ADD)
        argumentCounter++;
    else
        argumentCounter = argumentNumber;


    printf("DEFVAR TF@%%%d\nMOVE TF@%%%d ", argumentCounter, argumentCounter);

    switch (token.type) {
        case T_IDENTIFIER:
            printf("neaka hodnota z TS\n");
            break;
        case T_INT:
            printf("int@%d\n", *(int*)(token.data));
            free(token.data);
            break;
        case T_FLOAT:
            printf("float@%f\n", *(float*)(token.data));
            free(token.data);
            break;
        case T_STRING:
            printf("string@%s\n", (char*)(token.data));
            free(token.data);
            break;
        default:
            printf("WHEEE\n");
            break;
    }
}

/*
 * Generates code to create a new temporary frame.
 */
void gen_TF() {
    printf("CREATE FRAME\n");
}

/*
 * Generates code to define a new label.
 * @param token - token.data character string is used as the name for the label
 */
void gen_label(Token token) {
    printf("LABEL $%s\n", (char*)token.data);
}

/*
 * Generates code for 1 function parameter.
 * @param token - Contains the token that is used as the parameter.
 * @param parameterNumber - Contains the position of the parameter.
 * @param                    - Send positive number to set the last position to a positive number.
 * @param                    - Send GEN_COUNTER_ADD to increment the last used number and use it as position number.
 */
void gen_parameter(Token token, int parameterNumber) {

    static int parameterCounter = 0;

    if (parameterNumber == GEN_COUNTER_RESET) {
        parameterCounter = 0;
        return;
    }
    if (parameterNumber == GEN_COUNTER_ADD)
        parameterCounter++;
    else
        parameterCounter = parameterNumber;

    printf("DEFVAR LF@%s\nMOVE LF@%s LF@%%%d\n", (char*)(token.data), (char*)(token.data), parameterCounter);
}

/*
 * Generates code that calls function.
 * @param token - Represents the function.
 */
void gen_call(Token token) {
    printf("CALL $%s\n", (char*)(token.data));
}

/*
 * Generates code for return value variable definition.
 */
void gen_retval() {
    printf("DEFVAR LF@%%retval\nMOVE LF@%%retval nil@nil\n");
}

/*
 * Generates code for return statement.
 */
void gen_return() {
    printf("RETURN\n");
}

/*
 * Generates code that moves return value to the left-side variable of assignment.
 * @param token - Token representing the left-side variable.
 */
void gen_getretval(Token token) {
    printf("MOVE LF@%s TF@%%retval\n", (char*)(token.data));
}

/*
 * Generates code for pushing temporary frame onto the frame stack.
 */
void gen_pushframe() {
    printf("PUSHFRAME\n");
}

/*
 * Generates code for poping top of the frame stack onto the temporary frame.
 */
void gen_popframe() {
    printf("POPFRAME\n");
}

/*
 * Generates code that defines a new variable.
 * @param token - Token representing the variable that is to be defined.
 */
void gen_defvar(Token token) {
    printf("DEFVAR LF@%s\n", (char*)(token.data));
}

/*
 * Generates control code based on the result of IF comparison.
 */
void gen_if_cmpResult() {
    int resultID = gen_uniqueID_last();
    int thenID = gen_uniqueID_next();
    int elseID = gen_uniqueID_next();
    printf("JUMPIFEQ IF%%then%%%x LF@IF%%%x bool@true\n", thenID, resultID);
    printf("JUMP IF%%else%%%x\n", elseID);
    printf("LABEL IF%%then%%%x\n", thenID);
}

/*
 * Generates else control label.
 * @param elseID - id used for the else label when generating if control code.
 */
void gen_if_elseLabel(int elseID) {
    int endID = gen_uniqueID_next();
    printf("JUMP IF%%end%%%x\n", endID);
    printf("LABEL IF%%else%%%x\n", elseID);
}

/*
 * Generates end control label.
 * @param endID - id used for the end label when generating else control label.
 */
void gen_if_endLabel(int endID) {
    printf("LABEL IF%%end%%%x\n", endID);
}

/*
 * Generates label to check while cycle condition.
 */
void gen_while_doLabel() {
    int doID = gen_uniqueID_next();
    printf("LABEL WHILE%%do%%%x\n", doID);
}

/*
 * Generates control code based on the result of WHILE comparison.
 */
void gen_while_cmpResult() {
    int resultID = gen_uniqueID_last();
    int endID = gen_uniqueID_next();
    printf("JUMPIFEQ WHILE%%end%%%x LF@WHILE%%%x bool@false\n", endID, resultID);
}

/*
 * Generates end control label.
 * @param endID - id used for the end label when generating while control code.
 * @param doID  - id of the do label made when generating while control code.
 */
void gen_while_endLabel(int endID, int doID) {
    printf("JUMP WHILE%%do%%%x\n", doID);
    printf("LABEL WHILE%%end%%%x\n", endID);
}



// Built-in Function Code Generation
// =================================

/*
 * Generates code for built-in function INPUTS().
 */
void gen_bif_inputs(Token token) {
    printf("READ LF@%s string\n", (char*)(token.data));
}

/*
 * Generates code for built-in function INPUTI().
 */
void gen_bif_inputi(Token token){
    printf("READ LF@%s int\n", (char*)(token.data));
}

/*
 * Generates code for built-in function INPUTF().
 */
void gen_bif_inputf(Token token){
    printf("READ LF@%s float\n", (char*)(token.data));
}

/*
 * Generates code for built-in function PRINT().
 */
void gen_bif_print(){
    printf("WRITE lol");
}

/*
 * Generates code for built-in function LENGTH().
 */
void gen_bif_length(){
    printf("");
}

/*
 * Generates code for built-in function SUBSTR().
 */
void gen_bif_substr(){
    printf("");
}

/*
 * Generates code for built-in function ORD().
 */
void gen_bif_ord(){
    printf("");
}

/*
 * Generates code for built-in function CHR().
 */
void gen_bif_chr(){
    printf("");
}
