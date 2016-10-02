/** HECKEL Thibault - TP1 Réseau & Com. UNIX - L3 Informatique site de Luminy | Septembre 2016
* Partie III - 1) 2)
*
*/

#include "bor-util.h"

// Variables globales
pid_t childOne;
pid_t childTwo;
pid_t father;
pid_t thePeer;

// Procédure handleExit qui est appelée lorsque le signal SIGALRM est reçu
void handleExit(int signal){
    printf("TIMEOUT\n");
    exit(1);
}

// Procédure handSignal qui est appelée lorsque le signal SIGUSR1 est reçu
// A pour but d'envoyer un signal, via "kill" à un PID voulu.
void handleSignal(int signal){
    printf("%d a envoyé le signal SIGUSR1 à %d\n", getpid(), thePeer);
    alarm(5);
    sleep(1); // <= modifier cette valeur pour effectuer un signal SIGALRM
    kill(thePeer, SIGUSR1);
}

// Procédure permettant au processus enfant numéro 1 d'intéragir avec les différents signaux reçu
void childOneProcess(int peer){
    thePeer = peer;
    bor_signal(SIGUSR1, handleSignal, SA_RESTART);
    bor_signal(SIGALRM, handleExit, SA_RESTART);
    while(1) pause();
}


// Procédure permettant au processus enfant numéro 2 d'intéragir avec les différents signaux reçu
void childTwoProcess(int peer){
    thePeer = peer;
    bor_signal(SIGUSR1, handleSignal, SA_RESTART);
    bor_signal(SIGALRM, handleExit, SA_RESTART);
    kill(getpid(), SIGUSR1);
    while(1) pause();
}


// Procédure permettant au processus père d'intéragir avec les différents signaux reçu
void fatherProcess(int peer){
    thePeer = peer;
    bor_signal(SIGUSR1, handleSignal, SA_RESTART);
    bor_signal(SIGALRM, handleExit, SA_RESTART);
    while((wait(NULL) <= 0)) pause();
}


//! MAIN !//
int main(){

    father = getpid();
    int pid = fork();
    int pidTwo;

    if(pid == -1){
        perror("Fork failed");
        exit(1);
    }
    else if(pid == 0){
        // Premier fils
        childOne = pid;
        printf("Père %d\n", father);
        childOneProcess(father);
        exit(0);
    }
    else{
        pidTwo = fork();
        childOne = pid;

        if(pidTwo == -1){
            perror("Fork Two failed");
            exit(1);
        }
        else if(pidTwo == 0){
            // Fils deux
            printf("childOne %d\n",childOne);
            childTwoProcess(childOne);
            exit(0);
        }
        else{
            childTwo = pidTwo;
            // Père
            printf("childTwo %d\n", childTwo);
            fatherProcess(childTwo);
            exit(0);
        }
    }

    return 0;
}
