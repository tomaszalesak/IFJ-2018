/*
**	IFJ
**	Code generation
**	---------------------------
**	Vojtěch Novotný - xnovot1f
*/

#include "generator.h"

/// @var frame - Variable containing a character representing the current frame used to access variables.
/// Can be set to either G or L using the gen_set_frame(int frame_type) function.
char frame = 'G';

/// @var UID - Variable containing the number that was last used for a UID.
/// It is incremented every time a new UID is generated.
int UID = INT_MIN;

// Generation Control Functions
// ============================

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
 * Sets the frame to either local or global. The frame is then used in variable operations.
 * @param frame - Int representing the frame. Can be either GEN_GLOBAL or GEN_LOCAL.
 */
void gen_set_frame(int frame_type) {
    if (frame_type == GEN_GLOBAL)
        frame = 'G';
    else
        frame = 'L';
}


// Function And Control Structure Code Generation
// ==============================================

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
            printf("%cF@%s\n", (char)frame, (char*)(token.data));
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
            break;
    }
}

/*
 * Generates code to create a new temporary frame.
 */
void gen_TF() {
    printf("CREATEFRAME\n");
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

    printf("DEFVAR %cF@%s\nMOVE %cF@%s %cF@%%%d\n", (char)frame, (char*)(token.data), (char)frame, (char*)(token.data), (char)frame, parameterCounter);
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
void gen_retval_def() {
    printf("DEFVAR %cF@%%retval\nMOVE %cF@%%retval nil@nil\n", (char)frame, (char)frame);
}

/*
 * Generates code for assingning return value into the retval variable.
 * @param token - Token that contains the return value.
 */
void gen_retval_ass(Token token) {
    printf("MOVE %cF@%%retval ", (char)frame);

    switch (token.type) {
        case T_IDENTIFIER:
            printf("%cF@%s\n", (char)frame, (char*)(token.data));
            break;
        case T_INT:
            printf("int@%d\n", *(int*)(token.data));
            break;
        case T_FLOAT:
            printf("float@%f\n", *(float*)(token.data));
            break;
        case T_STRING:
            printf("string@%s\n", (char*)(token.data));
            break;
        case PREC_E:
            printf("%cF@%%result%%%x\n", (char)frame, (int)(token.data));
            break;
        default:
            compiler_exit(ERR_INTERNAL);
            break;
    }
}

/*
 * Generates code for return statement.
 */
void gen_return() {
    printf("RETURN\n");
}

/*
 * Generates code that moves return value of a function to the target variable.
 * Called after the end of function call.
 * @param token - Token representing the target variable.
 */
void gen_retval_get(Token token) {
    printf("MOVE %cF@%s TF@%%retval\n", (char)frame, (char*)(token.data));
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
    printf("DEFVAR %cF@%s\n", (char)frame, (char*)(token.data));
}

/*
 * Generates control code based on the result of IF comparison.
 */
void gen_if_cmpResult() {
    int resultID = gen_uniqueID_last();
    int thenID = gen_uniqueID_next();
    int elseID = gen_uniqueID_next();
    printf("JUMPIFEQ IF%%then%%%x %cF@%%result%%%x bool@true\n", thenID, (char)frame, resultID);
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
    printf("JUMPIFEQ WHILE%%end%%%x %cF@WHILE%%result%%%x bool@false\n", endID, (char)frame, resultID);
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

/*
 * Generates IFJcode2018 file header.
 */
void gen_code_header() {
    printf(".IFJcode18\n");
}


int gen_jumparound_jump() {
    int label = gen_uniqueID_next();
    printf("JUMP jumparound%%%x\n", label);
    return label;
}

void gen_jumparound_label(int label) {
    printf("LABEL jumparound%%%x\n", label);
}

// Expression Code Generation
// ==========================

/*
 * Generates code to move a value from one variable to another variable.
 * @param from - Token representing source variable.
 * @param to   - Token representing target variable.
 */
void gen_exp_MOV(Token from, Token to) {
    printf("MOVE %cF@%s %cF@%s\n", (char)frame, (char*)to.data, (char)frame, (char*)from.data);
}

/*
 * Generates code for one argument of an expression.
 * @param token - Token containing information about the argument.
 */
void gen_exp_putArg(Token token) {
    switch (token.type) {
        case T_IDENTIFIER:
            printf("%cF@%s ", (char)frame, (char*)(token.data));
            break;
        case T_INT:
            printf("int@%d ", *(int*)(token.data));
            break;
        case T_FLOAT:
            printf("float@%f ", *(float*)(token.data));
            break;
        case T_STRING:
            printf("string@%s ", (char*)(token.data));
            break;
        case PREC_E:
            printf("%cF@%%tmp%%%x ", (char)frame, (int)(token.data));
            break;
        default:
            compiler_exit(ERR_INTERNAL);
            break;
    }
}

/*
 * Generates a newline at the end of an expression.
 */
void gen_exp_finalize() {
    printf("\n");
}

/*
 * Generates beginning code for a "x = y * z" type expression.
 * Defines variable for storing the result and defines the operation.
 * @return     UID of variable x, that will store the result of the expression.
 */
int gen_exp_MUL() {
    int result = gen_uniqueID_next();

    printf("DEFVAR %cF@%%tmp%%%x\n", (char)frame, result);
    printf("MUL %cF@%%tmp%%%x ", (char)frame, result);

    return result;
}

/*
 * Generates beginning code for a "x = y / z" type expression.
 * Defines variable for storing the result and defines the operation.
 * @return     UID of variable x, that will store the result of the expression.
 */
int gen_exp_DIV() {
    int result = gen_uniqueID_next();

    printf("DEFVAR %cF@%%tmp%%%x\n", (char)frame, result);
    printf("DIV %cF@%%tmp%%%x ", (char)frame, result);

    return result;
}

/*
 * Generates beginning code for a "x = y + z" type expression.
 * Defines variable for storing the result and defines the operation.
 * @return     UID of variable x, that will store the result of the expression.
 */
int gen_exp_ADD() {
    int result = gen_uniqueID_next();

    printf("DEFVAR %cF@%%tmp%%%x\n", (char)frame, result);
    printf("ADD %cF@%%tmp%%%x ", (char)frame, result);

    return result;
}

/*
 * Generates beginning code for a "x = y - z" type expression.
 * Defines variable for storing the result and defines the operation.
 * @return     UID of variable x, that will store the result of the expression.
 */
int gen_exp_SUB() {
    int result = gen_uniqueID_next();

    printf("DEFVAR %cF@%%tmp%%%x\n", (char)frame, result);
    printf("SUB %cF@%%tmp%%%x ", (char)frame, result);

    return result;
}

/*
 * Generates the final result variable of one line of code.
 */
void gen_exp_result(Token token) {
    int result = gen_uniqueID_next();

    printf("DEFVAR %cF@%%result%%%x\n", (char)frame, result);
    printf("MOVE %cF@%%result%%%x ", (char)frame, result);

    switch (token.type) {
        case T_IDENTIFIER:
            printf("%cF@%s\n", (char)frame, (char*)(token.data));
            break;
        case T_INT:
            printf("int@%d\n", *(int*)(token.data));
            break;
        case T_FLOAT:
            printf("float@%f\n", *(float*)(token.data));
            break;
        case T_STRING:
            printf("string@%s\n", (char*)(token.data));
            break;
        case PREC_E:
            printf("%cF@%%tmp%%%x\n", (char)frame, (int)(token.data));
            break;
        default:
            compiler_exit(ERR_INTERNAL);
            break;
    }
}/*
void gen_exp_result(int tmp) {
    int result = gen_uniqueID_next();

    printf("DEFVAR LF@%%result%%%x\n", result);
    printf("MOVE LF@%%result%%%x LF@%%tmp%%%x\n", result, tmp);
}*/


/*
 * Generates beginning code for a "x = y == z" type expression.
 * Defines variable for storing the result and defines the operation.
 * @return     UID of variable x, that will store the result of the expression.
 */
int gen_exp_EQ() {
    int result = gen_uniqueID_next();

    printf("DEFVAR %cF@%%tmp%%%x\n", (char)frame, result);
    printf("EQ %cF@%%tmp%%%x ", (char)frame, result);

    return result;
}

/*
 * Generates beginning code for a "x = !z" type expression.
 * Defines variable for storing the result and defines the operation.
 * @return     UID of variable x, that will store the result of the expression.
 */
int gen_exp_NOT() {
    int result = gen_uniqueID_next();

    printf("DEFVAR %cF@%%tmp%%%x\n", (char)frame, result);
    printf("NOT %cF@%%tmp%%%x ", (char)frame, result);

    return result;
}

/*
 * Generates beginning code for a "x = y < z" type expression.
 * Defines variable for storing the result and defines the operation.
 * @return     UID of variable x, that will store the result of the expression.
 */
int gen_exp_LT() {
    int result = gen_uniqueID_next();

    printf("DEFVAR %cF@%%tmp%%%x\n", (char)frame, result);
    printf("LT %cF@%%tmp%%%x ", (char)frame, result);

    return result;
}

/*
 * Generates beginning code for a "x = y > z" type expression.
 * Defines variable for storing the result and defines the operation.
 * @return     UID of variable x, that will store the result of the expression.
 */
int gen_exp_GT() {
    int result = gen_uniqueID_next();

    printf("DEFVAR %cF@%%tmp%%%x\n", (char)frame, result);
    printf("GT %cF@%%tmp%%%x ", (char)frame, result);

    return result;
}

/*
 * Generates code that assigns result of an expression to the target variable.
 * Called after end of expression analysis.
 * @param token - Token representing the target variable.
 */
void gen_result_ass(Token token) {
    // @var resultID - ID of the result.
    int resultID = gen_uniqueID_last();

    printf("MOVE %cF@%s %cF@%%result%%%x\n", (char)frame, (char*)(token.data), (char)frame, resultID);
}


// Built-in Function Code Generation
// =================================

/*
 * Generates code for built-in function INPUTS().
 */
void gen_bif_inputs(Token token) {
    printf("READ %cF@%s string\n", (char)frame, (char*)(token.data));
}

/*
 * Generates code for built-in function INPUTI().
 */
void gen_bif_inputi(Token token){
    printf("READ %cF@%s int\n", (char)frame, (char*)(token.data));
}

/*
 * Generates code for built-in function INPUTF().
 */
void gen_bif_inputf(Token token){
    printf("READ %cF@%s float\n", (char)frame, (char*)(token.data));
}

/*
 * Generates code for built-in function PRINT().
 * @param args_count - Int with number of arguments of the print function.
 */
void gen_bif_print(int args_count){
    gen_pushframe();
    for (int i = 1; i <= args_count; i++) {
        printf("WRITE LF@%%%d\n", i);
    }
    gen_popframe();
}

/*
 * Generates code for built-in function LENGTH().
 */
void gen_bif_length(){
    printf("DEFVAR %cF@str\nMOVE %cF@str %cF@%%1\n",  (char)frame, (char)frame, (char)frame);
    gen_retval_def();
    printf("STRLEN %cF@%%retval %cF@str", (char)frame, (char)frame);
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
