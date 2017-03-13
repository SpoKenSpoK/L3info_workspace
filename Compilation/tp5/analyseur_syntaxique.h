#ifndef __ANALYSEUR_SYNTAXIQUE__
#define __ANALYSEUR_SYNTAXIQUE__

#include <stdio.h>
#include "syntabs.h"

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
n_instr* instructionFaire();
n_instr* instructionAffect();
n_instr* instructionBloc();
n_instr* instructionSi();
n_instr* instructionTantque();
n_instr* instructionAppel();
n_instr* instructionRetour();
n_instr* instructionEcriture();
n_instr* instructionVide();
n_instr* optSinon();
void expression();
void expressionBis();
void conjonction();
void conjonctionBis();
void comparaison();
void comparaisonBis();
void expArith();
void expArithBis();
void terme();
n_exp* termeBis(n_exp* );
n_exp* negation();
n_exp* facteur();
void var();
void optIndice();
void appelFct();
void listeExpressions();
void listeExpressionsBis();

void test_syntaxique(FILE* );

#endif
