#pragma once
/*
**	IFJ
**	Code generation
**	---------------------------
**	Vojtěch Novotný - xnovot1f
*/

#include <stdlib.h>
#include <stdio.h>
#include "lexicalanalyzer.h"

#ifndef GEN_H
#define GEN_H

inline int gen_parameter(Token token, int cisloParametru);

inline int gen_TF();

#endif // !GEN_H
