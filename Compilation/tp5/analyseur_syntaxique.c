#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "symboles.h"
#include "premiers.h"
#include "suivants.h"
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "util.h"
#include "syntabs.h"
#include "affiche_arbre_abstrait.h"

int uniteCourante;
char buff[1024];
char val[1024];
int showXML = 1;

void readToken(){
    nom_token(uniteCourante, buff, val);
    affiche_element(buff, val, showXML);
    uniteCourante = yylex();
}

void programme( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _optDecVariables_) || est_premier(uniteCourante, _listeDecFonctions_) ){
        optDecVariables();
        listeDecFonctions();
    }
    else if( est_suivant(uniteCourante, _programme_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);
}

void optDecVariables( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _listeDecVariables_) ){
        listeDecVariables();
        if( uniteCourante != POINT_VIRGULE ) erreur(__FUNCTION__);
        readToken();
    }
    else if( est_suivant(uniteCourante, _optDecVariables_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);
}


void listeDecVariables( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _declarationVariable_) ){
        declarationVariable();
        listeDecVariablesBis();
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);
}

void listeDecVariablesBis( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == VIRGULE ){
        readToken();
        declarationVariable();
        listeDecVariablesBis();
    }
    else if( est_suivant(uniteCourante, _listeDecVariablesBis_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);
}

void declarationVariable( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante != ENTIER ) erreur(__FUNCTION__);
    readToken();
    if( uniteCourante != ID_VAR ) erreur(__FUNCTION__);
    readToken();

    optTailleTableau();

    affiche_balise_fermante(__FUNCTION__, showXML);
}

void optTailleTableau( void ){

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == CROCHET_OUVRANT ){
        readToken();
        if( uniteCourante != NOMBRE ) erreur(__FUNCTION__);
        readToken();
        if( uniteCourante != CROCHET_FERMANT ) erreur(__FUNCTION__);
        readToken();
    }

    else if( est_suivant(uniteCourante, _optTailleTableau_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);
}

void listeDecFonctions( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _declarationFonction_) ){
        declarationFonction();
        listeDecFonctions();
    }
    else if( est_suivant(uniteCourante, _listeDecFonctions_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);
}

void declarationFonction( void ){

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante != ID_FCT ) erreur(__FUNCTION__);

    readToken();

    listeParam();
    optDecVariables();
    instructionBloc();

    affiche_balise_fermante(__FUNCTION__, showXML);
}

void listeParam( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante != PARENTHESE_OUVRANTE ) erreur(__FUNCTION__);

    readToken();
    optListeDecVariables();

    if( uniteCourante != PARENTHESE_FERMANTE ) erreur(__FUNCTION__);

    readToken();

    affiche_balise_fermante(__FUNCTION__,showXML);
}

void optListeDecVariables( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _listeDecVariables_) ){
        listeDecVariables();
    }
    else if( est_suivant(uniteCourante, _optListeDecVariables_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);
}

void instruction( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

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

    affiche_balise_fermante(__FUNCTION__, showXML);
}

n_instr* instructionFaire( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);
    readToken();

    n_instr* _instrOne = instructionBloc();

    if( uniteCourante != TANTQUE ) erreur(__FUNCTION__);
    readToken();

    n_exp* _expOne = expression();

    if( uniteCourante != POINT_VIRGULE ) erreur(__FUNCTION__);
    readToken();
    affiche_balise_fermante(__FUNCTION__, showXML);

    return cree_n_instr_faire(_instrOne, _expOne);
}

n_instr* instructionAffect( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);
    if( est_premier(uniteCourante, _var_) )
        n_var* _variable = var();

    if( uniteCourante != EGAL ) erreur(__FUNCTION__);
    readToken();

    n_exp* _expOne = expression();

    if(uniteCourante != POINT_VIRGULE) erreur(__FUNCTION__);
    readToken();
    affiche_balise_fermante(__FUNCTION__, showXML);

    return cree_n_instr_affect(_variable, _expOne);
}

