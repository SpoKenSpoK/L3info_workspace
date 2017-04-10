#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "symboles.h"
#include "premiers.h"
#include "suivants.h"
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "syntabs.h"
#include "affiche_arbre_abstrait.h"
#include "tabsymboles.h"
#include "generationX86.h"

int uniteCourante;
char buff[1024];
char val[1024];
int showXML = 0;
int showTAB_SYMB = 0;

int indexExp = -1;
char actualExp[1024];
char adrAlloc[1024];

int nbArg_Dec = 0;
int nbArg_App = 0;
int nbArg_idFonction = 0;

int nbDecVar = 0;

int portee = P_VARIABLE_GLOBALE;
int adresseLocaleCourante;
int adresseArgumentCourant;
int adresseGlobaleCourante;

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
        //X86_include();

        IWriteFile("%include 'io.asm'\n\n");

        if( sectionBSS == 0 ){

            IWriteFile("section .bss\nsinput: resb 255	;reserve a 255 byte space in memory for the users input string\n");
            //defaultRoute("../ASM_DEFAUT/section_bss.asm");
            sectionBSS = 1;
        }
        _nldecOne = optDecVariables();
        //X86_text();
        //X86_start();

        IWriteFile("\nsection .text\n");
        IWriteFile("global _start\n_start:\n    call main\n    mov  eax, 1                              ; 1 est le code de SYS_EXIT\n    int 0x80\n");


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
        _nldecTwo = herite;
    }
    else erreur(__FUNCTION__);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _nldecTwo;
}

n_dec* declarationVariable( void ){
    int tailleTAB = 0;
    char* varName;
    n_dec* _decOne;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante != ENTIER ) erreur(__FUNCTION__);
    readToken();
    if( uniteCourante != ID_VAR ) erreur(__FUNCTION__);
    readToken();

    varName = duplique_chaine(val);
    if( rechercheDeclarative(varName) >= 0 ){
        erreur_1s("La variable %s est déjà déclarée.", varName);
    }

    tailleTAB = optTailleTableau();

    if ( tailleTAB != 0 ){
        _decOne = cree_n_dec_tab(varName, tailleTAB);
        ajouteIdentificateur(varName, P_VARIABLE_GLOBALE, T_TABLEAU_ENTIER, adresseGlobaleCourante, tailleTAB);
        adresseGlobaleCourante+=( 4 * tailleTAB );
        X86_bss(varName, "rest", tailleTAB);
    }
    else {
        _decOne = cree_n_dec_var(varName);

        if( portee == P_VARIABLE_GLOBALE ){
            ajouteIdentificateur(varName, P_VARIABLE_GLOBALE, T_ENTIER, adresseGlobaleCourante, 1);
            adresseGlobaleCourante+=4;
            X86_bss(varName, "rest", 1);
        }
        else if( portee == P_VARIABLE_LOCALE ){
            ajouteIdentificateur(varName, P_VARIABLE_LOCALE, T_ENTIER, adresseLocaleCourante, 1);
            adresseLocaleCourante+=4;
            nbDecVar++;
        }
        else if( portee == P_ARGUMENT ){
            ajouteIdentificateur(varName, P_ARGUMENT, T_ENTIER, adresseArgumentCourant, 1);
            adresseArgumentCourant+=4;
            ++nbArg_Dec;
        }
    }

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _decOne;
}

int optTailleTableau( void ){
    int tailleTAB;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == CROCHET_OUVRANT ){
        readToken();
        if( uniteCourante != NOMBRE ) erreur(__FUNCTION__);
        readToken();
        tailleTAB = atoi(val);
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

    //variable globale remise à 0
    nbDecVar = 0;
    nbArg_Dec = 0;
    foncName = duplique_chaine(val);

    // On recherche si la fonction n'est pas déjà déclaréer auparavant
    if( rechercheDeclarative(foncName) >= 0 ){ erreur_1s("La fonction %s est déjà déclarée", foncName); }
    if( rechercheExecutable(foncName) >= 0 ){ erreur_1s("La fonction %s est déjà déclarée", foncName); }

    // Ajout de l'identificateur fonction
    int foncID = ajouteIdentificateur(foncName, P_VARIABLE_GLOBALE, T_FONCTION, 0, 0);
    entreeFonction();

    // Ecriture X86 nom fonction
    IWriteFile("\n");
    IWriteFile(foncName);
    IWriteFile(":\n");
    // DEPLACEMENT DU BASE POINTEUR
    IWriteFile("    push ebp            ; sauvegarde la valeur de ebp\n");
    IWriteFile("    mov  ebp, esp            ; nouvelle valeur de ebp\n");

    _nldecOne = listeParam();
    // Ajout du nombre d'argument à l'identificateur fonction
    tabsymboles.tab[foncID].complement = nbArg_Dec;
    nbArg_idFonction = nbArg_Dec;

    // TODO -> A NE PAS FAIRE

    _nldecTwo = optDecVariables();

    if( nbDecVar > 0 ){
        IWriteFile("    sub  esp, ");
        sprintf(adrAlloc, "%d   ; allocation variable locale\n", nbDecVar*4);
        IWriteFile(adrAlloc);
    }

    _instrOne = instructionBloc();

    if( nbDecVar > 0 ){
        IWriteFile("    add  esp, ");
        sprintf(adrAlloc, "%d   ; desallocation variable locale\n", nbDecVar*4);
        IWriteFile(adrAlloc);
    }

    // Retour
    IWriteFile("    pop  ebp             ; restaure la valeur de ebp\n"); // RESTAURATION DE LA VALEUR EBP
    IWriteFile("    ret\n");

    if( showTAB_SYMB == 1) afficheTabsymboles();
    sortieFonction();

    affiche_balise_fermante(__FUNCTION__, showXML);
    return cree_n_dec_fonc(foncName, _nldecOne, _nldecTwo, _instrOne);
}

