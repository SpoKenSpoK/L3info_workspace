#ifndef __ANALYSEUR_SYNTAXIQUE__
#define __ANALYSEUR_SYNTAXIQUE__

#include "stdio.h"

void readToken();
void programme();
void optDecVariables();
void listeDecVariables();
void listeDecVariablesBis();
void declarationVariable();
void listeInstructions();
void optTailleTableau();
void listeDecFonctions();
void declarationFonction();
void listeParam();
void optListeDecVariables();
void instruction();
void instructionFaire();
void instructionAffect();
void instructionBloc();
void instructionSi();
void instructionTantque();
void instructionAppel();
void instructionRetour();
void instructionEcriture();
void instructionVide();
void optSinon();
void expression();
void expressionBis();
void conjonction();
void conjonctionBis();
void comparaison();
void comparaisonBis();
void expArith();
void expArithBis();
void terme();
void termeBis();
void negation();
void facteur();
void var();
void optIndice();
void appelFct();
void listeExpressions();
void listeExpressionsBis();

void test_syntaxique(FILE* );

#endif
