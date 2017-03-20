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
int showXML = 0;

void readToken(){
    nom_token(uniteCourante, buff, val);
    affiche_element(buff, val, showXML);
    uniteCourante = yylex();
}

n_prog* programme( void ){
    n_l_dec* _nldecOne;
    n_l_dec* _nldecTwo;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _optDecVariables_) || est_premier(uniteCourante, _listeDecFonctions_) ){
        _nldecOne = optDecVariables();
        _nldecTwo = listeDecFonctions();
    }
    else if( est_suivant(uniteCourante, _programme_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return NULL;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return cree_n_prog(_nldecOne, _nldecTwo);
}

n_l_dec* optDecVariables( void ){
    n_l_dec* _nldecOne;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _listeDecVariables_) ){
        _nldecOne = listeDecVariables();
        if( uniteCourante != POINT_VIRGULE ) erreur(__FUNCTION__);
        readToken();
    }
    else if( est_suivant(uniteCourante, _optDecVariables_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return NULL;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _nldecOne;
}


n_l_dec* listeDecVariables( void ){
    n_dec* _decOne;
    n_l_dec* _nldecOne;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _declarationVariable_) ){
        _decOne = declarationVariable();
        _nldecOne = listeDecVariablesBis(NULL);
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return cree_n_l_dec(_decOne, _nldecOne);
}

n_l_dec* listeDecVariablesBis( n_l_dec* herite ){
    n_dec* _decOne;
    n_l_dec* _nldecOne;
    n_l_dec* _nldecTwo;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == VIRGULE ){
        readToken();
        _decOne = declarationVariable();
        _nldecOne = cree_n_l_dec(_decOne, herite);
        _nldecTwo = listeDecVariablesBis(_nldecOne);
    }
    else if( est_suivant(uniteCourante, _listeDecVariablesBis_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return NULL;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _nldecTwo;
}

n_dec* declarationVariable( void ){
    int tailleTAB;
    char* varName;
    n_dec* _decOne;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante != ENTIER ) erreur(__FUNCTION__);
    readToken();
    if( uniteCourante != ID_VAR ) erreur(__FUNCTION__);
    readToken();
    varName = duplique_chaine(val);

    tailleTAB = optTailleTableau();

    if ( tailleTAB != 0 ){ _decOne = cree_n_dec_tab(varName, tailleTAB); }
    else { _decOne = cree_n_dec_var(varName); }

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _decOne;
}

int optTailleTableau( void ){
    int tailleTAB;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == CROCHET_OUVRANT ){
        readToken();
        if( uniteCourante != NOMBRE ) erreur(__FUNCTION__);
        tailleTAB = atoi(val);
        readToken();
        if( uniteCourante != CROCHET_FERMANT ) erreur(__FUNCTION__);
        readToken();
    }

    else if( est_suivant(uniteCourante, _optTailleTableau_) ) tailleTAB = 0;
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);
    return tailleTAB;
}

n_l_dec* listeDecFonctions( void ){
    n_dec* _decOne;
    n_l_dec* _nldecOne;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _declarationFonction_) ){
        _decOne = declarationFonction();
        _nldecOne = listeDecFonctions();
    }
    else if( est_suivant(uniteCourante, _listeDecFonctions_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return NULL;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return cree_n_l_dec(_decOne, _nldecOne);
}

n_dec* declarationFonction( void ){
    char* foncName;
    n_l_dec* _nldecOne;
    n_l_dec* _nldecTwo;
    n_instr* _instrOne;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante != ID_FCT ) erreur(__FUNCTION__);
    readToken();
    foncName = duplique_chaine(val);

    _nldecOne = listeParam();
    _nldecTwo = optDecVariables();
    _instrOne = instructionBloc();

    affiche_balise_fermante(__FUNCTION__, showXML);

    return cree_n_dec_fonc(foncName, _nldecOne, _nldecTwo, _instrOne);
}

