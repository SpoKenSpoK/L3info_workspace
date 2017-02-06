#include"symboles.h"
#include"premiers.h"

void initialise_premiers(void){
    int i,j;

    // Initialiser toutes les cases du tableau à 0
    for(i=0; i <= NB_NON_TERMINAUX; i++)
        for(j=0; j <= NB_TERMINAUX; j++)
            premiers[i][j] = 0;

    //PG
    premiers[_programme_][ENTIER] = 1;
    premiers[_programme_][EPSILON] = 1;

    //ODV
    premiers[_optDecVariables_][ENTIER] = 1;
    premiers[_optDecVariables_][EPSILON] = 1;

    //LDV
    premiers[_listeDecVariables_][ENTIER] = 1;

    //LDVB
    premiers[_listeDecVariablesBis_][VIRGULE] = 1;
    premiers[_listeDecVariablesBis_][EPSILON] = 1;

    //DV
    premiers[_declarationVariable_][ENTIER] = 1;

    //OTT
    premiers[_optTailleTableau_][CROCHET_OUVRANT] = 1;
    premiers[_optTailleTableau_][EPSILON] = 1;

    //LDF
    premiers[_listeDecFonctions_][ID_FCT] = 1;
    premiers[_listeDecFonctions_][EPSILON] = 1;

    //DF
    premiers[_declarationFonction_][ID_FCT] = 1;

    //LP
    premiers[_listeParam_][PARENTHESE_OUVRANTE] = 1;

    //OLDV
    premiers[_optListeDecVariables_][ENTIER] = 1;
    premiers[_optListeDecVariables_][EPSILON] = 1;

    //I
    premiers[_instruction_][ID_VAR] = 1;
    premiers[_instruction_][ACCOLADE_OUVRANTE] = 1;
    premiers[_instruction_][SI] = 1;
    premiers[_instruction_][TANTQUE] = 1;
    premiers[_instruction_][ID_FCT] = 1;
    premiers[_instruction_][RETOUR] = 1;
    premiers[_instruction_][ECRIRE] = 1;
    premiers[_instruction_][POINT_VIRGULE] = 1;

    //IAFF
    premiers[_instructionAffect_][]


    premiers



    // TODO: Compléter le tableau premiers ici
    }

int est_premier(int non_terminal, int terminal)
{
  return premiers[terminal][non_terminal];
}
