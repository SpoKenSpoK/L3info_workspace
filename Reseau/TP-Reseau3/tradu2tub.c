#include "bor-util.h"

int main(){
    int pipeOne[2];
    int pipeTwo[2];

    pipe(pipeOne);
    pipe(pipeTwo);

    pid_t pid = fork();
    if(pid < 0){
        perror("Fork failed");
        exit(1);
    }

    if(pid == 0){ // Fils
        close(pipeTwo[0]);
        close(pipeOne[1]);

        dup2(pipeOne[0], 0);
        dup2(pipeTwo[1], 1);

        close(pipeOne[0]);
        close(pipeTwo[1]);

        execlp("cat", "cat", NULL);
        //execlp("tr", "tr", "a-z", "A-Z", NULL);
        exit(0);
    }
    // Père
    int result;
    char tmp[80];
    fd_set readSet;

    close(pipeOne[0]);
    close(pipeTwo[1]);

    do{
        FD_ZERO(&readSet);
        FD_SET(0, &readSet);
        FD_SET(pipeTwo[0], &readSet);

        result = select(pipeTwo[0]+1, &readSet, NULL, NULL, NULL);

        if(FD_ISSET(0, &readSet)){
            bor_read_str(0, tmp, 80);
            bor_write_str(pipeOne[1], tmp);
        }

        if(FD_ISSET(pipeTwo[0], &readSet)){
            bor_read_str(pipeTwo[0], tmp, 80);
            bor_write_str(1, tmp);
        }

    }while(result > 0);

    return 0;
}
