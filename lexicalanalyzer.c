/*
**	IFJ
**	Lexical analysis
**	---------------------------
**	Vojtěch Novotný - xnovot1f
*/

#include "lexicalanalyzer.h"

/// Character comparison macros.
#define IS_LCHAR(c) (c >= 'a' && c <= 'z')
#define IS_UCHAR(c) (c >= 'A' && c <= 'Z')
#define IS_CHAR(c) (IS_LCHAR(c) || IS_UCHAR(c))
#define IS_NUMBER(c) (c >= '0' && c <= '9')
#define IS_WHITESPACE(c) (c == ' ' || c == '\t')
#define IS_OPERATOR(c) (c == '+' || c == '-' || c == '*' || c == '/' || c == '<' || c == '>' || c == '!' || c == '=')
#define IS_TOKENEND(c) (IS_WHITESPACE(c) || IS_OPERATOR(c) || c == ',' || c == '(' || c == ')' || c == 10 || c == 13 || c == EOF || c == '#')

/// EOL
#define EOL 10
#define OLDC_EMPTY -255

/// Number buffer size
#define NBUFFER_MAX 64

/// Enum that defines different states of Lexical Analyzer state machine.
typedef enum {
    AS_EMPTY,
    AS_NUMBER,
    AS_ZERO,
    AS_FLOAT,
    AS_FLOAT_E,
    AS_FLOAT_SIGN,
    AS_ID,
    AS_IDFUNC,
    AS_STRING,
    AS_STRINGEND,
    AS_ESCAPE,
    AS_ESCAPEHEX1,
    AS_ESCAPEHEX2,
    AS_OPERATOR_LT,
    AS_OPERATOR_GT,
    AS_OPERATOR_NOT,
    AS_OPERATOR_ASS,
    AS_COMMENT_LINE,
    AS_COMMENT_BLOCK_BEGIN,
    AS_COMMENT_BLOCK,
    AS_COMMENT_BLOCK_EOL,
    AS_COMMENT_BLOCK_END,
    AS_DONE,
    AS_ERROR
} AnalyzerState;

