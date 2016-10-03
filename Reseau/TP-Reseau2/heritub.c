#include "bor-util.h"

int main(){
    // Création d'un pipe
    int pipefd[2];
    pipe(pipefd);

    // Lecture des caractères entrés par l'utilisateur
    char buffer[80]; // <= taille arbitraire
    ssize_t nbChar_read = bor_read_str(0, buffer, 80);
    if(nbChar_read == 0) exit(1);

    // Affichage dans la sortie standard
    bor_write(1,buffer,10);
    printf("\n");

    // Ecriture dans le pipe
    bor_write_str(pipefd[1], buffer);
    close(pipefd[1]);

    pid_t pid = fork();
    if(pid < 0){ // Erreur
        perror("Fork failed");
        close(pipefd[0]);
        exit(1);
    }

    if(pid == 0){ // Fils
        bor_read_str(pipefd[0], buffer, 7+1);
        bor_write_str(1, buffer);
        printf("\n");
        close(pipefd[0]);
        exit(0);
    }

    // Père
    bor_read_str(pipefd[0], buffer, 3+1);
    bor_write_str(1, buffer);
    printf("\n");
    close(pipefd[0]);
    exit(0);
}
