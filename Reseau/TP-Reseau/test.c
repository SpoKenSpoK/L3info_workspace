#include "bor-util.h"

char* getMessage(int nb_char_max){
  char* message = malloc(sizeof(char) * nb_char_max);
  int cpt_char = 0;

  printf("Saisir %d caractères :", nb_char_max);

  while(cpt_char < nb_char_max){
    message[cpt_char] = getchar();
    cpt_char++;
  }

  return message;
}

void write_message(int fd, char* message, int nb_char_write){
  int check_succes = write(fd, message, nb_char_write);
  if(check_succes < 0){
      printf("Erreur lors de l'écriture dans le tube du message : %s\n",message);
      exit(1);
  }
}

void print_message(char* message){
  printf("%s\n", message);
}

void read_message(int fd, char* message, int nb_char_read){
  int nb_octet_read = read(fd, message, sizeof(char) * nb_char_read);

  if(nb_octet_read < 0){
      printf("Erreur lors de la lecture dans le tube du message : %s\n", message);
      exit(1);
  }
  message[nb_octet_read] = 0;
}

int get_last_read_char(int size_string, int nb_char_read_first){
  return size_string - nb_char_read_first;
}


int main(int argc, char const *argv[]) {
  int nb_char_read_first = 3, size_string = 10;

/*
  if(argc != 3){
    printf("Usage : ./heritub nb_char_read nb_char_read_first");
    exit(1)
  }
  int nb_char_read_first = atoi(argv[2]), size_string = atoi(argv[1]);
*/

  pid_t pere;
  int pipe_fd[2];
  pipe(pipe_fd);

  char* buffer_message = getMessage(size_string);
  write_message(pipe_fd[1], buffer_message, strlen(buffer_message));

  read_message(pipe_fd[0], buffer_message, nb_char_read_first);
  print_message(buffer_message);

  pere = fork();

  if(pere < 0){perror("fork"); exit(1);}

  if(pere == 0){
    int last_nb_char_read = get_last_read_char(size_string, nb_char_read_first);
    read_message(pipe_fd[0], buffer_message, last_nb_char_read);
    print_message(buffer_message);
  }

  return 0;
}
