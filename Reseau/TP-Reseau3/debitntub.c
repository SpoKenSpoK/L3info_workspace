#include "bor-util.h"

// Création d'un tableau d'entier à deux dimensions (allocation dynamique de la mémoire)
// Initialisation de chaque pipe
int** initPipes(int nbTube){
    int** tabPipes = malloc( sizeof(int*) * nbTube );
    for(int i=0; i < nbTube; ++i){
        tabPipes[i] = malloc( sizeof(int) * 2 );
        pipe(tabPipes[i]);
    }

    return tabPipes;
}

// Libération de la mémoire allouée pour créer le tableau à deux dimensions
void freePipes(int nbTube, int** tabPipes){
    for(int i=0; i < nbTube; ++i)
        free(tabPipes[i]);

    free(tabPipes);
}

// renvoie le max des deux valeurs de fd
int getMaxfd(int fdOne, int fdTwo){
    if(fdOne > fdTwo) return fdOne;
    return fdTwo;
}





int main(int argc, char const* argv[]){
    if(argc != 3) return 0;

    // Convertion d'une chaîne de caractères en entier
    int bufsize = atoi(argv[1]);
    int nbTube = atoi(argv[2]);
    printf("Taille du buffer : %d\n", bufsize);
    printf("Nombre de tubes : %d\n", nbTube);

    char* string = malloc( sizeof(char) * bufsize );    // Allocation d'une chaîne de caractères de taille bufsize
    int** tabPipes = initPipes(nbTube);                 // Allocation d'un tableau d'entier à deux dimensions

    // Initialisation de la liste des descripteurs
    fd_set writeSet;
    struct timeval timer;

    // Remise à 0 de la liste des descripteurs en écriture
    FD_ZERO(&writeSet);
    //FD_SET(0, &setOne);

    // le timer entre chaque select est de 1 secondes
    timer.tv_sec = 1;
    timer.tv_usec = 0;

    // Création d'un fils
    pid_t pid = fork();
    if(pid < 0){
        perror("Fork failed");
        exit(1);
    }
    if(pid == 0){ // Fils


        free(string);
        freePipes(nbTube, tabPipes);
        exit(0);
    }
    // Père


    // Libération de la mémoire allouée
    free(string);
    freePipes(nbTube, tabPipes);
    return 0;
}
