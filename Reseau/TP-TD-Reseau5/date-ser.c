#include "bor-util.h"
// Communication UDP :

const char* path;
int soc ;

void handleSIGINT(int sig){
    if(sig == SIGINT){
        close(soc);
        unlink(path);
        exit(-1);
    }
}

int main(int argc, char* argv[]){
    if(argc != 2){ perror("erreur au niveau des arguments\n"); return -1; }
    bor_signal(SIGINT, handleSIGINT, SA_RESTART);

    path = argv[1];
    struct sockaddr_un sa;
    struct sockaddr_un sb;
    size_t bufsize = 200;
    ssize_t msg_from;
    ssize_t msg_to;
    time_t timer;

    soc = bor_create_socket_un(SOCK_DGRAM, path, &sa);

    while(1){
        char* buffer = malloc( sizeof(*buffer) * bufsize );

        // Réception depuis le client :
        msg_from = bor_recvfrom_un_str(soc, buffer, bufsize, &sb);
        if (msg_from < 0) {
            close(soc);
            unlink(path);
            perror("Error : recvfrom\n");
            exit(-1);
        }
        printf("Message recu: %s\n", buffer);

        // Récupération la date courante :
        time(&timer);
        sprintf(buffer, "%s", ctime(&timer));

        // Envoie vers le client :
        msg_to = bor_sendto_un_str(soc, buffer, &sb);
        if (msg_to < 0) {
            close(soc);
            unlink(path);
            perror("Error : sendto\n");
            exit(-1);
        }
        printf("Message envoye\n");

        free(buffer);
    }

    close(soc);
    unlink(path);

    return 0;
}
