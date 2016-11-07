#include "bor-util.h"

int boucleWhile = 1;

void handle(int sig) {
    printf("Signal recu : %d\n", sig);
    switch (sig) {
        case SIGINT: boucleWhile = 0; break;
        case SIGPIPE: boucleWhile = 0; break;
    }
}

void cleanEnd(int soc, const char* path){
    close(soc);
    unlink(path);
    printf("Server down\n");
}

void childProcess(int socService) {
    int result;
    char buffer[1024];

    printf("childProcess start\n");

    while( boucleWhile ) {
        char finalBuffer[1024];
        int index = 0;

        result = bor_read_str(socService, buffer, sizeof (buffer) - 1);
        if (result <= 0){
            perror("Erreur provenant de la lecture du message du client : ctrl + D certainement\n");
            break;
        }

        printf("Message recu depuis le client : %s\n", buffer);

        // Test sur les différents digits en s'assurant qu'il s'agit bien de chiffres.
        for (int i = 0; buffer[i] != 0; ++i) {
            // Comparaison sur le code ASCII, pas besoin de convertir en entier.
            if (buffer[i] >= '0' && buffer[i] <= '9' && buffer[i] % 2 == 0) {
                finalBuffer[index] = buffer[i];
                index++;
            }
        }
        finalBuffer[index] = '\0';

        result = bor_write_str(socService, finalBuffer);
        if (result <= 0){
            perror("Erreur sur l ecriture vers le client\n");
            break;
        }

        printf("Message envoye au client : %s\n", finalBuffer);
    }

    printf("childProcess end\n");
    close(socService);
}


int main(int argc, char* argv[]){

    bor_signal(SIGINT, handle, 0);
    bor_signal(SIGPIPE, handle, 0);

    if(argc != 2){
        perror("Erreur : nombre d arguments donnes en parametres\n");
        return -1;
    }

    printf("Server up\n");

    const char* serPath = argv[1];

    struct sockaddr_un serSock;
    struct sockaddr_un cliSock;

    int soc = bor_create_socket_un(SOCK_STREAM, serPath, &serSock);
    if(soc < 0){
        perror("Erreur a la creation du socket Serveur\n");
        return -1;
    }

    if (bor_listen(soc, 1) < 0){
        perror("Erreur au niveau du bor_listen\n");
        cleanEnd(soc, serPath);
        return -1;
    }

    int socService;
    while( boucleWhile ){
        socService = bor_accept_un(soc, &cliSock);
        if( socService < 0 ){
            perror("Erreur au niveau du bor_accept_un\n");
            break;
        }

        pid_t fils = fork();
        if(fils < 0){
            perror("Erreur au niveau du fork\n");
            return -1;
        }
        if(fils == 0){ // Fils
            close(soc);
            childProcess(socService);
            return 0;
        }
        // Père
    }

    cleanEnd(soc, serPath);
    return 0;
}
