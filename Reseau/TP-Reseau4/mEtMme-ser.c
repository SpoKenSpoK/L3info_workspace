#include "bor-util.h"

// Structure Server
typedef struct {
    char* family_namefile;
    char* listen_pipe_name;
    int listen_pipe;
    char pipe_from_client_name[100];
    char pipe_to_client_name[100];
    int pipe_from_client;
    int pipe_to_client;
} Server;


int create_listener(Server* server){
    if( mkfifo(server->listen_pipe_name, 0600) < 0 ) return -1;
    return 0;
}

int open_listener(Server* server){
    server->listen_pipe = open(server->listen_pipe_name, O_RDONLY);
    if(server->listen_pipe < 0)
        return -1;

    return 0;
}

void close_listener(Server* server){
    close(server->listen_pipe);
}

void delete_listener(Server* server){
    unlink(server->listen_pipe_name);
}

int open_client_pipes(Server* server){
    server->pipe_to_client = open(server->pipe_to_client_name, O_WRONLY);
    if (server->pipe_to_client < 0)
        return -1;

    server->pipe_from_client = open(server->pipe_from_client_name, O_RDONLY);
    if (server->pipe_from_client < 0) {
        close(server->pipe_to_client);
        return -1;
    }

    return 0;
}

char* find_child(Server* server, char* name){
    // Ouverture en lecture du fichier txt
    FILE *file = fopen(server->family_namefile, "r");

    if (file == NULL) {
        perror("Erreur a l ouverture du fichier txt\n");
        exit(-1);
    }


    char buffer[200];
    char tmp[200];

    name[strlen(name) - 1] = '\0';

    while( fgets(buffer, sizeof(buffer), file) != NULL ){
        sscanf(buffer, "%s", tmp);
        buffer[strlen(buffer) - 1] = '\0';

        if (strcasecmp(name, tmp) == 0)
            return strstr(buffer, ": ");
    }

    fclose(file);

    return " !! aucun nom de famille ne correspond";

}

void start_dialog(Server* server){
    while(1){
        char buffer[200];
        int length = bor_read_str(0, buffer, 200);
        if(length <= 0) break;

        char* child = find_child(server, buffer);
        length = bor_write_str(server->pipe_to_client, child);
        if (length <= 0) break;
    }
}



int main(int argc, char* argv[]){
    if(argc != 3) return -1;

    Server server;
    server.listen_pipe_name = argv[1];
    server.family_namefile = argv[2];

    if( create_listener(&server) < 0 ){
        perror("Erreur a la creation du tube d ecoute\n");
        return -1;
    }

    while(1){

        if( open_listener(&server) < 0 ){
            perror("Erreur a l ouverture du tube d ecoute\n");
            unlink(server.listen_pipe_name);
            return -1;
        }

        char buffer[200];

        bor_read_str(server.listen_pipe, buffer, 200);

        pid_t pid = fork();
        if(pid < 0){
            perror("Erreur a la creation du fils\n");
            exit(-1);
        }

        if(pid == 0){ // fils
            sscanf(buffer, "%s %s", server.pipe_from_client_name, server.pipe_to_client_name);
            open_client_pipes(&server);
            start_dialog(&server);
        }

        close_listener(&server);
    }

    delete_listener(&server);
}
