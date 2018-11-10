// Tomáš Zálešák
// Header file for dealing with infinite strings
// Inspired by https://www.fit.vutbr.cz/study/courses/IFJ/private/projekt/jednoduchy_interpret.zip


#ifndef TEST_STRING_H
#define TEST_STRING_H

typedef struct
{
    char* str;		// string ending with '\0'
    int length;		// real length of the string
    int allocSize;	// size of allocated memory
} string;


int strInit(string *s);
void strFree(string *s);

void strClear(string *s);
int strAddChar(string *s1, char c);
int strAddString(string *s1, char *s2);
int strCopyString(string *s1, string *s2);
int strCmpString(string *s1, string *s2);
int strCmpConstStr(string *s1, char *s2);

char *strGetStr(string *s);
int strGetLength(string *s);

#endif //TEST_STRING_H
