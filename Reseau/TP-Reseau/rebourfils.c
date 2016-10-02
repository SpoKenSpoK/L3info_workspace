/** HECKEL Thibault - TP1 Réseau & Com. UNIX - L3 Informatique site de Luminy | Septembre 2016
* Partie II - 1) 2) 3)
*
* Note pour le correcteur :
* Une très bonne partie de la compréhension de l'utilisation du wait c'est faite en parcourant des sujets sur le net.
*
*/

// include .h dans le quel sont contenus tous les include nécessaires.
#include "bor-util.h"

/**
* \return un entier non-signé, qui est la valeur voulue par l'utilisateur.
*
* Conversion d'une chaîne de caractères C entrée par l'utilisateur en un entier non-signé.
* Cette fonction permet notamment d'arrêter le programme dans le cas où rien n'a été entré par l'utilisateur et que la touche 'enter' est préssée.
*
* Fonction en partie modifiée mais provenant du web.
*/
unsigned int properScanf(){
    char keyInput[12];
    char* end = NULL;
    unsigned int output = 0;

	printf("Enter a number : \n");
    // Récupération des caractères entrés par l'utilisateur.
    fgets(keyInput, sizeof(keyInput), stdin);
    // Transformation de la chaîne de caractère entrée en un entier non-signé.
    output = strtoul(keyInput, &end, 12);

    return output;
}


/**
* \param : un entier non-signé à partir duquel commencer le compte à rebours.
*
* A chaque itération, le programme se met en pause durant une seconde.
*
*/
void bombTimer(unsigned int starter){
    for(unsigned int i = starter; i > 0; --i){
        printf("%u - %d\n", i, getpid());       // Affichage du compte à rebours.
        sleep(1);
    }
}


/**
* \param : un entier non-signé représentant le nombre de processus enfants voulus.
*
* Procédure permettant la création d'un nombre voulus de processus enfants.
* Chaque enfant exécute un compteur de n secondes.
*
*/
void makeChildren(unsigned int nbChild){
    int pid;
    for(unsigned int i = 0; i < nbChild; ++i) {
        pid = fork();

        // Utilisation d'un switch pour varié du if/else
        switch(pid){
            case -1:
                perror("Erreur au niveau du fork\n");
                exit(1);
            break;

            case 0:
                //printf("Child (%d): %d - father : %d\n", i + 1, getpid(), getppid()); // Affichage du fils créé.
                bombTimer(10);
                exit(0);
            break;
        }
    }
}


//! MAIN !//
int main(){
    unsigned int nbChild;   // Nombre de processus enfant voulus.

    /*
        Boucle Tant Que principale. Celle-ci s'exécute jusqu'à qu'un '0' soit entré par l'utilisateur.
        Une fois 0 entré, il n'est plus possible d'intéragir avec le processus père.
        L'utilisateur devra attendre la fin de tous les processus fils avant d'avoir la main sur la console.
    */
    do{
        nbChild = properScanf();
        makeChildren(nbChild);
    }while(nbChild != 0);

    // Tant qu'il y a un pid "attendu", nous continuons d'attendre d'autres potentiels processus.
    while(wait(NULL) > 0);

    // Une fois que tous les processus ont été attendu, affichage confirmant la fin du processus père.
    printf("Fin de tous les fils detectes\n");

    return 0;
}