n_l_dec* listeParam( void ){
    n_l_dec* _nldecOne;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante != PARENTHESE_OUVRANTE ) erreur(__FUNCTION__);

    portee = P_ARGUMENT;

    readToken();
    _nldecOne = optListeDecVariables();

    if( uniteCourante != PARENTHESE_FERMANTE ) erreur(__FUNCTION__);

    portee = P_VARIABLE_LOCALE;
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



    // X86_instrAffect VARIABLE | à faire TODO*
    // $a = ebx (ebx <- registre )

    int index = rechercheExecutable(_variable->nom);
    int portee = tabsymboles.tab[index].portee;
    int adr = tabsymboles.tab[index].adresse;

    switch( portee ){
        // GLOBAL
        case 1:
            IWriteFile("    pop  ebx\n");
            IWriteFile("    mov  [");
            IWriteFile(_variable->nom);
            IWriteFile("], ebx   ; global\n");
        break;

        // LOCAL
        case 2:
            IWriteFile("    pop  ebx\n");
            IWriteFile("    mov  [ebp - ");
            sprintf(adrAlloc,"%d], ebx  ; local\n", (- 4 - (adr)) * -1);
            IWriteFile(adrAlloc);
        break;

        //PARAMETRES
        case 3:
            IWriteFile("    pop  ebx\n");
            IWriteFile("    mov  [ebp + ");
            sprintf(adrAlloc,"%d], ebx\n    ; parametre", 4 + (4*nbArg_idFonction) - (adr));
            IWriteFile(adrAlloc);
        break;

    }

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

    //int adrSinon = indexExp + 1;
    int adrRetour = indexExp + 2;
    indexExp+=2;
    _expOne = expression();

    if( uniteCourante != ALORS ) erreur(__FUNCTION__);
    readToken();

    _instrOne = instructionBloc();

    sprintf(actualExp, "    jmp  e%d\n", adrRetour);
    IWriteFile(actualExp);

    _instrTwo = optSinon();


    _instrThree = cree_n_instr_si(_expOne, _instrOne, _instrTwo);

    // ENTREE DANS LE TANT QUE
    sprintf(actualExp, "e%d:\n", adrRetour);
    IWriteFile(actualExp);

    affiche_balise_fermante(__FUNCTION__, showXML);

    return _instrThree;
}

