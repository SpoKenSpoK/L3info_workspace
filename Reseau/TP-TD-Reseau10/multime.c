#include "bor-util.h"
#include "bor-timer.h"


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
    int timerOne, timerTwo, timerThree, timerFour;
    int currentTimer;
    int result;

    timerOne = bor_timer_add(2000, NULL);
    timerTwo = bor_timer_add(5000, NULL);
    timerThree = bor_timer_add(10000, NULL);
    timerFour = bor_timer_add(20000, NULL);

    for(;;){
        result = select(0, NULL, NULL, NULL, bor_timer_delay());
        if( result < 0 ){
            perror("Erreur : select fail\n");
            break;
        }

        currentTimer = bor_timer_handle();
        if( currentTimer < 0 ){
            perror("bor_timer_handle() < 0\n");
            break;
        }

        else if( currentTimer == timerOne ){
            timerOne = bor_timer_add(2000, NULL);
            printf("- 2 Seconds - \n");
        }

        else if( currentTimer == timerTwo ) printf("- 5 Seconds -\n");
        else if( currentTimer == timerThree ) printf("- 10 Seconds -\n");

        else if( currentTimer == timerFour ){
            printf("- 20 Seconds -\nShutdown\n");
            break;
        }

        else printf("\ntimer non defini\n");

        bor_timer_remove( currentTimer );
    }

    return 0;
}
