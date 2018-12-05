/*
**	IFJ
**	Implementation of dealing with infinite strings
**	---------------------------
**	Tomáš Zálešák - xzales13
**  Inspired by https://www.fit.vutbr.cz/study/courses/IFJ/private/projekt/jednoduchy_interpret.zip
*/


#ifndef STRING_H
#define STRING_H

#define STR_ERROR   1
#define STR_SUCCESS 0

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
