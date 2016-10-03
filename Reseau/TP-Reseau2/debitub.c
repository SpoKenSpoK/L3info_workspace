#include "bor-util.h"

int main(int argc, char const* argv[]){
    if(argc != 2) return 0;

    int bufsize = atoi(argv[1]);
    printf("Taille du buffer : %d\n",bufsize);

    char* string = malloc( sizeof(char) * bufsize );

    int pipe[2];
    pipe(pipe);

    pid_t pid = fork();
    if(pid < 0){
        perror("Fork failed\n");
    }
    if(pid == 0){ // Fils

    }

    // Père

    return 0;
}
