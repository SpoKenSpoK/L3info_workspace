#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "generationX86.h"



/*
    Initialisation de variables nécessaires pour I/O
*/
void X86_INIT(){
    outPUT_filename = "test.asm";
    outPUT = fopen(outPUT_filename, "w+");

    // Booléens permettant de savoir si une telle seciton a déjà été parcourue
    sectionDATA = 0;
    sectionBSS = 0;
    sectionTEXT = 0;
}

// Fermeture du nouveau fichier .asm
void X86_CLOSE(){
    if( outPUT ){ fclose(outPUT); }
}

// Ecriture dans le stream outPUT
void IWriteFile(char* content) {
    fseek(outPUT, 0, SEEK_END);
    fputs(content, outPUT);
}

void IWriteFileFrom(char* content, int position){
    fseek(outPUT, position, SEEK_SET);
    fputs(content, outPUT);
}

// Open & ReadFile
// @param le nom du fichier à parser
// @return le contenu du fichier sous forme de châine de caractères
char* OReadFile(char* _filename){
    char* str = 0;
    long nbChar;
    FILE* file = fopen(_filename,"r");

    if( file ){
        fseek(file, 0, SEEK_END); // recherche de la fin du fichier
        nbChar = ftell(file);     // position du curseur (situé ici à la fin du fichier) = nombre de caractère dans le fichier (taille fichier)
        fseek(file, 0, SEEK_SET); // position begin
        str = malloc(nbChar);
        if( str ) fread(str, 1, nbChar, file); // lecture >> str
        fclose(file);
    }
    else
        erreur_1s("Le fichier %s n'a pas pu être ouvert.", _filename);

    return str;
}

// Ecriture dans le stream outPUT du contenu du fichier _filename
void defaultRoute(char* _filename){
    IWriteFile( OReadFile(_filename) );
}

// primaire
void X86_include() { defaultRoute("ASM_DEFAUT/include.asm"); }
void X86_data() {
    if( sectionDATA == 0 ){
        defaultRoute("ASM_DEFAUT/section_data.asm");
        sectionDATA = 1;
    }

    // suite
}

void X86_bss(char* name, char* bytes, int mult){
    char content[2048];
    sprintf(content, "%s: %s %d\n", name, bytes, mult);
    IWriteFile(content);
}

void X86_text(){ defaultRoute("ASM_DEFAUT/section_text.asm"); }
void X86_start(){ defaultRoute("ASM_DEFAUT/_start.asm"); }
void X86_main(){
    IWriteFile("\nmain:\n");

    // code

    IWriteFile("    ret\n");
}

// INSTRUCTION
void X86_instrWrite(){
    IWriteFile("    int 0x80\n");
}

void X86_instrAffect(){


}