n_l_dec* listeParam( void ){
    n_l_dec* _nldecOne;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante != PARENTHESE_OUVRANTE ) erreur(__FUNCTION__);

    readToken();
    _nldecOne = optListeDecVariables();

    if( uniteCourante != PARENTHESE_FERMANTE ) erreur(__FUNCTION__);

    readToken();

    affiche_balise_fermante(__FUNCTION__,showXML);

    return _nldecOne;
}

n_l_dec* optListeDecVariables( void ){
    n_l_dec* _nldecOne;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _listeDecVariables_) ){
        _nldecOne = listeDecVariables();
    }
    else if( est_suivant(uniteCourante, _optListeDecVariables_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return NULL;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _nldecOne;
}

n_instr* instruction( void ){
    n_instr* _instrOne;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == FAIRE ){ _instrOne = instructionFaire(); }
    else if( est_premier(uniteCourante, _instructionAffect_) ){     _instrOne = instructionAffect();    }
    else if( est_premier(uniteCourante, _instructionBloc_)){        _instrOne = instructionBloc();      }
    else if( est_premier(uniteCourante, _instructionSi_)){          _instrOne = instructionSi();        }
    else if( est_premier(uniteCourante, _instructionTantque_)){     _instrOne = instructionTantque();   }
    else if( est_premier(uniteCourante, _instructionAppel_)){       _instrOne = instructionAppel();     }
    else if( est_premier(uniteCourante, _instructionRetour_)){      _instrOne = instructionRetour();    }
    else if( est_premier(uniteCourante, _instructionEcriture_)){    _instrOne = instructionEcriture();  }
    else if( est_premier(uniteCourante, _instructionVide_)){        _instrOne = instructionVide();      }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _instrOne;
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


    n_instr* _expFinal = cree_n_instr_faire(_instrOne, _expOne);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _expFinal;
}

n_instr* instructionAffect( void ){
    n_var* _variable;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _var_) ) _variable = var();

    if( uniteCourante != EGAL ) erreur(__FUNCTION__);
    readToken();


    n_exp* _expOne = expression();

    if(uniteCourante != POINT_VIRGULE) erreur(__FUNCTION__);
    readToken();

    n_instr* _expFinal = cree_n_instr_affect(_variable, _expOne);

    affiche_balise_fermante(__FUNCTION__, showXML);
    return _expFinal;
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
    n_instr* _instrTwo;
    n_instr* _instrThree;
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

n_instr* optSinon( void ){
    n_instr* _instrOne;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == SINON ){
        readToken();
        _instrOne = instructionBloc();
    }
    else if( est_suivant( uniteCourante, _optSinon_ )){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return NULL;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _instrOne;
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
    else
        erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return cree_n_instr_tantque(_expOne, _instrBloc);
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

n_instr* instructionEcriture( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante != ECRIRE ) erreur(__FUNCTION__);
    readToken();

    if( uniteCourante != PARENTHESE_OUVRANTE ) erreur(__FUNCTION__);
    readToken();
    n_exp* _expOne = expression();

    if( uniteCourante != PARENTHESE_FERMANTE ) erreur(__FUNCTION__);
    readToken();

    if( uniteCourante != POINT_VIRGULE ) erreur(__FUNCTION__);
    readToken();

    affiche_balise_fermante(__FUNCTION__, showXML);

    return cree_n_instr_ecrire(_expOne);
}

n_instr* instructionVide( void ){
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante != POINT_VIRGULE )
     erreur(__FUNCTION__);

    readToken();

    affiche_balise_fermante(__FUNCTION__, showXML);

    return cree_n_instr_vide();
}

n_exp* expression( void ){
    n_exp* _expOne;
    n_exp* _expTwo;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier( uniteCourante, _conjonction_) ){
        _expOne = conjonction();
        _expTwo = expressionBis(_expOne);
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _expTwo;
}

