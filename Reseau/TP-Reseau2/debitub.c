#include "bor-util.h"

double nb_char_read = 0;

// Handle signal ALARM
void handleSignal(int sig){
    printf("Caractères lus : %lf kB\n", nb_char_read/1000);
    alarm(1);
}

// Handle du signal SIGPIPE
void handle_SIGPIPE(int sig){
    printf("\nSIGNAL SIGPIPE RECU PAR LE PERE ! (kill -13 %d)\n\n", getpid());
}

int main(int argc, char const* argv[]){
    if(argc != 2) return 0;

    // Convertion d'une chaîne de caractères en entier
    int bufsize = atoi(argv[1]);
    printf("Taille du buffer : %d\n",bufsize);

    // Allocation d'une chaîne de caractères
    char* string = malloc( sizeof(char) * bufsize );

    // Pipe
    int pipefd[2];
    pipe(pipefd);

    // Fork
    pid_t pid = fork();
    if(pid < 0){
        perror("Fork failed\n");
        exit(1);
    }
    if(pid == 0){ // Fils
        bor_signal(SIGALRM, handleSignal, SA_RESTART);
        alarm(1); // On initialise une fois
        close(pipefd[1]);
        while(1){
            nb_char_read = (double) bor_read(pipefd[0], string, bufsize);
        }

        free(string);
        exit(0);
    }

    // Père
    bor_signal(SIGPIPE, handle_SIGPIPE, SA_RESTART);
    while(1){
        close(pipefd[0]);
        bor_write(pipefd[1], string, bufsize);
    }

    free(string);

    return 0;
}
