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

void readToken(){
    nom_token(uniteCourante, buff, val);
    affiche_element(buff, val, 1);
    uniteCourante = yylex();
}

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
        if( uniteCourante != POINT_VIRGULE ) erreur(__FUNCTION__);
        readToken();
    }
    else if( est_suivant(uniteCourante, _optDecVariables_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}


void listeDecVariables( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _declarationVariable_) ){
        declarationVariable();
        listeDecVariablesBis();
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void listeDecVariablesBis( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == VIRGULE ){
        readToken();
        declarationVariable();
        listeDecVariablesBis();
    }
    else if( est_suivant(uniteCourante, _listeDecVariablesBis_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void declarationVariable( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante != ENTIER ) erreur(__FUNCTION__);
    readToken();
    if( uniteCourante != ID_VAR ) erreur(__FUNCTION__);
    readToken();

    optTailleTableau();

    affiche_balise_fermante(__FUNCTION__, 1);
}

void optTailleTableau( void ){

    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == CROCHET_OUVRANT ){
        readToken();
        if( uniteCourante != NOMBRE ) erreur(__FUNCTION__);
        readToken();
        if( uniteCourante != CROCHET_FERMANT ) erreur(__FUNCTION__);
        readToken();
    }

    else if( est_suivant(uniteCourante, _optTailleTableau_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void listeDecFonctions( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _declarationFonction_) ){
        declarationFonction();
        listeDecFonctions();
    }
    else if( est_suivant(uniteCourante, _listeDecFonctions_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void declarationFonction( void ){

    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante != ID_FCT ) erreur(__FUNCTION__);

    readToken();

    listeParam();
    optDecVariables();
    instructionBloc();

    affiche_balise_fermante(__FUNCTION__, 1);
}

void listeParam( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante != PARENTHESE_OUVRANTE ) erreur(__FUNCTION__);

    readToken();
    optListeDecVariables();

    if( uniteCourante != PARENTHESE_FERMANTE ) erreur(__FUNCTION__);

    readToken();

    affiche_balise_fermante(__FUNCTION__,1);
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
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void instruction( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == FAIRE ){ instructionFaire(); }
    else if( est_premier(uniteCourante, _instructionAffect_) ){ instructionAffect(); }
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

void instructionFaire( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);
    readToken();
    instructionBloc();
    if( uniteCourante != TANTQUE ) erreur(__FUNCTION__);
    readToken();
    expression();
    if( uniteCourante != POINT_VIRGULE ) erreur(__FUNCTION__);
    readToken();
    affiche_balise_fermante(__FUNCTION__, 1);
}

void instructionAffect( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);
    if( est_premier(uniteCourante, _var_) ) var();
    if( uniteCourante != EGAL ) erreur(__FUNCTION__);
    readToken();
    expression();
    if(uniteCourante != POINT_VIRGULE) erreur(__FUNCTION__);
    readToken();
    affiche_balise_fermante(__FUNCTION__, 1);
}

void instructionBloc( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if(uniteCourante != ACCOLADE_OUVRANTE) erreur(__FUNCTION__);

    readToken();
    listeInstructions();

    if(uniteCourante != ACCOLADE_FERMANTE) erreur(__FUNCTION__);

    readToken();

    affiche_balise_fermante(__FUNCTION__, 1);
}

void listeInstructions( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _instruction_) ){
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
    if( uniteCourante != SI ) erreur(__FUNCTION__);
    readToken();
    expression();

    if( uniteCourante != ALORS ) erreur(__FUNCTION__);
    readToken();
    instructionBloc();
    optSinon();

    affiche_balise_fermante(__FUNCTION__, 1);
}

void optSinon( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == SINON ){
        readToken();
        instructionBloc();
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
        readToken();
        expression();
            if( uniteCourante == FAIRE ){
                readToken();
                instructionBloc();
            }
            else erreur(__FUNCTION__);
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void instructionAppel( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _appelFct_) ){
        appelFct();
        if( uniteCourante != POINT_VIRGULE ) erreur(__FUNCTION__);
        readToken();
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void instructionRetour( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante != RETOUR ) erreur(__FUNCTION__);
    readToken();
    expression();
    if( uniteCourante != POINT_VIRGULE ) erreur(__FUNCTION__);
    readToken();
    affiche_balise_fermante(__FUNCTION__, 1);
}

void instructionEcriture( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante != ECRIRE ) erreur(__FUNCTION__);
    readToken();

    if( uniteCourante != PARENTHESE_OUVRANTE ) erreur(__FUNCTION__);
    readToken();
    expression();

    if( uniteCourante != PARENTHESE_FERMANTE ) erreur(__FUNCTION__);
    readToken();

    if( uniteCourante != POINT_VIRGULE ) erreur(__FUNCTION__);
    readToken();

    affiche_balise_fermante(__FUNCTION__, 1);
}