n_exp* expressionBis( n_exp* herite ){
    n_exp* _expOne;
    n_exp* _expTwo;
    n_exp* _expThree;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == OU ){
        readToken();
        _expOne = conjonction();
        _expTwo = cree_n_exp_op(ou, herite, _expOne);
        _expThree = expressionBis(_expTwo);
    }
    else if( est_suivant(uniteCourante, _expressionBis_) ){
        _expThree = herite;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _expThree;
}

n_exp* conjonction( void ){
    n_exp* _expOne;
    n_exp* _expTwo;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _comparaison_) ){
        _expOne = comparaison();
        _expTwo = conjonctionBis(_expOne);
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _expTwo;
}

n_exp* conjonctionBis( n_exp* herite ){
    n_exp* _expOne;
    n_exp* _expTwo;
    n_exp* _expThree;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == ET ){
        readToken();
        _expOne = comparaison();
        _expTwo = cree_n_exp_op(et, herite, _expOne);
        _expThree = conjonctionBis(_expTwo);
    }
    else if( est_suivant(uniteCourante, _conjonctionBis_) ){
        _expThree = herite;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _expThree;
}

n_exp* comparaison( void ){
    n_exp* _expOne;
    n_exp* _expTwo;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _expArith_) ){
        _expOne = expArith();
        _expTwo = comparaisonBis(_expOne);
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _expTwo;
}

n_exp* comparaisonBis( n_exp* herite ){
    n_exp* _expOne;
    n_exp* _expTwo;
    n_exp* _expThree;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == EGAL ){
        readToken();
        _expOne = expArith();
        _expTwo = cree_n_exp_op(egal, herite, _expOne);
        _expThree = comparaisonBis(_expTwo);
    }
    else if( uniteCourante == INFERIEUR ){
        readToken();
        _expOne = expArith();
        _expTwo = cree_n_exp_op(inf, herite, _expOne);
        _expThree = comparaisonBis(_expTwo);
    }
    else if( est_suivant(uniteCourante, _comparaisonBis_) ) _expThree = herite;

    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _expThree;
}

n_exp* expArith( void ){
    n_exp* _expOne;
    n_exp* _expTwo;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _terme_) ){
        _expOne = terme();
        _expTwo = expArithBis(_expOne);
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _expTwo;
}

n_exp* expArithBis( n_exp* herite ){
    n_exp* _expOne;
    n_exp* _expTwo;
    n_exp* _expThree;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == PLUS ){
        readToken();
        _expOne = terme();
        _expTwo = cree_n_exp_op(plus, herite, _expOne);
        _expThree = expArithBis(_expTwo);
    }
    else if( uniteCourante == MOINS ){
        readToken();
        _expOne = terme();
        _expTwo = cree_n_exp_op(moins, herite, _expOne);
        _expThree = expArithBis(_expTwo);
    }
    else if( est_suivant(uniteCourante, _expArithBis_) ) _expThree = herite;
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _expThree;
}

n_exp* terme( void ){
    n_exp* _expOne;
    n_exp* _expTwo;
    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _negation_) ){
        _expOne = negation();
        _expTwo = termeBis(_expOne);
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _expTwo;
}

n_exp* termeBis( n_exp* herite ){
    n_exp* _expOne;
    n_exp* _expTwo;
    n_exp* _expThree;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == FOIS ){
        readToken();
        _expOne = negation();
        _expTwo = cree_n_exp_op(fois, herite, _expOne);
        _expThree = termeBis(_expTwo);
    }
    else if( uniteCourante == DIVISE ){
        readToken();
        _expOne = negation();
        _expTwo = cree_n_exp_op(divise, herite, _expOne);
        _expThree = termeBis(_expTwo);

    }
    else if( est_suivant(uniteCourante, _termeBis_) ) _expThree = herite;
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _expThree;
}

