#ifndef __ANALYSEUR_SYNTAXIQUE__
#define __ANALYSEUR_SYNTAXIQUE__

#include <stdio.h>
#include "syntabs.h"

void readToken();
n_prog* programme();
n_l_dec* optDecVariables();
n_l_dec* listeDecVariables();
n_l_dec* listeDecVariablesBis(n_l_dec* );
n_dec* declarationVariable();
n_l_instr* listeInstructions();
int optTailleTableau();
n_l_dec* listeDecFonctions();
n_dec* declarationFonction();
n_l_dec* listeParam();
n_l_dec* optListeDecVariables();
n_instr* instruction();
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
n_exp* expression();
n_exp* expressionBis(n_exp*);
n_exp* conjonction();
n_exp* conjonctionBis(n_exp* );
n_exp* comparaison();
n_exp* comparaisonBis(n_exp* );
n_exp* expArith();
n_exp* expArithBis(n_exp* );
n_exp* terme();
n_exp* termeBis(n_exp* );
n_exp* negation();
n_exp* facteur();
n_var* var();
n_exp* optIndice();
n_appel* appelFct();
n_l_exp* listeExpressions();
n_l_exp* listeExpressionsBis(n_l_exp* );

void test_syntaxique(FILE* ,int , int);

#endif
