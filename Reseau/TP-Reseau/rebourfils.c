#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// include .h
#include "bor-util.h"
#include "bor-timer.h"

/*unsigned int saisir_nombre(){
    char number[10];
	unsigned int nombre = 0;
    char* fin = NULL;

	printf("Saisir un nombre: ");
    fgets(number, sizeof(number), stdin);
    nombre = strtoul(number, &fin, 10);

    return nombre;
}*/

/*
    Usage :

    int h1, h2, res;

    h1 = bor_timer_add (delai1, data1);
    h2 = bor_timer_add (delai2, data2);

    res = select (..., bor_timer_delay());

    if (res == 0) {
        handle = bor_timer_handle();
        data = bor_timer_data();

        if (handle == h1) {
            printf ("delai1 ecoule");
        } else if (handle == h2) {
            printf ("delai2 ecoule");
        }

        bor_timer_remove (handle);
    }

*/

int main(){
    unsigned int number = 0;
    //pid_t father = getpid();
    printf("Entrez un nombre : \n");
    /*printf("%d \n",scanf("%u",&number));*/
    scanf("%u",&number);

    for(unsigned int i=0; i < number; ++i){
        if(fork()==0){
            bor_timer_add(10);
            //exit(0);
        }
    }

    //maxValue = number;
    /*for(unsigned int i = 0; i < number; i++) {
        int pid = fork();
        if(pid < 0) {
            printf("Error");
            exit(1);
        } else if (pid == 0) {
            printf("Child (%d): %d\n", i + 1, getpid());
            exit(0);
        } else  {
            wait(NULL);
        }
    }*/




    //make_a_son(0);
    return 0;
}
