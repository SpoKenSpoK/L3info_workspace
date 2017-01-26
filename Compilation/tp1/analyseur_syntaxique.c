#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "symboles.h"
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "util.h"


int uniteCourante;

void pError(char * car){
    printf("Erreur : %s", car);
    exit(-1);
}

void E0(void){
    E1();
    E0_();
}

void E0_(void){
    if(uniteCourante == OU){
        uniteCourante = yylex();
        E0();
    }
    else return;
}

void E1(void){
    E2();
    E1_();
}

void E1_(void){
    if(uniteCourante == ET){
        uniteCourante = yylex();
        E1();
    }
    else return;
}

void E2(void){
    if(uniteCourante == NON){
        uniteCourante = yylex();
        E2();
    }
    else E3();
}

void E3(void){
    E4();
    E3_();
}

void E3_(void){
    if(uniteCourante == EGAL || uniteCourante == INFERIEUR){
        uniteCourante = yylex();
        E3();
    }
    else return;
}

void E4(void){
    E5();
    E4_();
}

void E4_(void){
    if(uniteCourante == PLUS || uniteCourante == MOINS ){
        uniteCourante = yylex();
        E4();
    }
    else return;
}

void E5(void){
    E6();
    E5_();
}

void E5_(void){
    if(uniteCourante == FOIS || uniteCourante == DIVISE){
        uniteCourante = yylex();
        E5();
    }
    else return;
}

void E6(void){
    if(uniteCourante == PARENTHESE_OUVRANTE){
        uniteCourante = yylex();
        E0();
        if(uniteCourante == PARENTHESE_FERMANTE){
            uniteCourante = yylex();
        }
    }
    else if(uniteCourante == ID_VAR) return;
    else if(uniteCourante == NOMBRE) return;
    else if(uniteCourante == ID_FCT) return;
    else pError("E6");
}

void test_syntaxique(FILE *yyin) {
    uniteCourante = yylex();
    E0();
    printf("correct\n");
}
