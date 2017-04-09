#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "symboles.h"

char yytext[100];
FILE *yyin;

// argv[1] => nom du fichier
int main(int argc, char **argv) {
  yyin = fopen(argv[2], "r");
  if(yyin == NULL){
    fprintf(stderr, "impossible d'ouvrir le fichier %s\n", argv[1]);
    exit(1);
  }

  if( strcmp( argv[1], "-l") == 0 )  test_yylex_internal(yyin);         // Lexical
  if( strcmp( argv[1], "-s") == 0 )  test_syntaxique(yyin, 1, 0, 0);    // Syntaxique
  if( strcmp( argv[1], "-a") == 0 )  test_syntaxique(yyin, 0, 1, 0);    // Abstrait
  if( strcmp( argv[1], "-t") == 0 )  test_syntaxique(yyin, 0, 1, 1);    // TabSymboles

  return 0;
}