n_instr* optSinon( void ){
    n_instr* _instrOne;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == SINON ){
        sprintf(actualExp, "e%d:\n", indexExp-3);
        IWriteFile(actualExp);

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

        // ENTREE DANS LE TANT QUE
        sprintf(actualExp, "e%d:\n", ++indexExp);
        IWriteFile(actualExp);
        // A utiliser pour la valeur de l'expression retour
        int indexRetourTantQue = ++indexExp;

        _expOne = expression();
        if( uniteCourante == FAIRE ){
            readToken();

            _instrBloc = instructionBloc();

            sprintf(actualExp, "    jmp  e%d\n", indexRetourTantQue-1);
            IWriteFile(actualExp);

            sprintf(actualExp, "e%d:\n", indexRetourTantQue);
            IWriteFile(actualExp);
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

    IWriteFile("    pop  eax\n");
    IWriteFile("    mov  [ebp + ");
    sprintf(adrAlloc,"%d], eax\n", 8 + (4*nbArg_idFonction) );
    IWriteFile(adrAlloc);
    IWriteFile("    pop  ebp\n");
    IWriteFile("    ret\n");

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

    IWriteFile("    pop	 eax\n");
    IWriteFile("    call iprintLF           ; écrire + \\n\"\n");

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

        //TODO INFERIEUR
        IWriteFile("    pop  ebx     ; depile la seconde operande dans ebx\n");
        IWriteFile("    pop	 eax		; depile la permière operande dans eax\n");
        IWriteFile("    cmp	 eax, ebx\n");

        // SI BIEN EAX == EBX :
        IWriteFile("    je   ");
        sprintf(actualExp, "e%d\n", ++indexExp);
        IWriteFile(actualExp);

        // SINON
        IWriteFile("    push 0\n");
        IWriteFile("    jmp	 ");
        sprintf(actualExp, "e%d\n", ++indexExp);
        IWriteFile(actualExp);

        sprintf(actualExp, "e%d:\n", --indexExp);
        IWriteFile(actualExp);
        IWriteFile("    push 1\n");

        sprintf(actualExp, "e%d:\n", ++indexExp);
        IWriteFile(actualExp);

        IWriteFile("    pop  eax\n");
        IWriteFile("    cmp	 eax, 00\n");
        IWriteFile("    jz	 ");

        sprintf(actualExp, "e%d\n", indexExp-3);
        IWriteFile(actualExp);

        _expThree = comparaisonBis(_expTwo);
    }
    else if( uniteCourante == INFERIEUR ){
        readToken();
        _expOne = expArith();
        _expTwo = cree_n_exp_op(inf, herite, _expOne);

        //TODO INFERIEUR
        IWriteFile("    pop  ebx     ; depile la seconde operande dans ebx\n");
        IWriteFile("    pop	 eax		; depile la permière operande dans eax\n");
        IWriteFile("    cmp	 eax, ebx\n");

        // SI BIEN EAX < EBX :
        IWriteFile("    jl   ");
        sprintf(actualExp, "e%d\n", ++indexExp);
        IWriteFile(actualExp);

        // SINON
        IWriteFile("    push 0\n");
        IWriteFile("    jmp	 ");
        sprintf(actualExp, "e%d\n", ++indexExp);
        IWriteFile(actualExp);

        sprintf(actualExp, "e%d:\n", --indexExp);
        IWriteFile(actualExp);
        IWriteFile("    push 1\n");

        sprintf(actualExp, "e%d:\n", ++indexExp);
        IWriteFile(actualExp);

        IWriteFile("    pop  eax\n");
        IWriteFile("    cmp	 eax, 00\n");
        IWriteFile("    jz	 ");

        sprintf(actualExp, "e%d\n", indexExp-2);
        IWriteFile(actualExp);

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

        // TODO ADDITION
        IWriteFile("    pop  ebx		 ; depile la seconde operande dans ebx\n");
        IWriteFile("    pop  eax		 ; depile la permière operande dans eax\n");
        IWriteFile("    add  eax, ebx	 ; effectue l'opération\n");
        IWriteFile("    push eax		 ; empile le résultat\n");

        _expThree = expArithBis(_expTwo);
    }
    else if( uniteCourante == MOINS ){
        readToken();
        _expOne = terme();
        _expTwo = cree_n_exp_op(moins, herite, _expOne);

        // TODO SOUSTRACTION
        IWriteFile("    pop  ebx		 ; depile la seconde operande dans ebx\n");
        IWriteFile("    pop  eax		 ; depile la permière operande dans eax\n");
        IWriteFile("    sub  eax, ebx	 ; effectue l'opération\n");
        IWriteFile("    push eax		 ; empile le résultat\n");

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

        // TODO MULTIPLICATION
        IWriteFile("    pop	 ebx		 ; depile la seconde operande dans ebx\n");
        IWriteFile("    pop	 eax		 ; depile la permière operande dans eax\n");
        IWriteFile("    mul	 ebx		 ; effectue l'opération\n");
        IWriteFile("    push eax		 ; empile le résultat\n");

        _expThree = termeBis(_expTwo);
    }
    else if( uniteCourante == DIVISE ){
        readToken();
        _expOne = negation();
        _expTwo = cree_n_exp_op(divise, herite, _expOne);

        // TODO Division
        IWriteFile("    pop	 ebx		 ; depile la seconde operande dans ebx\n");
        IWriteFile("    pop	 eax		 ; depile la permière operande dans eax\n");
        IWriteFile("    div	 ebx		 ; effectue l'opération\n");
        IWriteFile("    push eax		 ; empile le résultat\n");

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

        // TODO FONCTION AFFECT EGAL
        IWriteFile("    push ");
        IWriteFile(val);
        IWriteFile("\n");
    }
    else if( uniteCourante == LIRE ){
        readToken();
        if( uniteCourante != PARENTHESE_OUVRANTE ) erreur(__FUNCTION__);
        readToken();
        if( uniteCourante != PARENTHESE_FERMANTE ) erreur(__FUNCTION__);
        readToken();

        // TODO FONCTION AFFECT LIRE
        IWriteFile("    mov	 ecx, sinput\n");
    	IWriteFile("    mov	 edx, 255\n");
    	IWriteFile("    mov	 eax, 3		 ; 3 est le code de SYS_READ\n");
    	IWriteFile("    mov	 ebx, 0		 ; 0 est le code de STDIN\n");
    	IWriteFile("    int	 80h\n");
    	IWriteFile("    mov	 eax, sinput\n");
    	IWriteFile("    call atoi\n");
        IWriteFile("    push eax\n");

        _expOne = cree_n_exp_lire();
    }
    else if( est_premier(uniteCourante, _var_) ){
        _variable = var();
        _expOne = cree_n_exp_var(_variable);

        // TODO

        int index = rechercheExecutable(_variable->nom);
        int portee = tabsymboles.tab[index].portee;
        int adr = tabsymboles.tab[index].adresse;

        switch( portee ){
            // GLOBAL
            case 1:
                IWriteFile("    mov  ebx, [");
                IWriteFile(_variable->nom);
                IWriteFile("]\n");
                IWriteFile("    push ebx    ; global\n");
            break;

            // LOCAL
            case 2:
                IWriteFile("    mov  ebx, [ebp - ");
                sprintf(adrAlloc,"%d] ; lit variable dans ebx\n", (- 4 - (adr)) * -1);
                IWriteFile(adrAlloc);
                IWriteFile("    push ebx    ; local\n");

            break;

            //PARAMETRES
            case 3:
                IWriteFile("    mov  ebx, [ebp + ");
                sprintf(adrAlloc,"%d] ; lit variable dans ebx\n", 4 + (4*nbArg_idFonction) - (adr));
                IWriteFile(adrAlloc);
                IWriteFile("    push ebx    ; parametre\n");
            break;
        }

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

    int IDvar_ = rechercheExecutable(varName);
    if( IDvar_ >= 0 ){
        if( tabsymboles.tab[IDvar_].type == T_TABLEAU_ENTIER && _expOne == NULL )
            erreur_1s("Le tableau %s ne peut être utilisé sans indice", varName);

        if( tabsymboles.tab[IDvar_].type == T_ENTIER && _expOne != NULL )
            erreur_1s("La variable %s n'est pas un tableau (aucune possibilité de transtypage)", varName);
    }else{
        erreur_1s("La variable %s est non déclarée auparavant", varName);
    }

    affiche_balise_fermante(__FUNCTION__, showXML);

    if( _expOne != NULL ){
        _varOne = cree_n_var_indicee(varName, _expOne);
    }
    else{
        _varOne = cree_n_var_simple(varName);
    }

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
    int IDfonc_;

    nbArg_App = 0;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( uniteCourante == ID_FCT ){
        readToken();
        foncName = duplique_chaine(val);
        IDfonc_ = rechercheExecutable(foncName);

        nbArg_idFonction = tabsymboles.tab[IDfonc_].complement;
        IWriteFile("    sub esp, 4  ; allocation valeur de retour\n");

        if( uniteCourante == PARENTHESE_OUVRANTE ){
            readToken();
             _nlexpOne = listeExpressions();

             IWriteFile("    call ");
             IWriteFile(foncName);
             IWriteFile("\n");

             IWriteFile("    add  esp, ");
             sprintf(adrAlloc, "%d  ;desallocation parametre\n", nbArg_idFonction*4);
             IWriteFile(adrAlloc);

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

    if( IDfonc_ >= 0 ){
        if( nbArg_App != tabsymboles.tab[IDfonc_].complement )
            erreur_1s("Nombre d'arguments dans la fonction %s incorrect", foncName);
    }else{
        erreur_1s("La fonction %s est non déclarée auparavant", foncName);
    }

    _appelOne = cree_n_appel(foncName, _nlexpOne);
    return _appelOne;
}

n_l_exp* listeExpressions( void ){
    n_exp* _expOne;
    n_l_exp* _nlexpOne;

    affiche_balise_ouvrante(__FUNCTION__, showXML);

    if( est_premier(uniteCourante, _expression_) ){
        ++nbArg_App;
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
        ++nbArg_App;
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

void test_syntaxique(FILE *yyin, int showSyntaxique, int showAbstrait, int _showTAB_SYMB) {
    showXML = showSyntaxique;

    initialise_suivants();
    initialise_premiers();
    uniteCourante = yylex();

    showTAB_SYMB = _showTAB_SYMB;

    X86_INIT();
    n_prog* prog = programme();
    X86_CLOSE();

    if( showXML == 0 && showAbstrait == 1 && showTAB_SYMB == 0)
        affiche_n_prog(prog);
}
