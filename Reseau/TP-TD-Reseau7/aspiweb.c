#include "bor-util.h"

int shutDown = 1;
char buffer[2048];

void cleanEnd(int soc){
    close(soc);
    printf("Client down\n");
}

void handleSignal(int sig){
    printf("Signal recu : %d\n", sig);
    switch (sig) {
        case SIGINT: shutDown = 0;
        case SIGPIPE: shutDown = 0;
    }
}

int connection(int soc, const struct sockaddr_in *sa){
    printf("Connexion au serveur %s:%d en cours\n", inet_ntoa(sa->sin_addr), ntohs(sa->sin_port));
    return bor_connect_in(soc, sa);
}

int dialog(int soc, const char* filename){
    int result = bor_read_str(soc, buffer, sizeof(buffer));

    if( result < 0 ) perror("Erreur a la lecture\n");
    else printf("Lecture : %s\n", buffer);

    FILE* file = fopen(filename, "w");
    if (file == NULL)
        perror("Erreur : fichier impossible a ouvrir en ecriture\n");
    else{
        printf("Ecriture dans le fichier\n");
        fprintf(file, "%s", buffer);
    }

    return result;
}

int main(int argc, char* argv[]){
    bor_signal(SIGINT, handleSignal, 0);
    bor_signal(SIGPIPE, handleSignal, 0);

    if( argc != 5 ){
        perror("Erreur : nombre d arguments non respecte\n");
        return -1;
    }

    // Création d'un socket pour le client
    struct sockaddr_in cliSock;
    struct sockaddr_in servSock;

    const char* servHostname = argv[1]; // Nom du serveur
    int servPort = atoi(argv[2]);
    const char* request = argv[3];
    const char* filename = argv[4];

    int soc = bor_create_socket_in(SOCK_STREAM, 0, &cliSock);   // Création du socket client
    if( soc < 0 ){
        perror("Erreur : creation de la socket client fail\n");
        return -1;
    }

    int result = bor_resolve_address_in(servHostname, servPort, &servSock); // Résolution de l'adresse serveur
    if( result < 0 ){
        perror("Erreur : resolution de l adresse serveur fail\n");
        cleanEnd(soc);
        return -1;
    }

    result = connection(soc, &servSock);    // Connexion au serveur
    if( result < 0 ){
        perror("Erreur : connexion au serveur fail\n");
        cleanEnd(soc);
        return -1;
    }else printf("Connexion etablie\n");

    sprintf( buffer, "GET /%s HTTP/1.1\nHost: %s\n\n", request, servHostname );

    printf("Envoie de la requete suivante : %s\n", buffer);
    if( bor_write_str(soc, buffer) < 0 ){
        perror("Erreur : l envoi de la requete a fail\n");
        cleanEnd(soc);
        return -1;
    }

    for(;;){
        if( dialog(soc, filename) <= 0 ) break;
        if( shutDown == 0 ) break;
    }

    cleanEnd(soc);
    return 0;
}