n_instr* instructionBloc( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if(uniteCourante != ACCOLADE_OUVRANTE) erreur(__FUNCTION__);

    readToken();
    n_l_instr* _listeInstr = listeInstructions();

    if(uniteCourante != ACCOLADE_FERMANTE) erreur(__FUNCTION__);

    readToken();

    affiche_balise_fermante(__FUNCTION__, showXML);

    return cree_n_instr_bloc(_listeInstr);
}

n_l_instr* listeInstructions( void ){
    n_instr* _instr;
    n_l_instr* _listeInstr;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _instruction_) ){
        _instr = instruction();
        _listeInstr = listeInstructions();
    }
    else if( est_suivant(uniteCourante, _listeInstructions_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return NULL;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return cree_n_l_instr(_instr, _listeInstr);
}

n_instr* instructionSi( void ){
    n_instr* _instrOne;
    n_instr* _instTwo;
    n_instr* _instThree;
    n_exp* _expOne;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante != SI ) erreur(__FUNCTION__);
    readToken();
    _expOne = expression();

    if( uniteCourante != ALORS ) erreur(__FUNCTION__);
    readToken();

    _instrOne = instructionBloc();
    _instrTwo = optSinon();
    _instrThree = cree_n_instr_si(_expOne, _instrOne, _instrTwo);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _instrThree;
}

void optSinon( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == SINON ){
        readToken();
        instructionBloc();
    }
    else if( est_suivant( uniteCourante, _optSinon_ )){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);
}

n_instr* instructionTantque( void ){
    n_exp* _expOne;
    n_instr* _instrBloc;


    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == TANTQUE ){
        readToken();
        _expOne = expression();
            if( uniteCourante == FAIRE ){
                readToken();
                _instrBloc = instructionBloc();
            }
            else erreur(__FUNCTION__);
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return cree_n_instr_tantque(_expOne _instrBloc);
}

n_instr* instructionAppel( void ){
    n_appel* _appelFonction;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _appelFct_) ){
        _appelFonction = appelFct();
        if( uniteCourante != POINT_VIRGULE ) erreur(__FUNCTION__);
        readToken();
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return cree_n_instr_appel(_appelFonction);
}

n_instr* instructionRetour( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante != RETOUR ) erreur(__FUNCTION__);
    readToken();
    n_exp* _expOne = expression();
    if( uniteCourante != POINT_VIRGULE ) erreur(__FUNCTION__);
    readToken();
    affiche_balise_fermante(__FUNCTION__, showXML);

    return cree_n_instr_retour(_expOne);
}

void instructionEcriture( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante != ECRIRE ) erreur(__FUNCTION__);
    readToken();

    if( uniteCourante != PARENTHESE_OUVRANTE ) erreur(__FUNCTION__);
    readToken();
    expression();

    if( uniteCourante != PARENTHESE_FERMANTE ) erreur(__FUNCTION__);
    readToken();

    if( uniteCourante != POINT_VIRGULE ) erreur(__FUNCTION__);
    readToken();

    affiche_balise_fermante(__FUNCTION__, showXML);
}

void instructionVide( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante != POINT_VIRGULE )
     erreur(__FUNCTION__);

    readToken();

    affiche_balise_fermante(__FUNCTION__, showXML);
}

void expression( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier( uniteCourante, _conjonction_) ){
        conjonction();
        expressionBis();
    } else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);
}

void expressionBis( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == OU ){
        readToken();
        conjonction();
        expressionBis();
    }
    else if( est_suivant(uniteCourante, _expressionBis_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);
}

void conjonction( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _comparaison_) ){
        comparaison();
        conjonctionBis();
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);
}

void conjonctionBis( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == ET ){
        readToken();
        comparaison();
        conjonctionBis();
    }
    else if( est_suivant(uniteCourante, _conjonctionBis_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);
}

void comparaison( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _expArith_) ){
        expArith();
        comparaisonBis();
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);
}

void comparaisonBis( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == EGAL || uniteCourante == INFERIEUR ){
        readToken();
        expArith();
        comparaisonBis();
    }
    else if( est_suivant(uniteCourante, _comparaisonBis_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);
}

