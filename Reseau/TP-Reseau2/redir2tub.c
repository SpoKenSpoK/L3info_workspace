#include "bor-util.h"

int main(){
    int pipeOne[2];
    int pipeTwo[2];

    pipe(pipeOne);
    pipe(pipeTwo);

    int pidOne = fork();
    if(pidOne < 0){
        perror("Fork One failed");
        exit(1);
    }
    if(pidOne == 0){ // Fils number One
        // Fermeture des entrées/sorties non utilisées
        close(pipeOne[0]);
        close(pipeTwo[0]);
        close(pipeTwo[1]);

        // Redirection de la sortie standard vers le tube1
        dup2(pipeOne[1], 1);

        // Fermeture de la sortie du tube1
        close(pipeOne[1]);

        // Recouvrement
        execlp("ls", "ls", "-t", NULL);

        perror("execlp ls -t failed\n");
        exit(1);
    }
    // Père

    int pidTwo = fork();
    if(pidTwo < 0){
        perror("Fork Two failed");
        exit(1);
    }
    if(pidTwo == 0){ // Fils number Two
        // Fermeture des entrées/sorties non utilisées
        close(pipeOne[1]);
        close(pipeTwo[0]);

        // Redirections
        dup2(pipeOne[0], 0);
        dup2(pipeTwo[1], 1);

        // Fermetures
        close(pipeOne[0]);
        close(pipeTwo[1]);

        // Recouvrement
        execlp("sort", "sort", NULL);

        perror("execlp sort failed\n");
        exit(1);
    }

    // Père
    // Fermeture des entrées/sorties non utilisées
    close(pipeOne[0]);
    close(pipeOne[1]);
    close(pipeTwo[1]);

    // Redirection de l'entrée standard vers le tube2
    dup2(pipeTwo[0], 0);

    // Fermeture de l'entrée du tube2
    close(pipeTwo[0]);

    // Recouvrement
    execlp("head", "head", "-3", NULL);

    perror("execlp sort failed\n");
    exit(1);
}