/// Gets a sequence of characters from STDIN that corresponds to one token.
/// Function is based on a finite state machine.
/// returns     token of type typedef struct Token.
///             Token contains int type that defines the type of token and
///             void* data that contains any additional data about token.
Token getToken() {

    // Stores the last character of the last call of getToken.
    static int oldC = OLDC_EMPTY;

    // Stores whether the last char was EOL to detect multiline comments.
    // Multiline comments have to begin and end on new new lines.
    static int wasEOL = 1;

    // Current state of Lexical Analyzer
    AnalyzerState state = AS_EMPTY;

    char nBuffer[NBUFFER_MAX] = {0};
    string sBuffer;
    int nBuffer_i = 0;
    int hexBuffer = 0;

    // Token return value
    Token token;
    token.type = T_ERROR;

    while (state != AS_DONE && state != AS_ERROR) {

        // variable containing character that is currently being inspected
        int c;

        // The last character of some calls of getToken is stored in oldC.
        // When getToken is called, the first character is loaded from this variable
        // instead of geting it for STDIN.
        if (oldC != OLDC_EMPTY) {
            c = oldC;
            oldC = OLDC_EMPTY;
        } else
            c = getc(stdin);

        // STATE MACHINE LOGIC
        // AS is Analyzer State
        switch (state) {
            case AS_EMPTY:
                if (c == '_' || IS_CHAR(c)) {
                    strInit(&sBuffer);
                    strAddChar(&sBuffer, (char)c);
                    state = AS_ID;;
                } else if (IS_NUMBER(c) && c != '0') {
                    nBuffer[nBuffer_i] = (char) c;
                    nBuffer_i++;
                    state = AS_NUMBER;
                } else if (c == '0') {
                    token.type = T_INT;
                    state = AS_ZERO;
                } else if (c == '\"') {
                    strInit(&sBuffer);
                    state = AS_STRING;
                } else if (c == EOL) {
                    token.type = T_EOL;
                    wasEOL = 1;
                    state = AS_DONE;
                } else if (c == EOF) {
                    token.type = T_EOF;
                    state = AS_DONE;
                } else if (c == ',') {
                    token.type = T_COMMA;
                    state = AS_DONE;
                } else if (c == '+') {
                    token.type = OP_ADD;
                    state = AS_DONE;
                } else if (c == '-') {
                    token.type = OP_SUB;
                    state = AS_DONE;
                } else if (c == '*') {
                    token.type = OP_MUL;
                    state = AS_DONE;
                } else if (c == '/') {
                    token.type = OP_DIV;
                    state = AS_DONE;
                } else if (c == '<') {
                    state = AS_OPERATOR_LT;
                } else if (c == '>') {
                    state = AS_OPERATOR_GT;
                } else if (c == '!') {
                    state = AS_OPERATOR_NOT;
                } else if (c == '=') {
                    if (wasEOL == 1)
                        state = AS_COMMENT_BLOCK_BEGIN;
                    else
                        state = AS_OPERATOR_ASS;
                } else if (c == '(') {
                    token.type = T_LBRACKET;
                    state = AS_DONE;
                } else if (c == ')') {
                    token.type = T_RBRACKET;
                    state = AS_DONE;
                } else if (c == '#') {
                    state = AS_COMMENT_LINE;
                } else if (IS_WHITESPACE(c)) {
                    // do nothing
                } else {
                    state = AS_ERROR;
                }
                break;

            // Analyzer found number on input
            // Number can stay as an int literal or it can transform into float.
            case AS_NUMBER:
                if (IS_NUMBER(c)) {
                    if (nBuffer_i < NBUFFER_MAX) {
                        nBuffer[nBuffer_i] = (char) c;
                        nBuffer_i++;
                    } else {
                        fprintf(stderr, "Number too big.");
                        token.type = T_ERROR;
                        break;
                    }

                } else if (c == '.') {
                    if (nBuffer_i < NBUFFER_MAX) {
                        nBuffer[nBuffer_i] = (char) c;
                        nBuffer_i++;
                    } else {
                        fprintf(stderr, "Number too big.");
                        token.type = T_ERROR;
                        break;
                    }
                    state = AS_FLOAT;

                } else if (c == 'e' || c == 'E') {
                    if (nBuffer_i < NBUFFER_MAX) {
                        nBuffer[nBuffer_i] = (char) c;
                        nBuffer_i++;
                    } else {
                        fprintf(stderr, "Number too big.");
                        token.type = T_ERROR;
                        break;
                    }
                    state = AS_FLOAT_E;

                } else if (IS_TOKENEND(c)) {
                    token.type = T_INT;
                    int value = (int) strtol(nBuffer, NULL, 10);
                    token.data = malloc(sizeof(int));
                    *(int *) (token.data) = value;
                    oldC = c;
                    state = AS_DONE;
                } else
                    state = AS_ERROR;
                break;

            // If a token starts with 0, it can only be 0.
            case AS_ZERO:
                if (IS_TOKENEND(c)) {
                    token.data = malloc(sizeof(int));
                    *(int *) (token.data) = 0;
                    oldC = c;
                    state = AS_DONE;
                } else {
                    state = AS_ERROR;
                }
                break;

            // Cases for float literal.
            case AS_FLOAT:
                if (IS_NUMBER(c)) {
                    if (nBuffer_i < NBUFFER_MAX) {
                        nBuffer[nBuffer_i] = (char) c;
                        nBuffer_i++;
                    } else {
                        fprintf(stderr, "Number too big.");
                        token.type = T_ERROR;
                        break;
                    }
                } else if (c == 'e' || c == 'E') {
                    if (nBuffer_i < NBUFFER_MAX) {
                        nBuffer[nBuffer_i] = (char) c;
                        nBuffer_i++;
                    } else {
                        fprintf(stderr, "Number too big.");
                        token.type = T_ERROR;
                        break;
                    }

                    state = AS_FLOAT_E;
                } else if (IS_TOKENEND(c)) {
                    token.type = T_FLOAT;
                    float value = (float) strtod(nBuffer, NULL);
                    token.data = malloc(sizeof(float));
                    *(float *) (token.data) = value;
                    oldC = c;
                    state = AS_DONE;
                }
                break;

            case AS_FLOAT_E:
                if (IS_NUMBER(c)) {
                    if (nBuffer_i < NBUFFER_MAX) {
                        nBuffer[nBuffer_i] = (char) c;
                        nBuffer_i++;
                    } else {
                        fprintf(stderr, "Number too big.");
                        token.type = T_ERROR;
                        break;
                    }
                } else if (c == '+' || c == '-') {
                    if (nBuffer_i < NBUFFER_MAX) {
                        nBuffer[nBuffer_i] = (char) c;
                        nBuffer_i++;
                    } else {
                        fprintf(stderr, "Number too big.");
                        token.type = T_ERROR;
                        break;
                    }

                    state = AS_FLOAT_SIGN;
                } else if (IS_TOKENEND(c)) {
                    token.type = T_FLOAT;
                    float value = (float) strtod(nBuffer, NULL);
                    token.data = malloc(sizeof(float));
                    *(float *) (token.data) = value;
                    oldC = c;
                    state = AS_DONE;
                } else {
                    token.type = T_ERROR;
                }
                break;

            case AS_FLOAT_SIGN:
                if (IS_NUMBER(c)) {
                    if (nBuffer_i < NBUFFER_MAX) {
                        nBuffer[nBuffer_i] = (char) c;
                        nBuffer_i++;
                    } else {
                        fprintf(stderr, "Number too big.");
                        token.type = T_ERROR;
                        break;
                    }
                } else if (IS_TOKENEND(c)) {
                    token.type = T_FLOAT;
                    float value = (float) strtod(nBuffer, NULL);
                    token.data = malloc(sizeof(float));
                    *(float *) (token.data) = value;
                    oldC = c;
                    state = AS_DONE;
                } else {
                    state = AS_ERROR;
                }
                break;

            // Cases for string literal.
            case AS_STRING:
                if (c == '\"') {
                    state = AS_STRINGEND;
                } else if (c == '\\') {
                    state = AS_ESCAPE;
                } else if (c > 31) {
                    strAddChar(&sBuffer, (char)c);
                } else {
                    strFree(&sBuffer);
                    state = AS_ERROR;
                }
                break;

            case AS_STRINGEND:
                if (IS_TOKENEND(c)) {
                    token.type = T_STRING;
                    (token.data) = strGetStr(&sBuffer);
                    oldC = c;
                    state = AS_DONE;
                } else {
                    strFree(&sBuffer);
                    state = AS_ERROR;
                }
                break;

            // Cases for escape sequences in string literals.
            case AS_ESCAPE:
                if (c == '\"' || c == '\\') {
                    strAddChar(&sBuffer, (char)c);
                    state = AS_STRING;
                } else if (c == 't') {
                    strAddChar(&sBuffer, '\t');
                    state = AS_STRING;
                } else if (c == 'n') {
                    strAddChar(&sBuffer, '\n');
                    state = AS_STRING;
                } else if (c == 'x') {
                    state = AS_ESCAPEHEX1;
                } else {
                    strFree(&sBuffer);
                    state = AS_ERROR;
                }
                break;

            case AS_ESCAPEHEX1:
                if (IS_NUMBER(c)) {
                    hexBuffer = c - '0';
                    state = AS_ESCAPEHEX2;
                } else if (c >= 'a' && c <= 'f') {
                    hexBuffer = c - 'a' + 10;
                    state = AS_ESCAPEHEX2;
                } else if (c >= 'A' && c <= 'F') {
                    hexBuffer = c - 'A' + 10;
                    state = AS_ESCAPEHEX2;
                } else {
                    strFree(&sBuffer);
                    state = AS_ERROR;
                }
                break;

            case AS_ESCAPEHEX2:
                if (IS_NUMBER(c)) {
                    hexBuffer *= 16;
                    hexBuffer += c - '0';
                    strAddChar(&sBuffer, (char) hexBuffer);
                    state = AS_STRING;
                } else if (c >= 'a' && c <= 'f') {
                    hexBuffer *= 16;
                    hexBuffer += c - 'a' + 10;
                    strAddChar(&sBuffer, (char) hexBuffer);
                    state = AS_STRING;
                } else if (c >= 'A' && c <= 'F') {
                    hexBuffer *= 16;
                    hexBuffer += c - 'A' + 10;
                    strAddChar(&sBuffer, (char) hexBuffer);
                    state = AS_STRING;
                } else if (c == '\"') {
                    state = AS_STRINGEND;
                } else if (c == '\\') {
                    state = AS_ESCAPE;
                } else if (c > 31) {
                    strAddChar(&sBuffer, (char)c);
                    state = AS_STRING;
                } else {
                    strFree(&sBuffer);
                    state = AS_ERROR;
                }
                break;

            // Case for identifier token.
            case AS_ID:
                if (IS_CHAR(c) || IS_NUMBER(c) || c == '_') {
                    strAddChar(&sBuffer, (char)c);

                // Characters ! and ? can only be at the end of an identifier token.
                } else if (c == '!' || c == '?') {
                    strAddChar(&sBuffer, (char)c);
                    state = AS_IDFUNC;

                // When a ID token ends, it is tested for keywords.
                } else if (IS_TOKENEND(c)) {
                    if (strCmpConstStr(&sBuffer, "def") == 0) {
                        token.type = KW_DEF;
                    } else if (strCmpConstStr(&sBuffer, "do") == 0) {
                        token.type = KW_DO;
                    } else if (strCmpConstStr(&sBuffer, "else") == 0) {
                        token.type = KW_ELSE;
                    } else if (strCmpConstStr(&sBuffer, "end") == 0) {
                        token.type = KW_END;
                    } else if (strCmpConstStr(&sBuffer, "if") == 0) {
                        token.type = KW_IF;
                    } else if (strCmpConstStr(&sBuffer, "nil") == 0) {
                        token.type = KW_NIL;
                    } else if (strCmpConstStr(&sBuffer, "not") == 0) {
                        token.type = KW_NOT;
                    } else if (strCmpConstStr(&sBuffer, "then") == 0) {
                        token.type = KW_THEN;
                    } else if (strCmpConstStr(&sBuffer, "while") == 0) {
                        token.type = KW_WHILE;
                    } else if (strCmpConstStr(&sBuffer, "inputs") == 0) {
                        token.type = BIF_INPUTS;
                    } else if (strCmpConstStr(&sBuffer, "inputi") == 0) {
                        token.type = BIF_INPUTI;
                    } else if (strCmpConstStr(&sBuffer, "inputf") == 0) {
                        token.type = BIF_INPUTF;
                    } else if (strCmpConstStr(&sBuffer, "print") == 0) {
                        token.type = BIF_PRINT;
                    } else if (strCmpConstStr(&sBuffer, "length") == 0) {
                        token.type = BIF_LENGTH;
                    } else if (strCmpConstStr(&sBuffer, "substr") == 0) {
                        token.type = BIF_SUBSTR;
                    } else if (strCmpConstStr(&sBuffer, "ord") == 0) {
                        token.type = BIF_ORD;
                    } else if (strCmpConstStr(&sBuffer, "chr") == 0) {
                        token.type = BIF_CHR;
                    } else {
                        token.type = T_IDENTIFIER;
                        (token.data) = strGetStr(&sBuffer);
                    }

                    oldC = c;
                    state = AS_DONE;
                } else {
                    strFree(&sBuffer);
                    state = AS_ERROR;
                }
                break;

            case AS_IDFUNC:
                if (IS_TOKENEND(c)) {
                    token.type = T_FUNCTION;
                    (token.data) = strGetStr(&sBuffer);
                    oldC = c;
                    state = AS_DONE;
                } else {
                    strFree(&sBuffer);
                    state = AS_ERROR;
                }
                break;

            // Cases for different operators.
            case AS_OPERATOR_LT:
                if (c == '=') {
                    token.type = OP_LTE;
                    state = AS_DONE;
                } else {
                    token.type = OP_LT;
                    oldC = c;
                    state = AS_DONE;
                }
                break;

            case AS_OPERATOR_GT:
                if (c == '=') {
                    token.type = OP_GTE;
                    state = AS_DONE;
                } else {
                    token.type = OP_GT;
                    oldC = c;
                    state = AS_DONE;
                }
                break;

            case AS_OPERATOR_NOT:
                if (c == '=') {
                    token.type = OP_NEQ;
                    state = AS_DONE;
                } else {
                    state = AS_ERROR;
                }
                break;

            case AS_OPERATOR_ASS:
                if (c == '=') {
                    token.type = OP_EQ;
                    state = AS_DONE;
                } else {
                    token.type = OP_ASS;
                    oldC = c;
                    state = AS_DONE;
                }
                break;

            // Case for single line comment.
            // Line breaks at the end of single line comments are sent as EOL tokens.
            case AS_COMMENT_LINE:
                if (c == EOL) {
                    token.type = T_EOL;
                    wasEOL = 1;
                    state = AS_DONE;
                }
                break;


            // Multiline comment cases.
            // Tests for "=beginWHITESPACE" or "=beginEOL" at the beginning of a line.
            // If it is successful than opens a multiline comment, if not returns Lexical Error.
            case AS_COMMENT_BLOCK_BEGIN:
                if (c == 'b' && getc(stdin) == 'e' && getc(stdin) == 'g' && getc(stdin) == 'i' && getc(stdin) == 'n') {
                    int c2 = getc(stdin);
                    if (IS_WHITESPACE(c2) || c2 == EOL)
                        state = AS_COMMENT_BLOCK;
                    else
                        state = AS_ERROR;
                }
                else
                    state = AS_ERROR;
                break;

            case AS_COMMENT_BLOCK:
                if (c == EOL)
                    state = AS_COMMENT_BLOCK_EOL;
                else if (c == EOF) {
                    token.type = T_EOF;
                    state = AS_DONE;
                }
                break;

            case AS_COMMENT_BLOCK_EOL:
                if (c == '=' && getc(stdin) == 'e' && getc(stdin) == 'n' && getc(stdin) == 'd') {
                    int c2 = getc(stdin);
                    if (IS_WHITESPACE(c2))
                        state = AS_COMMENT_BLOCK_END;
                    else if (c2 == EOL) {
                        state = AS_EMPTY;
                        wasEOL = 1;
                    }
                    else if (c == EOF) {
                        token.type = T_EOF;
                        state = AS_DONE;
                    }
                    else
                        state = AS_ERROR;
                }
                else if (c == EOL)
                    state = AS_COMMENT_BLOCK_EOL;
                else
                    state = AS_COMMENT_BLOCK;
                break;

            case AS_COMMENT_BLOCK_END:
                if (c == EOL) {
                    state = AS_EMPTY;
                    wasEOL = 1;
                }
                else if (c == EOF) {
                    token.type = T_EOF;
                    state = AS_DONE;
                }
                break;

            // Invalid analyzer state case.
            default:
                state = AS_ERROR;
                break;
        }
    }

    // wasEOL is only relevant for the first token after the EOL,
    // after that it has to be reset.
    if (state == AS_DONE) {
        if (token.type != T_EOL && wasEOL == 1)
            wasEOL = 0;
    }

    // If the analyzer finishes in error state, exits with lexical analysis error code 1.
    if (state == AS_ERROR) {
        exit(1);
        //token.type = T_ERROR;
    }

    //tester(token);
    return token;
}
