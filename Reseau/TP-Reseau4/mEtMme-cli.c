#include "bor-util.h"

// Structure Client
typedef struct {
    char* listen_pipe_name;
    int listen_pipe;
    char pipe_to_server_name[100];
    char pipe_from_server_name[100];
    int pipe_to_server;
    int pipe_from_server;
} Client;


// Création des tubes nommés
int create_service_pipes(Client* param){
    sprintf( param->pipe_to_server_name, "pipe_to_server %d", (int) getpid() );
    sprintf( param->pipe_from_server_name, "pipe_from_server %d", (int) getpid() );

    if( mkfifo(param->pipe_to_server_name, 0644) < 0 ) return -1;
    if( mkfifo(param->pipe_from_server_name, 0622) < 0 ){
        unlink(param->pipe_to_server_name);
        return -1;
    }

    return 0;
}

// Envoie au server des noms des tubes
int contact_server(Client* param){
    char msg[200];
    sprintf( msg, "%s %s", param->pipe_to_server_name, param->pipe_from_server_name);

    param->listen_pipe = open(param->listen_pipe_name, O_WRONLY);
    if(param->listen_pipe < 0)
        return -1;

    int result = bor_write(param->listen_pipe, msg, 200);
    close(param->listen_pipe);

    return( result == 0 ? -1 : 0 );
}

// Ouverture en lecture / écriture des tubes nommés
int open_service_pipes(Client* param){
    param->pipe_to_server = open(param->pipe_to_server_name, O_WRONLY);
    if(param->pipe_to_server < 0) return -1;

    param->pipe_from_server = open(param->pipe_from_server_name, O_RDONLY);
    if(param->pipe_from_server < 0){
        close(param->pipe_to_server);
        return -1;
    }

    return 0;
}

// Dialog entre le client et le server
void start_dialog(Client* param){
    while(1){
        char buffer[200];
        int length = bor_read(0, buffer, 200);
        if(length == 0) break;

        int sent = bor_write(param->pipe_to_server, buffer, length);
        if(sent <= 0) break;
        length = bor_read_str(param->pipe_from_server, buffer, 200);
        if(length <= 0) break;
        bor_write(1, buffer, length);
    }
}

// fermeture des tubes nommés
void close_service_pipes(Client* param){
    close(param->pipe_to_server);
    close(param->pipe_from_server);
}

// suppression des tubes nommés
void unlink_service_pipes(Client* param){
    unlink(param->pipe_to_server_name);
    unlink(param->pipe_from_server_name);
}


int main(int argc, char* argv[]){
    if(argc != 2) return -1;

    Client param;
    param.listen_pipe_name = argv[1];

    if( create_service_pipes(&param) < 0 ){
        perror("Erreur a la creation des tubes nommes\n");
        return -1;
    }

    if( contact_server(&param) < 0 ){
        unlink_service_pipes(&param);
        perror("Erreur : impossibilite de contacter le server\n");
        return -1;
    }

    if( open_service_pipes(&param) < 0 ){
        unlink_service_pipes(&param);
        perror("Erreur : ouverture des tubes nomes fail\n");
    }

    start_dialog(&param);

    close_service_pipes(&param);
    unlink_service_pipes(&param);
    return 0;
}
