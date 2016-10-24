#include "bor-util.h"

const char* path;
int soc ;
char* buffer;

void cleanEnd(){
    close(soc);
    unlink(path);
    free(buffer);
}

void handleSIGINT(int sig){
    if(sig == SIGINT){
        cleanEnd();
        exit(-1);
    }
}

int main(int argc, char* argv[]){
    if(argc != 3){ perror("Erreur au niveau des arguments\n"); return -1; }

    bor_signal(SIGINT, handleSIGINT, SA_RESTART);

    path = argv[1];
    const char* path_serv = argv[2];

    struct sockaddr_un sa;
    struct sockaddr_un sb;
    size_t bufsize = 200;
    ssize_t msg_from;
    ssize_t msg_to;

    soc = bor_create_socket_un(SOCK_DGRAM, path, &sa);
    buffer = malloc( sizeof(*buffer) * bufsize );

    // Création adresse socket pour le serveur
    bor_set_sockaddr_un(path_serv, &sb);

    // Envoie vers le serveur :
    sprintf(buffer, "Hello");
    msg_to = bor_sendto_un_str(soc, buffer, &sb);
    if (msg_to < 0) {
        cleanEnd();
        perror("Erreur : sendto\n");
        exit(-1);
    }
    printf("Message envoye : %s\n", buffer);

    // Réception depuis le server :
    msg_from = bor_recvfrom_un_str(soc, buffer, bufsize, &sb);
    if (msg_from < 0) {
        cleanEnd();
        perror("Error : recvfrom\n");
        exit(-1);
    }
    printf("Message recu : %s\n", buffer);

    cleanEnd();

    return 0;
}
