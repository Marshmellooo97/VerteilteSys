#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


int running = 1;
int turn = 0;
char str[128];

void* client(void* _arg){
    while (running){
        while (turn == 1){
            sleep(1);
        }
        
        printf("Eingabe\n");
        scanf("%s",str);
        turn = 1;
        if(strcmp(str,"quit") == 0) //strcmp gibt die Sotirung zurück
            running = 0;
    }
    return 0;
}
void* server(void* _arg){
    while (running) {
        while (turn == 0){
            sleep(1);
        }
         printf("Server: %s\n",str);
         turn = 0;
    }
    return 0;    
}
int main(int argc, char** argv){
    pthread_t cl;
    pthread_t se;

    pthread_create(&cl, NULL, client, NULL);
    pthread_create(&se, NULL, server, NULL);
    pthread_join(cl, NULL);
    pthread_join(se, NULL);
    return 0;
}


