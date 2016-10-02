#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* exit */
#include <unistd.h>     /* close */
#include <string.h>     /* strlen */
#include <fcntl.h>      // read | close | write

char* getKeyboardChar(const unsigned int wantedChar){
    unsigned int cpt = 0;
    char* strTmp = malloc( sizeof(char) * wantedChar );

    printf("Ecrire %u caracteres puis presser entree : \n", wantedChar);
    do{
        strTmp[cpt] = getchar();
        ++cpt;
        //printf("%c - %u - %lu\n", strTmp[cpt-1], cpt, strlen(strTmp));
    }while(cpt != wantedChar);

    return strTmp;
}

void writePipe(int pipefd, char* string){
    int result = write(pipefd, string, strlen(string));
    if (result < 0) { perror("Erreur au niveau du write dans le pipe"); exit (1); }
}

void readPipe(int pipefd, char* string, unsigned int nbChar){
    int result = read(pipefd, string, sizeof(char) * nbChar);
    if (result < 0) { perror("Erreur au niveau du read dans le pipe"); exit (1); }
    string[result] = 0; // '\0
}

void printStr(char* string){
    printf("%s\n", string);
}

int main(){
    // Création d'un pipe
    int pipefd[2];
    pipe(pipefd);

    // Récupération des caractères tapés par l'utilisateur :
    const unsigned int wantedChar = 10;
    char* string = getKeyboardChar(wantedChar);

    // Ecriture dans le pipe
    writePipe(pipefd[1], string);

    // Fork()
    pid_t pid = fork();
    switch(pid){
        case -1:
            perror("Fork failed");
            exit(1);
        break;

        case 0: // Fils
            readPipe(pipefd[0], string, 7); // Extraction du pipe des septs derniers caractères
            printStr(string);
            free(string);
            exit(0);
        break;

        default: // Père
            readPipe(pipefd[0], string, 3); // Extraction dans le pipe des trois premiers caractères
            printStr(string);
            free(string);
            exit(0);
        break;
    }
}
