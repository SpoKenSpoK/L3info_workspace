#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "symboles.h"
#include "premiers.h"
#include "suivants.h"
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "util.h"

int uniteCourante;
char buff[1024];
char val[1024];

void programme( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _optDecVariables_) || est_premier(uniteCourante, _listeDecFonctions_) ){
        optDecVariables();
        listeDecFonctions();
    }
    else if( est_suivant(uniteCourante, _programme_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void optDecVariables( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _listeDecVariables_) ){
        listeDecVariables();
        if( uniteCourante == POINT_VIRGULE ){
            nom_token(uniteCourante, buff, val);
            affiche_element(buff, val, 1);
            uniteCourante = yylex();
        }
        //else erreur(__FUNCTION__);
    }
    else if( est_suivant(uniteCourante, _optDecVariables_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    //else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}


void listeDecVariables( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _declarationVariable_) || est_premier(uniteCourante, _listeDecVariablesBis_) ){
        declarationVariable();
        listeDecVariablesBis();
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }else{
        erreur(__FUNCTION__);
    }


}

void listeDecVariablesBis( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == VIRGULE ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();

        //if( est_premier(uniteCourante, _declarationVariable_) || est_premier(uniteCourante, _listeDecVariablesBis_) ){
        declarationVariable();
        listeDecVariablesBis();

        affiche_balise_fermante(__FUNCTION__, 1);
        return;
        //}
        //else erreur(__FUNCTION__);
    }
    else if( est_suivant(uniteCourante, _listeDecVariablesBis_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    //else erreur(__FUNCTION__);
}

void declarationVariable( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == ENTIER ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();

        if( uniteCourante == ID_VAR ){
            nom_token(uniteCourante, buff, val);
            affiche_element(buff,val,1);
            uniteCourante = yylex();

            //if( est_premier(uniteCourante, _optTailleTableau_) ){
            optTailleTableau();
            //}
            //else erreur(__FUNCTION__);
        }
        //else erreur(__FUNCTION__);
    }
    //selse erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void optTailleTableau( void ){

    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == CROCHET_OUVRANT ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();

        if( uniteCourante == NOMBRE ){
            nom_token(uniteCourante, buff, val);
            affiche_element(buff,val,1);
            uniteCourante = yylex();

            if( uniteCourante == CROCHET_FERMANT ){
                nom_token(uniteCourante, buff, val);
                affiche_element(buff,val,1);
                uniteCourante = yylex();
            }//else erreur(__FUNCTION__);
        }//else erreur(__FUNCTION__);
    }
    else if( est_suivant(uniteCourante, _optTailleTableau_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    //else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void listeDecFonctions( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _declarationFonction_) || est_premier(uniteCourante, _listeDecFonctions_) ){
        declarationFonction();
        listeDecFonctions();
    }
    else if( est_suivant(uniteCourante, _listeDecFonctions_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    //else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void declarationFonction( void ){

    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == ID_FCT ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();

        /*if( est_premier(uniteCourante, _listeParam_) || est_premier(uniteCourante, _optDecVariables_) || est_premier(uniteCourante, _instructionBloc_) ){ */
        listeParam();
        optDecVariables();
        instructionBloc();

        affiche_balise_fermante(__FUNCTION__, 1);
        return;

        //else erreur(__FUNCTION__);
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void listeParam( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == PARENTHESE_OUVRANTE ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();

        //if( est_premier(uniteCourante, _optListeDecVariables_) ){
            optListeDecVariables();

            if( uniteCourante == PARENTHESE_FERMANTE ){
                nom_token(uniteCourante, buff, val);
                affiche_element(buff,val,1);
                uniteCourante = yylex();

                affiche_balise_fermante(__FUNCTION__,1);
                return;

            } //else erreur(__FUNCTION__);
        //} //else erreur(__FUNCTION__);
    } //else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void optListeDecVariables( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _listeDecVariables_) ){
        listeDecVariables();
    }
    else if( est_suivant(uniteCourante, _optListeDecVariables_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }

    affiche_balise_fermante(__FUNCTION__, 1);
}

void instruction( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _instructionAffect_) ){ instructionAffect(); }
    else if( est_premier(uniteCourante, _instructionBloc_)){ instructionBloc(); }
    else if( est_premier(uniteCourante, _instructionSi_)){ instructionSi(); }
    else if( est_premier(uniteCourante, _instructionTantque_)){ instructionTantque(); }
    else if( est_premier(uniteCourante, _instructionAppel_)){ instructionAppel(); }
    else if( est_premier(uniteCourante, _instructionRetour_)){ instructionRetour(); }
    else if( est_premier(uniteCourante, _instructionEcriture_)){ instructionEcriture(); }
    else if( est_premier(uniteCourante, _instructionVide_)){ instructionVide(); }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void instructionAffect( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _var_) ){
        var();
        if( uniteCourante == EGAL ){
            nom_token(uniteCourante, buff, val);
            affiche_element(buff,val,1);
            uniteCourante = yylex();
            //if( est_premier(uniteCourante, _expression_ )){
                expression();
                if(uniteCourante == POINT_VIRGULE){
                    nom_token(uniteCourante, buff, val);
                    affiche_element(buff,val,1);
                    uniteCourante = yylex();
                } //else erreur(__FUNCTION__);
            //} else erreur(__FUNCTION__);
        } else erreur(__FUNCTION__);
    } else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void instructionBloc( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if(uniteCourante == ACCOLADE_OUVRANTE){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();
        if( est_premier(uniteCourante, _listeInstructions_)){
            listeInstructions();
            if(uniteCourante == ACCOLADE_FERMANTE){
                nom_token(uniteCourante, buff, val);
                affiche_element(buff,val,1);
                uniteCourante = yylex();
            } //else erreur(__FUNCTION__);
        } //else erreur(__FUNCTION__);
    } //else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void listeInstructions( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _instruction_) || est_premier(uniteCourante, _listeInstructions_)){
        instruction();
        listeInstructions();
    }
    else if( est_suivant(uniteCourante, _listeInstructions_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void instructionSi( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == SI ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();
        //if( est_premier(uniteCourante, _expression_)){
            expression();
            if( uniteCourante == ALORS ){
                nom_token(uniteCourante, buff, val);
                affiche_element(buff,val,1);
                uniteCourante = yylex();
                //if( est_premier(uniteCourante, _instructionBloc_) || est_premier(uniteCourante, _optSinon_) ){
                    instructionBloc();
                    optSinon();

                    affiche_balise_fermante(__FUNCTION__, 1);
                    return;
                //} //else erreur(__FUNCTION__);
            } //else erreur(__FUNCTION__);
        //} else erreur(__FUNCTION__);
    } else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void optSinon( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == SINON ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();

        if( est_premier(uniteCourante, _instructionBloc_) ){
            instructionBloc();
        } else erreur(__FUNCTION__);
    }
    else if( est_suivant( uniteCourante, _optSinon_ )){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void instructionTantque( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == TANTQUE ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();
            expression();
            if( uniteCourante == FAIRE ){

                nom_token(uniteCourante, buff, val);
                affiche_element(buff,val,1);
                uniteCourante = yylex();

                instructionBloc();

            }
    }

    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void instructionAppel( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _appelFct_) ){
        appelFct();
        if( uniteCourante == POINT_VIRGULE ){
            nom_token(uniteCourante, buff, val);
            affiche_element(buff,val,1);
            uniteCourante = yylex();
        } else erreur(__FUNCTION__);
    } else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void instructionRetour( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == RETOUR ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();
        if( est_premier(uniteCourante, _expression_) ){
            expression();
            if( uniteCourante == POINT_VIRGULE ){
                nom_token(uniteCourante, buff, val);
                affiche_element(buff,val,1);
                uniteCourante = yylex();
            } else erreur(__FUNCTION__);
        } else erreur(__FUNCTION__);
    } else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void instructionEcriture( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == ECRIRE ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();
        if( uniteCourante == PARENTHESE_OUVRANTE ){
            nom_token(uniteCourante, buff, val);
            affiche_element(buff,val,1);
            uniteCourante = yylex();
            if( est_premier(uniteCourante, _expression_) ){
                expression();
                if( uniteCourante == PARENTHESE_FERMANTE ){
                    nom_token(uniteCourante, buff, val);
                    affiche_element(buff,val,1);
                    uniteCourante = yylex();
                    if( uniteCourante == POINT_VIRGULE ){
                        nom_token(uniteCourante, buff, val);
                        affiche_element(buff,val,1);
                        uniteCourante = yylex();
                    } else erreur(__FUNCTION__);
                } else erreur(__FUNCTION__);
            } else erreur(__FUNCTION__);
        } else erreur(__FUNCTION__);
    } else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void instructionVide( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == POINT_VIRGULE ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void expression( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier( uniteCourante, _conjonction_) || est_premier( uniteCourante, _expressionBis_) ){
        conjonction();
        expressionBis();

        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    } else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void expressionBis( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == OU ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();
        //if( est_premier(uniteCourante, _conjonction_) || est_premier(uniteCourante, _expressionBis_) ){
            conjonction();
            expressionBis();

            affiche_balise_fermante(__FUNCTION__, 1);
            return;
        /*}
        else erreur(__FUNCTION__);*/
    }
    else if( est_suivant(uniteCourante, _expressionBis_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    //else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void conjonction( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _comparaison_) || est_premier(uniteCourante, _conjonctionBis_) ){
        comparaison();
        conjonctionBis();
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void conjonctionBis( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == ET ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();

        //if( est_premier(uniteCourante, _comparaison_) || est_premier(uniteCourante, _conjonctionBis_) ){
            comparaison();
            conjonctionBis();

            affiche_balise_fermante(__FUNCTION__, 1);
            return;

        /*}
        else erreur(__FUNCTION__); */
    }
    else if( est_suivant(uniteCourante, _conjonctionBis_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    //else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void comparaison( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _expArith_) || est_premier(uniteCourante, _comparaisonBis_) ){
        expArith();
        comparaisonBis();
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void comparaisonBis( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == EGAL || uniteCourante == INFERIEUR ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();
        //if( est_premier(uniteCourante, _expArith_) || est_premier(uniteCourante, _comparaisonBis_) ){
        expArith();
        comparaisonBis();

        affiche_balise_fermante(__FUNCTION__, 1);
        return;
        //}
        //else erreur(__FUNCTION__);
    }
    else if( est_suivant(uniteCourante, _comparaisonBis_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    //else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void expArith( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _terme_) ||  est_premier(uniteCourante, _expArithBis_) ){
        terme();
        expArithBis();
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void expArithBis( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == PLUS || uniteCourante == MOINS ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();

        //if( est_premier(uniteCourante, _terme_) ||  est_premier(uniteCourante, _expArithBis_) ){
        terme();
        expArithBis();

        affiche_balise_fermante(__FUNCTION__, 1);
        return;
        /*}
        else erreur(__FUNCTION__);*/
    }
    else if( est_suivant(uniteCourante, _expArithBis_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    //else erreur(__FUNCTION__);


}

void terme( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _negation_) || est_premier(uniteCourante, _termeBis_) ){
        negation();
        termeBis();
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void termeBis( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == FOIS || uniteCourante == DIVISE ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();
        //if( est_premier(uniteCourante, _negation_) || est_premier(uniteCourante, _termeBis_) ){
            negation();
            termeBis();
        //} else erreur(__FUNCTION__);
    }
    else if( est_suivant(uniteCourante, _termeBis_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    //else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void negation( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == NON ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();

        if( est_premier(uniteCourante, _negation_) ){
            negation();
        }
        else erreur(__FUNCTION__);
    }
    else if( est_premier(uniteCourante, _facteur_) ){
        facteur();
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void facteur( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == PARENTHESE_OUVRANTE ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();
        //if( est_premier(uniteCourante, _expression_) ){
        expression();
        if( uniteCourante == PARENTHESE_FERMANTE ){
            nom_token(uniteCourante, buff, val);
            affiche_element(buff,val,1);
            uniteCourante = yylex();

        } else erreur(__FUNCTION__);
        //} else erreur(__FUNCTION__);
    }
    else if( uniteCourante == NOMBRE ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();

        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    else if( est_premier(uniteCourante, _appelFct_) ){ appelFct(); }
    else if( est_premier(uniteCourante, _var_) ){ var(); }
    else if( uniteCourante == LIRE ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();
        if( uniteCourante == PARENTHESE_OUVRANTE ){
            nom_token(uniteCourante, buff, val);
            affiche_element(buff,val,1);
            uniteCourante = yylex();
            if( uniteCourante == PARENTHESE_FERMANTE ){
                nom_token(uniteCourante, buff, val);
                affiche_element(buff,val,1);
                uniteCourante = yylex();
            } else erreur(__FUNCTION__);
        } else erreur(__FUNCTION__);
    } else erreur(__FUNCTION__);


    affiche_balise_fermante(__FUNCTION__, 1);
}

void var( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == ID_VAR ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();
        //if( est_premier(uniteCourante, _optIndice_) ){
        optIndice();
        //} else erreur(__FUNCTION__);
    } //else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void optIndice( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == CROCHET_OUVRANT ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();

        //if( est_premier( uniteCourante, _expression_) ){
            expression();
            if( uniteCourante == CROCHET_FERMANT ){
                nom_token(uniteCourante, buff, val);
                affiche_element(buff,val,1);
                uniteCourante = yylex();

                affiche_balise_fermante(__FUNCTION__, 1);
                return;
            }// else erreur(__FUNCTION__);
        //} else erreur(__FUNCTION__);
    }
    else if( est_suivant(uniteCourante, _optIndice_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    //else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void appelFct( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == ID_FCT ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();
        if( uniteCourante == PARENTHESE_OUVRANTE ){
            nom_token(uniteCourante, buff, val);
            affiche_element(buff,val,1);
            uniteCourante = yylex();
            //if( est_premier(uniteCourante, _listeExpressions_) ){
                listeExpressions();
                if( uniteCourante == PARENTHESE_FERMANTE ){
                    nom_token(uniteCourante, buff, val);
                    affiche_element(buff,val,1);
                    uniteCourante = yylex();
                }// else erreur(__FUNCTION__);
            //} else erreur(__FUNCTION__);
        }// else erreur(__FUNCTION__);
    }// else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void listeExpressions( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _expression_) || est_premier(uniteCourante, _listeExpressionsBis_) ){
        expression();
        listeExpressionsBis();
    }
    else if( est_suivant(uniteCourante, _listeExpressions_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void listeExpressionsBis( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == VIRGULE ){
        nom_token(uniteCourante, buff, val);
        affiche_element(buff,val,1);
        uniteCourante = yylex();

        if( est_premier(uniteCourante, _expression_) || est_premier(uniteCourante, _listeExpressionsBis_) ){
            expression();
            listeExpressionsBis();
        } else erreur(__FUNCTION__);
    }
    else if( est_suivant(uniteCourante, _listeExpressionsBis_)){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void test_syntaxique(FILE *yyin) {
    initialise_suivants();
    initialise_premiers();
    uniteCourante = yylex();
    programme();
}
