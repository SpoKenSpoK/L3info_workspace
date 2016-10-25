#include "bor-util.h"

struct tabHello {
    char sun_path[108];
    unsigned int cpt;
};

char* buffer;
const char* path;
int soc ;

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
    if(argc != 2){ perror("erreur au niveau des arguments\n"); return -1; }

    bor_signal(SIGINT, handleSIGINT, SA_RESTART);

    path = argv[1];
    struct sockaddr_un sa;
    struct sockaddr_un sb;
    size_t bufsize = 200;
    ssize_t msg_from;
    ssize_t msg_to;

    soc = bor_create_socket_un(SOCK_DGRAM, path, &sa);
    buffer = malloc( sizeof(*buffer) * bufsize );

    struct tabHello helloTab[10];
    for(unsigned int i=0; i < 10; ++i){
        sprintf(helloTab[i].sun_path, " ");
        helloTab[i].cpt = 0;
    }

    while(1){
        // Réception depuis le client :
        msg_from = bor_recvfrom_un_str(soc, buffer, bufsize, &sb);
        if (msg_from < 0) {
            cleanEnd();
            perror("Error : recvfrom\n");
            exit(-1);
        }
        //printf("Message recu: %s\n", buffer);

        if( strcmp(buffer,"HELLO") == 0 ){

            for(unsigned int i = 0; i < 10; ++i){
                if( strcmp(sb.sun_path, helloTab[i].sun_path) == 0 ){ helloTab[i].cpt++; break; }
                else{
                    if(helloTab[i].cpt == 0){
                        sprintf(helloTab[i].sun_path, sb.sun_path);
                        helloTab[i].cpt = 1;
                        break;
                    }
                }
            }
        }
        else if( strcmp(buffer, "NUMBER") == 0 ) {

            for(unsigned int i = 0; i < 10; ++i){
                printf("| %s : %u |\n", helloTab[i].sun_path, helloTab[i].cpt);

                if( strcmp(helloTab[i].sun_path, sb.sun_path) == 0 ){
                    sprintf(buffer, "%u", helloTab[i].cpt);
                    sprintf(helloTab[i].sun_path, " ");
                    helloTab[i].cpt = 0;
                }
            }

            // Envoie vers le client :
            msg_to = bor_sendto_un_str(soc, buffer, &sb);
            if (msg_to < 0) {
                cleanEnd();
                perror("Error : sendto\n");
                exit(-1);
            }
            printf("Message envoye %s depuis %s\n", buffer, sb.sun_path);
        }
    }

    cleanEnd();

    return 0;
}
