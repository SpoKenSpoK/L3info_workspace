#include "bor-util.h"

#define SLOTS_NB 32

int shutDown = 1;
char buffer[2048];

void handleSignal(int sig){
    printf("\nSignal recu : %d\n", sig);
    switch (sig) {
        case SIGINT: shutDown = 0;
        case SIGPIPE: shutDown = 0;
    }
}

typedef enum { S_FREE, S_READ_REQUEST, S_WRITE_RESPONSE } State;

typedef struct {
    State state;
    int soc;                    // Socket de service, défault -1
    struct sockaddr_in adr;     // Adresse du client
} Slot;

typedef struct {
    Slot slots[SLOTS_NB];
    int listen_soc;             // Socket d'écoute
    struct sockaddr_in adr;     // Adresse du serveur
} Server;

int is_free_slot(Slot* slot){ return slot->state == S_FREE; }

void init_slot(Slot* slot){
    slot->state = S_FREE;
    slot->soc = -1;
    memset(&slot->adr, 0, sizeof( slot->adr ));
}

void free_slot(Slot* slot){
    if( !is_free_slot(slot) ){
        close(slot->soc);
        init_slot(slot);
    }
}

void init_server(Server* ser){
    for(unsigned int i=0; i < SLOTS_NB; ++i)
        init_slot( &ser->slots[i] );

    ser->listen_soc = -1;
    memset(&ser->adr, 0, sizeof( ser->adr ));
}

int find_free_slot(Server* ser){
    for(unsigned int i=0; i < SLOTS_NB; ++i)
        if(is_free_slot(&ser->slots[i]))
            return i;

    return -1;
}

void close_server(Server* ser){
    close(ser->listen_soc);

    for(unsigned int i=0; i < SLOTS_NB; ++i)
        free_slot(&ser->slots[i]);
}

int start_server(Server* ser, int port){
    init_server(ser);

    int soc = bor_create_socket_in(SOCK_STREAM, port, &ser->adr);  //Création de la socket serveur
    if( soc < 0 ){
        perror("Erreur : creation de la socket serveur fail\n");
        return -1;
    }

    ser->listen_soc = soc;
    if( bor_listen(ser->listen_soc, SLOTS_NB) < 0 ){
        close_server(ser);
        perror("Erreur : socket d ecoute fail\n");
        return -1;
    }

    return 0;
}

int accept_connection(Server* ser){
    int index = find_free_slot(ser);

    if(index < 0) {
        printf("Trop de clients connectes\n");
        return -1;
    }

    Slot* slot = &ser->slots[ index ];
    slot->soc = bor_accept_in(ser->listen_soc, &slot->adr);

    if(slot->soc < 0){
        perror("Erreur au niveau du bor_accept_in");
        return -1;
    }

    slot->state = S_READ_REQUEST;

    return 0;
}

int read_request(Slot* slot){
    int result = bor_read_str(slot->soc, buffer, sizeof(buffer));
    if(result < 0){
        perror("Erreur : bor_read_str fail\n");
        return -1;
    }

    printf("Read : %s\n", buffer);
    slot->state = S_WRITE_RESPONSE;

    return result;
}

int write_response(Slot* slot){
    const char* buffHTML = "HTTP/1.1 500 Erreur du serveur\n\n <html><body><h1>Serveur en construction !!</h1></body></html>\n";
    int result = bor_write_str(slot->soc, buffHTML);
    if(result < 0){
        perror("Erreur : bor_write_str fail\n");
        return -1;
    }

    printf("Write : %s\n", buffer);
    slot->state = S_READ_REQUEST;

    return -1;
}


void handle_ready_slot(Slot* slot, fd_set* set_read, fd_set* set_write){
    int request = 1;

    if( is_free_slot(slot) ) return;

    if( slot->state == S_READ_REQUEST && FD_ISSET(slot->soc, set_read) )
        request = read_request(slot);

    else if( slot->state == S_WRITE_RESPONSE && FD_ISSET(slot->soc, set_write) )
        request = write_response(slot);

    if(request <= 0) free_slot(slot);
}

void prepare_select(Server* ser, int* maxfd, fd_set* set_read, fd_set* set_write){
    Slot* slot;

    *maxfd = ser->listen_soc;

    FD_ZERO(set_read);
    FD_ZERO(set_write);
    FD_SET(ser->listen_soc, set_read);

    for(unsigned int i = 0; i < SLOTS_NB; ++i){
        slot = &ser->slots[i];

        if( is_free_slot(slot) ) continue;

        if( slot->state == S_READ_REQUEST  ){
            FD_SET(slot->soc, set_read);
            if( slot->soc > *maxfd ) *maxfd = slot->soc;
        }

        if( slot->state == S_WRITE_RESPONSE ){
            FD_SET(slot->soc, set_write);
            if( slot->soc > *maxfd ) *maxfd = slot->soc;
        }
    }
}

int do_select(Server* ser){
    int maxfd;

    fd_set set_read;
    fd_set set_write;

    printf("init prepare_select ... \n");
    prepare_select(ser, &maxfd, &set_read, &set_write);

    int result = select(maxfd + 1, &set_read, &set_write, NULL, NULL);
    if(result <= 0) {
        perror("Erreur : do select fail\n");
        return -1;
    }

    if( FD_ISSET(ser->listen_soc, &set_read) ) accept_connection(ser);

    for(int i = 0; i < SLOTS_NB; i++)
        handle_ready_slot(&ser->slots[i], &set_read, &set_write);

    return 0;
}


int main(int argc, char* argv[]) {
    bor_signal(SIGINT, handleSignal, 0);
    bor_signal(SIGPIPE, handleSignal, 0);

    if( argc != 2 ){
        perror("Erreur : nombre d arguments incorrects\n");
        return -1;
    }

    Server ser;
    int serPort = atoi(argv[1]);

    printf("init server ... ");
    if( start_server(&ser, serPort) < 0 ) return -1;
    printf("done !\n");

    for(;;){
        printf("init select ... \n");
        if( do_select(&ser) < 0 ){
            perror("fail\n");
            break;
        }

        if( shutDown == 0 ) break;
    }

    close_server(&ser);
    printf("server down\n");

    return 0;
}
