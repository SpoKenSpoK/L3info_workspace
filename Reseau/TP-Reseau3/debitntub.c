#include "bor-util.h"

double nb_char_read = 0;
// Handle signal ALARM
void handleSignal(int sig){
    printf("Caractères lus : %lf kB\n", nb_char_read/1000);
    alarm(1);
    nb_char_read = 0;
}


void closeUselessFD(int index, int** tabPipes, int nbTube){
    for(int i=0; i<nbTube; ++i){
        close(tabPipes[i][index]);
    }
}

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

// renvoie la valeur max de toutes les valeurs fd
int getMaxFromAllfd(int** tabPipes, int nbTube){
    int maxfd = -1;

    for(int i=0; i < nbTube; ++i)
        for(int j=0; j < 2; ++j)
            maxfd = getMaxfd(maxfd, tabPipes[i][j]);

    return maxfd;
}

void echoResult_select(int result){
    if( result < 0 ){
        switch(result){
            case EBADF : perror("Descripteur invalide dans la liste\n"); break;
            case EINTR : perror("signal delivré\n"); break;
            case EINVAL : perror("nfds < 0 ou mauvais timeout\n"); break;
            case ENOMEM: perror("plus assez de mémoire\n"); break;
        }
    }
    if( result == 0)
        perror("timeout expiré ...\n");
    //if( result > 0)
        //printf("%d : descripteurs éligibles\n", result);
}

// Procédure qui a pour but de compléter la liste des descripteurs en écriture
void init_WriteSet(fd_set* writeSet, int** tabPipes, int nbTube){
    FD_ZERO(writeSet);
    for(int i = 0; i < nbTube; ++i)
        FD_SET(tabPipes[i][1], writeSet);
}

void init_ReadSet(fd_set* readSet, int** tabPipes, int nbTube){
    FD_ZERO(readSet);
    for(int i = 0; i < nbTube; ++i)
        FD_SET(tabPipes[i][0], readSet);
}

void processChild(fd_set* readSet, int** tabPipes, int nbTube, char* string, int bufsize){
    init_ReadSet(readSet, tabPipes, nbTube);
    int result = select( getMaxFromAllfd(tabPipes, nbTube) + 1 , readSet, NULL, NULL, NULL);
    echoResult_select(result);

    if(result > 0){
        for(int i=0; i < nbTube; ++i){
            if(FD_ISSET(tabPipes[i][0], readSet)){
                nb_char_read += (double) bor_read(tabPipes[i][0], string, bufsize);
            }
        }
    }

}

void processFather(fd_set* writeSet, int** tabPipes, int nbTube, char* string, int bufsize){
    init_WriteSet(writeSet, tabPipes, nbTube);
    int result = select( getMaxFromAllfd(tabPipes, nbTube) + 1 , NULL, writeSet, NULL, NULL);
    echoResult_select(result);

    if(result > 0){
        for(int i=0; i < nbTube; ++i){
            if(FD_ISSET(tabPipes[i][1], writeSet)){
                bor_write(tabPipes[i][1], string, bufsize);
            }
        }
    }

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

    // Création d'un fils
    pid_t pid = fork();
    if(pid < 0){
        perror("Fork failed");
        exit(1);
    }
    if(pid == 0){ // Fils
        fd_set readSet;

        bor_signal(SIGALRM, handleSignal, SA_RESTART);
        alarm(1); // On initialise une fois

        closeUselessFD(1, tabPipes, nbTube);
        while(1){
            processChild(&readSet, tabPipes, nbTube, string, bufsize);
        }

        free(string);
        freePipes(nbTube, tabPipes);
        exit(0);
    }
    // Père
    fd_set writeSet;
    closeUselessFD(0, tabPipes, nbTube);
    while(1){
        processFather(&writeSet, tabPipes, nbTube, string, bufsize);
    }

    // Libération de la mémoire allouée
    free(string);
    freePipes(nbTube, tabPipes);
    return 0;
}
