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
  suivants[_instructions][RETOUR] = 1;
  suivants[_instructions][ECRIRE] = 1;
  suivants[_instructions_][POINT_VIRGULE] = 1;

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

  //LI
  suivants[_listeInstructions_][ACCOLADE_FERMANTE] = 1;

  //ISI
  suivants[]


  // TODO: Compléter le tableau suivants ici
}

int est_suivant(int terminal, int non_terminal)
{
  return suivants[non_terminal][terminal];
}
