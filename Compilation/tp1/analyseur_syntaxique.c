#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "symboles.h"
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "util.h"


/**

E -> T E'
E' -> + E | e
T -> F T'
T' -> x F | e
F -> ( E ) | nombre

**/

void pError(char* car){
    printf(" Erreur de syntaxe %s\n", car);
    exit(-1);
}

int uniteCourante;

void E_zero(){
    T_zero();
    E_one();
}

void E_one(){
    if(uniteCourante == PLUS){
        uniteCourante = yylex();
        E_zero();
        return;
    }
}

void T_zero(){
    F_zero();
    T_one();
}

void T_one(){
    if(uniteCourante == FOIS){
        uniteCourante = yylex();
        F_zero();
        return;
    }
}

void F_zero(){
    if(uniteCourante == PARENTHESE_OUVRANTE){
        uniteCourante = yylex();
        E_zero();
        if(uniteCourante == PARENTHESE_FERMANTE){
            return;
        }
    }
    else if(uniteCourante == NOMBRE){
        uniteCourante = yylex();
        return;
    }
    else pError("F_zero");
}

void test_syntaxique(FILE *yyin) {
    uniteCourante = yylex();
    E_zero();
    printf("correct\n");
}
