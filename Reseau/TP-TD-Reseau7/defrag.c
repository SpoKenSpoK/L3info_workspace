#include "bor-util.h"

/// L'Histoire du ligne / ligne avec le buffer n'est pas du tout claire

int shutDown = 1;
int MAXSIZE = 2048;

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

void seekRow(char buffer[]){
    char tmp [ MAXSIZE ];
    int cpt = 0;

    for(int i = 0; i < MAXSIZE; ++i){
        if( buffer[i] == '\n'){
            printf("Lecture : %s\n",tmp);
            for(int j = 0; j < cpt; ++j){ tmp[j] = 0; }
            cpt = 0;
        }
        else{
            tmp[cpt] = buffer[i];
            ++cpt;
        }
    }

    printf("Lecture finale : %s\n",tmp);
}

int connection(int soc, const struct sockaddr_in *sa){
    printf("Connexion au serveur %s:%d en cours\n", inet_ntoa(sa->sin_addr), ntohs(sa->sin_port));
    return bor_connect_in(soc, sa);
}

int dialog(int soc){
    char buffer[ MAXSIZE ];
    int pos = 0;
    int result = bor_read_str(soc, buffer, sizeof(buffer));

    if( result <= 0 ) perror("Erreur a la lecture ou EOF\n");

    return result;
}

int main(int argc, char* argv[]){
    bor_signal(SIGINT, handleSignal, 0);
    bor_signal(SIGPIPE, handleSignal, 0);

    if( argc != 3 ) return -1;

    // Création d'un socket pour le client
    struct sockaddr_in cliSock;
    struct sockaddr_in servSock;

    const char* servHostname = argv[1]; // Nom du serveur
    int servPort = atoi(argv[2]); // Port du serveur

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
    }

    printf("Connexion etablie\n");

    // Dialog avec le serveur
    for(;;){
        if( dialog(soc) <= 0 ) break;
        if( shutDown == 0 ) break;
    }

    cleanEnd(soc);
    return 0;
}