void instructionVide( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante != POINT_VIRGULE )
     erreur(__FUNCTION__);

    readToken();

    affiche_balise_fermante(__FUNCTION__, 1);
}

void expression( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier( uniteCourante, _conjonction_) ){
        conjonction();
        expressionBis();
    } else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void expressionBis( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == OU ){
        readToken();
        conjonction();
        expressionBis();
    }
    else if( est_suivant(uniteCourante, _expressionBis_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void conjonction( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _comparaison_) ){
        comparaison();
        conjonctionBis();
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void conjonctionBis( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == ET ){
        readToken();
        comparaison();
        conjonctionBis();
    }
    else if( est_suivant(uniteCourante, _conjonctionBis_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void comparaison( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _expArith_) ){
        expArith();
        comparaisonBis();
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void comparaisonBis( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == EGAL || uniteCourante == INFERIEUR ){
        readToken();
        expArith();
        comparaisonBis();
    }
    else if( est_suivant(uniteCourante, _comparaisonBis_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void expArith( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _terme_) ){
        terme();
        expArithBis();
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void expArithBis( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == PLUS || uniteCourante == MOINS ){
        readToken();
        terme();
        expArithBis();
    }
    else if( est_suivant(uniteCourante, _expArithBis_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    else erreur(__FUNCTION__);


    affiche_balise_fermante(__FUNCTION__, 1);
}

void terme( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _negation_) ){
        negation();
        termeBis();
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void termeBis( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == FOIS || uniteCourante == DIVISE ){
        readToken();
        negation();
        termeBis();
    }
    else if( est_suivant(uniteCourante, _termeBis_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void negation( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == NON ){
        readToken();
        negation();
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
        readToken();
        expression();
        if( uniteCourante != PARENTHESE_FERMANTE ) erreur(__FUNCTION__);
        readToken();
    }
    else if( uniteCourante == NOMBRE ){
        readToken();
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    else if( uniteCourante == LIRE ){
        readToken();
        if( uniteCourante != PARENTHESE_OUVRANTE ) erreur(__FUNCTION__);
        readToken();
        if( uniteCourante != PARENTHESE_FERMANTE ) erreur(__FUNCTION__);
        readToken();
    }
    else if( est_premier(uniteCourante, _var_) ) var();
    else if( est_premier(uniteCourante, _appelFct_) ) appelFct();
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void var( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante != ID_VAR ) erreur(__FUNCTION__);
    readToken();
    optIndice();

    affiche_balise_fermante(__FUNCTION__, 1);
}

void optIndice( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == CROCHET_OUVRANT ){
        readToken();
        expression();

        if( uniteCourante != CROCHET_FERMANT ) erreur(__FUNCTION__);

        readToken();
    }
    else if( est_suivant(uniteCourante, _optIndice_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void appelFct( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( uniteCourante == ID_FCT ){
        readToken();
        if( uniteCourante == PARENTHESE_OUVRANTE ){
            readToken();
            listeExpressions();
            if( uniteCourante != PARENTHESE_FERMANTE ) erreur(__FUNCTION__);
            readToken();
        } else erreur(__FUNCTION__);
    }
    else if( est_suivant(uniteCourante, _appelFct_) ){
        affiche_balise_fermante(__FUNCTION__, 1);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, 1);
}

void listeExpressions( void ){
    affiche_balise_ouvrante(__FUNCTION__, 1);

    if( est_premier(uniteCourante, _expression_) ){
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
        readToken();
        expression();
        listeExpressionsBis();
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