n_exp* negation( void ){
    n_exp* _expOne;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == NON ){
        readToken();
        _expOne = negation();
    }
    else if( est_premier(uniteCourante, _facteur_) ) _expOne = facteur();
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _expOne;
}

n_exp* facteur( void ){
    n_exp* _expOne;
    n_var* _variable;
    n_appel* _appel;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == PARENTHESE_OUVRANTE ){
        readToken();
        _expOne = expression();
        if( uniteCourante != PARENTHESE_FERMANTE ) erreur(__FUNCTION__);
        readToken();
    }
    else if( uniteCourante == NOMBRE ){
        readToken();
        _expOne = cree_n_exp_entier(atoi(val));
    }
    else if( uniteCourante == LIRE ){
        readToken();
        if( uniteCourante != PARENTHESE_OUVRANTE ) erreur(__FUNCTION__);
        readToken();
        if( uniteCourante != PARENTHESE_FERMANTE ) erreur(__FUNCTION__);
        readToken();

        _expOne = cree_n_exp_lire();
    }
    else if( est_premier(uniteCourante, _var_) ){
        _variable = var();
        _expOne = cree_n_exp_var(_variable);
    }
    else if( est_premier(uniteCourante, _appelFct_) ){
        _appel = appelFct();
        _expOne = cree_n_exp_appel(_appel);
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _expOne;
}

n_var* var( void ){
    n_var* _varOne;
    n_exp* _expOne;
    char* varName;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante != ID_VAR ) erreur(__FUNCTION__);
    readToken();
    varName = duplique_chaine(val);
    _expOne = optIndice();

    affiche_balise_fermante(__FUNCTION__, showXML);

    if( _expOne != NULL )   _varOne = cree_n_var_indicee(varName, _expOne);
    else                    _varOne = cree_n_var_simple(varName);

    return _varOne;
}


n_exp* optIndice( void ){
    n_exp* _expOne;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == CROCHET_OUVRANT ){
        readToken();
        _expOne = expression();
        if( uniteCourante != CROCHET_FERMANT ) erreur(__FUNCTION__);
        readToken();
    }
    else if( est_suivant(uniteCourante, _optIndice_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return NULL;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _expOne;
}

n_appel* appelFct( void ){
    n_l_exp* _nlexpOne;
    n_appel* _appelOne;
    char* foncName;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == ID_FCT ){
        readToken();
        foncName = duplique_chaine(val);
        if( uniteCourante == PARENTHESE_OUVRANTE ){
            readToken();
             _nlexpOne = listeExpressions();
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

    _appelOne = cree_n_appel(foncName, _nlexpOne);
    return _appelOne;
}

n_l_exp* listeExpressions( void ){
    n_exp* _expOne;
    n_l_exp* _nlexpOne;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _expression_) ){
        _expOne = expression();
        _nlexpOne = listeExpressionsBis( NULL );
    }
    else if( est_suivant(uniteCourante, _listeExpressions_) ){
        affiche_balise_fermante(__FUNCTION__, showXML);
        return NULL;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return cree_n_l_exp(_expOne, _nlexpOne);
}

n_l_exp* listeExpressionsBis( n_l_exp* herite ){
    n_exp* _expOne;
    n_l_exp* _nlexpOne;
    n_l_exp* _nlexpTwo;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == VIRGULE ){
        readToken();
        _expOne = expression();
        _nlexpOne = cree_n_l_exp(_expOne, herite);
        _nlexpTwo = listeExpressionsBis(_nlexpOne);
    }
    else if( est_suivant(uniteCourante, _listeExpressionsBis_)) _nlexpTwo = herite;
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _nlexpTwo;
}

void test_syntaxique(FILE *yyin, int showSyntaxique, int showAbstrait) {
    showXML = showSyntaxique;

    initialise_suivants();
    initialise_premiers();
    uniteCourante = yylex();

    n_prog* prog = programme();

    if( showXML == 0 && showAbstrait == 1 )
        affiche_n_prog(prog);
}
