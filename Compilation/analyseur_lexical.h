#ifndef __ANALYSEUR_LEXICAL__
#define __ANALYSEUR_LEXICAL__

#include "stdio.h"

int yylex(void);
void nom_token( int token, char *nom, char *valeur );
void test_yylex_internal( FILE *yyin );

#endif
