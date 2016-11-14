#include "bor-util.h"


// NON TERMINE

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

void insertClient(int* tabClients, int soc){
    for(int i=0; i < 128; ++i){
        if(tabClients[i] != -1){
            tabClients[i] = soc;
            return;
        }
    }
}

// Renvoi le plus grand file descriptor trouvé
int getMax(int one, int two){
    if(one > two) return one;
    return two;
}

int main(int argc, char* argv[]){

    char buffer[1024];

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

    if (bor_listen(soc, 20) < 0){
        perror("Erreur au niveau du bor_listen\n");
        cleanEnd(soc, serPath);
        return -1;
    }

    int tabClients[128];
    for(int i=0; i < 128; ++i){
        tabClients[i] = -1;
    }

    int socService;
    while( boucleWhile ){

        // Création d'une nouvelle liste
        fd_set set;
        FD_ZERO(&set);
        FD_SET(soc, &set);

        int maxFd = soc;

        // On récupère le plus grand filedescriptor
        for(int i=0; i<128; ++i){
            if( tabClients[i] != -1 ){
                maxFd = getMax( maxFd, tabClients[i] );
            }
        }

        // Select
        int result = select(maxFd + 1, &set, NULL, NULL, NULL);
        if(result <= 0){
            perror("Erreur au niveau du select");
            break;
        }

        // Insertion du client
        if( FD_ISSET(soc, &set) ){
            socService = bor_accept_un(soc, &cliSock);
            if( socService < 0 ){
                perror("Erreur au niveau du bor_accept_un\n");
                break;
            }

            printf("Connexion etablie\n");
            insertClient(tabClients, socService);
        }

        // Suppression des buffers qui deviennent vide après lecture
        // Application au niveau des digits pour traiter le message et le renvoyer au client correspondant
        for(int i = 0; i < 128; ++i) {
            if( tabClients[i] != -1 && FD_ISSET(tabClients[i], &set) ){
                result = bor_read(tabClients[i], buffer, sizeof (buffer));
                if(result <= 0) {
                    printf("Déconnexion du client n° %d\n", i);
                    close( tabClients[i] );
                    tabClients[i] = -1;
                }

                printf("Message recu : %s \n", buffer);
                for (int j = 0; j < result; ++j)
                    if (buffer[j] >= '0' && buffer[j] <= '9' && buffer[j] % 2 == 0)
                        bor_write(tabClients[i], buffer + j, 1);
            }
        }
    }

    cleanEnd(soc, serPath);
    return 0;
}
