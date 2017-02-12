#include"symboles.h"
#include"suivants.h"

void initialise_suivants(void){
  int i,j;

  // Initialiser toutes les cases du tableau à 0
  for(i=0; i <= NB_NON_TERMINAUX; i++)
    for(j=0; j <= NB_TERMINAUX; j++)
      suivants[i][j] = 0;

  //exemple

  //PG
  suivants[_programme_][FIN] = 1;

  //ODV
  suivants[_optDecVariables_][ACCOLADE_OUVRANTE] = 1;
  suivants[_optDecVariables_][ID_FCT] = 1;

  //LDV
  suivants[_listeDecVariables_][POINT_VIRGULE] = 1;
  suivants[_listeDecVariables_][PARENTHESE_FERMANTE] = 1;

  //LDVB
  suivants[_listeDecVariablesBis_][POINT_VIRGULE] = 1;
  suivants[_listeDecVariablesBis_][PARENTHESE_FERMANTE] = 1;

  //DV
  suivants[_declarationVariable_][VIRGULE] = 1;

  //OTT
  suivants[_optTailleTableau_][VIRGULE] = 1;

  //LDF
  suivants[_listeDecFonctions_][FIN] = 1;

  //DF
  suivants[_declarationFonction_][ID_FCT] = 1;

  //LP
  suivants[_listeParam_][ENTIER] = 1;

  //OLDV
  suivants[_optListeDecVariables_][PARENTHESE_FERMANTE] = 1;

  //I
  suivants[_instruction_][ID_VAR] = 1;
  suivants[_instruction_][ACCOLADE_OUVRANTE] = 1;
  suivants[_instruction_][SI] = 1;
  suivants[_instruction_][TANTQUE] = 1;
  suivants[_instruction_][ID_FCT] = 1;
  suivants[_instruction_][RETOUR] = 1;
  suivants[_instruction_][ECRIRE] = 1;
  suivants[_instruction_][POINT_VIRGULE] = 1;

  //IAFF
  suivants[_instructionAffect_][ID_VAR] = 1;
  suivants[_instructionAffect_][ACCOLADE_OUVRANTE] = 1;
  suivants[_instructionAffect_][SI] = 1;
  suivants[_instructionAffect_][TANTQUE] = 1;
  suivants[_instructionAffect_][ID_FCT] = 1;
  suivants[_instructionAffect_][RETOUR] = 1;
  suivants[_instructionAffect_][ECRIRE] = 1;
  suivants[_instructionAffect_][POINT_VIRGULE] = 1;

  //IB
  suivants[_instructionBloc_][SINON] = 1;
  suivants[_instructionBloc_][ACCOLADE_OUVRANTE] = 1;
  suivants[_instructionBloc_][SI] = 1;
  suivants[_instructionBloc_][TANTQUE] = 1;
  suivants[_instructionBloc_][ID_FCT] = 1;
  suivants[_instructionBloc_][RETOUR] = 1;
  suivants[_instructionBloc_][ECRIRE] = 1;
  suivants[_instructionBloc_][POINT_VIRGULE] = 1;
  suivants[_instructionBloc_][SINON] = 1;

  //LI
  suivants[_listeInstructions_][ACCOLADE_FERMANTE] = 1;

  //ISI
  suivants[_instructionSi_][ID_VAR] = 1;
  suivants[_instructionSi_][ACCOLADE_OUVRANTE] = 1;
  suivants[_instructionSi_][SI] = 1;
  suivants[_instructionSi_][TANTQUE] = 1;
  suivants[_instructionSi_][ID_FCT] = 1;
  suivants[_instructionSi_][RETOUR] = 1;
  suivants[_instructionSi_][ECRIRE] = 1;
  suivants[_instructionSi_][POINT_VIRGULE] = 1;

  //OSINON
  suivants[_optSinon_][ID_VAR] = 1;
  suivants[_optSinon_][ACCOLADE_OUVRANTE] = 1;
  suivants[_optSinon_][SI] = 1;
  suivants[_optSinon_][TANTQUE] = 1;
  suivants[_optSinon_][ID_FCT] = 1;
  suivants[_optSinon_][RETOUR] = 1;
  suivants[_optSinon_][ECRIRE] = 1;
  suivants[_optSinon_][POINT_VIRGULE] = 1;

  //ITQ
  suivants[_instructionTantque_][ID_VAR] = 1;
  suivants[_instructionTantque_][ACCOLADE_OUVRANTE] = 1;
  suivants[_instructionTantque_][SI] = 1;
  suivants[_instructionTantque_][TANTQUE] = 1;
  suivants[_instructionTantque_][ID_FCT] = 1;
  suivants[_instructionTantque_][RETOUR] = 1;
  suivants[_instructionTantque_][ECRIRE] = 1;
  suivants[_instructionTantque_][POINT_VIRGULE] = 1;

  //IAPP
  suivants[_instructionAppel_][ID_VAR] = 1;
  suivants[_instructionAppel_][ACCOLADE_OUVRANTE] = 1;
  suivants[_instructionAppel_][SI] = 1;
  suivants[_instructionAppel_][TANTQUE] = 1;
  suivants[_instructionAppel_][ID_FCT] = 1;
  suivants[_instructionAppel_][RETOUR] = 1;
  suivants[_instructionAppel_][ECRIRE] = 1;
  suivants[_instructionAppel_][POINT_VIRGULE] = 1;

  //IRET
  suivants[_instructionRetour_][ID_VAR] = 1;
  suivants[_instructionRetour_][ACCOLADE_OUVRANTE] = 1;
  suivants[_instructionRetour_][SI] = 1;
  suivants[_instructionRetour_][TANTQUE] = 1;
  suivants[_instructionRetour_][ID_FCT] = 1;
  suivants[_instructionRetour_][RETOUR] = 1;
  suivants[_instructionRetour_][ECRIRE] = 1;
  suivants[_instructionRetour_][POINT_VIRGULE] = 1;

  //IECR
  suivants[_instructionEcriture_][ID_VAR] = 1;
  suivants[_instructionEcriture_][ACCOLADE_OUVRANTE] = 1;
  suivants[_instructionEcriture_][SI] = 1;
  suivants[_instructionEcriture_][TANTQUE] = 1;
  suivants[_instructionEcriture_][ID_FCT] = 1;
  suivants[_instructionEcriture_][RETOUR] = 1;
  suivants[_instructionEcriture_][ECRIRE] = 1;
  suivants[_instructionEcriture_][POINT_VIRGULE] = 1;

  //IVIDE
  suivants[_instructionVide_][ID_VAR] = 1;
  suivants[_instructionVide_][ACCOLADE_OUVRANTE] = 1;
  suivants[_instructionVide_][SI] = 1;
  suivants[_instructionVide_][TANTQUE] = 1;
  suivants[_instructionVide_][ID_FCT] = 1;
  suivants[_instructionVide_][RETOUR] = 1;
  suivants[_instructionVide_][ECRIRE] = 1;
  suivants[_instructionVide_][POINT_VIRGULE] = 1;

  //EXP
  suivants[_expression_][CROCHET_FERMANT] = 1;
  suivants[_expression_][PARENTHESE_FERMANTE] = 1;
  suivants[_expression_][VIRGULE] = 1;
  suivants[_expression_][POINT_VIRGULE] = 1;
  suivants[_expression_][ALORS] = 1;
  suivants[_expression_][FAIRE] = 1;

  //EXPB
  suivants[_expressionBis_][CROCHET_FERMANT] = 1;
  suivants[_expressionBis_][PARENTHESE_FERMANTE] = 1;
  suivants[_expressionBis_][VIRGULE] = 1;
  suivants[_expressionBis_][POINT_VIRGULE] = 1;
  suivants[_expressionBis_][ALORS] = 1
  suivants[_expressionBis_][FAIRE] = 1;

  //CONJ
  suivants[_conjonction_][OU] = 1;

  //CONJB
  suivants[_conjonctionBis_][OU] = 1;

  //COMP
  suivants[_comparaison_][ET] = 1;

  //COMPBs
  suivants[_comparaisonBis_][ET] = 1;

  //E
  suivants[_expArith_][INFERIEUR] = 1;
  suivants[_expArith_][EGAL] = 1;

  //EB
  suivants[_expArithBis_][INFERIEUR] = 1;
  suivants[_expArithBis_][EGAL] = 1;

  //T
  suivants[_terme_][PLUS] = 1;
  suivants[_terme_][MOINS] = 1;

  //TB
  suivants[_termeBis_][PLUS] = 1;
  suivants[_termeBis_][MOINS] = 1;

  //NEG
  suivants[_negation_][DIVISE] = 1;
  suivants[_negation_][FOIS] = 1;

  //F
  suivants[_negation_][DIVISE] = 1;
  suivants[_negation_][FOIS] = 1;

  //VAR
  suivants[_var_][EGAL] = 1;

  //OIND
  suivants[_optIndice_][EGAL] = 1;

  //APPF
  suivants[_appelFct_][FOIS] = 1;
  suivants[_appelFct_][DIVISE] = 1;
  suivants[_appelFct_][POINT_VIRGULE] = 1;

  //LEXP
  suivants[_listeExpressions_][PARENTHESE_FERMANTE] = 1;

  //LEXPB
  suivants[_listeExpressionsBis_][PARENTHESE_FERMANTE] = 1;
}

int est_suivant(int terminal, int non_terminal)
{
  return suivants[non_terminal][terminal];
}