void expArith( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _terme_) ){
        terme();
        expArithBis();
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);
}

void expArithBis( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == PLUS || uniteCourante == MOINS ){
        readToken();
        terme();
        expArithBis();
    }
    else if( est_suivant(uniteCourante, _expArithBis_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return;
    }
    else erreur(__FUNCTION__);


    affiche_balise_fermante(__FUNCTION__, showXML);
}

void terme( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _negation_) ){
        negation();
        termeBis();
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);
}

n_exp* termeBis( n_exp* herite ){
    n_exp* _two;
    n_exp* _one;
    n_exp* herite_fils;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == FOIS ){
        readToken();
        _two = negation();
        herite_fils = cree_n_exp_op(fois, herite, _two);
        _one = termeBis(herite_fils);
    }
    else if( uniteCourante == DIVISE ){
        readToken();
        _two = negation();
        herite_fils = cree_n_exp_op(divise, herite, _two);
        _one = termeBis(herite_fils);

    }
    else if( est_suivant(uniteCourante, _termeBis_) ) _one = herite;
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _one;
}

n_exp* negation( void ){
    n_exp* _one;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == NON ){
        readToken();
        _one = negation();
    }
    else if( est_premier(uniteCourante, _facteur_) ){
        _one = facteur();
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _one;
}

n_exp* facteur( void ){
    n_exp* _one;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == PARENTHESE_OUVRANTE ){
        readToken();
        _one = expression();
        if( uniteCourante != PARENTHESE_FERMANTE ) erreur(__FUNCTION__);
        readToken();
    }
    else if( uniteCourante == NOMBRE ){
        readToken();
        //affiche_balise_fermante(__FUNCTION__, showXML);
        //return;
        _one = cree_n_exp_entier(atoi(val));
    }
    else if( uniteCourante == LIRE ){
        readToken();
        if( uniteCourante != PARENTHESE_OUVRANTE ) erreur(__FUNCTION__);
        readToken();
        if( uniteCourante != PARENTHESE_FERMANTE ) erreur(__FUNCTION__);
        readToken();

        _one = cree_n_exp_lire();
    }
    else if( est_premier(uniteCourante, _var_) ){
        n_var* _variable = var();
        _one = cree_n_exp_var(_variable);
    }
    else if( est_premier(uniteCourante, _appelFct_) ){
        n_appel* _appel = appelFct();
        _one = cree_n_exp_appel(_appel);
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _one;
}

n_var* var( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante != ID_VAR ) erreur(__FUNCTION__);
    readToken();
    ???=optIndice();

    affiche_balise_fermante(__FUNCTION__, showXML);

    return var* ??;
}


void optIndice( void ){
    n_exp* _one;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == CROCHET_OUVRANT ){
        readToken();
        _one = expression();

        if( uniteCourante != CROCHET_FERMANT ) erreur(__FUNCTION__);

        readToken();
    }
    else if( est_suivant(uniteCourante, _optIndice_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return NULL;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return ??;
}

n_appel* appelFct( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == ID_FCT ){
        readToken();
        if( uniteCourante == PARENTHESE_OUVRANTE ){
            readToken();
            n_l_exp* nl_exp = listeExpressions();

            if( uniteCourante != PARENTHESE_FERMANTE ) erreur(__FUNCTION__);
            readToken();
        } else erreur(__FUNCTION__);
    }
    else if( est_suivant(uniteCourante, _appelFct_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return NULL;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return n_appel* ???;
}

void listeExpressions( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _expression_) ){
        expression();
        listeExpressionsBis();
    }
    else if( est_suivant(uniteCourante, _listeExpressions_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);
}

void listeExpressionsBis( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == VIRGULE ){
        readToken();
        expression();
        listeExpressionsBis();
    }
    else if( est_suivant(uniteCourante, _listeExpressionsBis_)){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);
}

void test_syntaxique(FILE *yyin) {
    initialise_suivants();
    initialise_premiers();
    uniteCourante = yylex();
    programme();
}
