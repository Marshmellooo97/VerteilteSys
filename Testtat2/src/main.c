#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include "Client.c"
#include "Server.c"
#include "threads.h"

#define MAX_NUM_CONNECTIONS 50
#define IP "127.0.0.1"
#define START_CLIENT_PORTS 4667
#define SIZE_MESSAGE 80
#define MAX_CLIENTS 5



int main(void) {

    pthread_t se;

    pthread_create(&se, NULL, serverMainThread, &glob_servers);

    char usrInputBuffer[SIZE_MESSAGE + 1] = {0};
    
    sleep(3);

    while (true) {
        printf("type a command: ");
        fgets(usrInputBuffer, sizeof(usrInputBuffer), stdin);

        if (usrInputBuffer[0] == 'q' || usrInputBuffer[0] == 'Q') {
            printf("program has ended");
            break;
        }
        else if (usrInputBuffer[0] == 'c' || usrInputBuffer[0] == 'C') {
            int freeSlot = thr_findFreeClientSlot(glob_clients, MAX_CLIENTS);
            if (freeSlot != -1) {
                printf("provide an ip addresss: ");
                bzero(usrInputBuffer, sizeof(usrInputBuffer));
                fgets(glob_clients[freeSlot].remoteIp, 15, stdin);
                glob_clients[freeSlot].localPort = freeSlot + START_CLIENT_PORTS;
                memcmp(glob_clients[freeSlot].remoteIp, usrInputBuffer, 15);
                pthread_mutex_lock(&_mut);
                glob_clients[freeSlot].isActive = true;
                pthread_mutex_unlock(&_mut);
                cli_establishConnection(&glob_clients[freeSlot]);
            } else {
                printf("maximum number of clients reached\n");
            }
        }
        else if (usrInputBuffer[0] == 'd' || usrInputBuffer[0] == 'D') {
            printf("connection to wich client do you like to end: ");
            fgets(usrInputBuffer, sizeof(usrInputBuffer), stdin);
            if (isdigit(usrInputBuffer[0])) {
                int clientID = atoi(usrInputBuffer);
                printf("end connection with client %d\n", clientID);
                thr_stopClientThread(&glob_clients[clientID]);
            } else {
                fprintf(stderr, "wrong input, expect a digit\n");
            }
        }
        else if (isdigit(usrInputBuffer[0])) {
            int id = atoi(usrInputBuffer);

            if (id < CLIENTS_MAX_NUM && thr_isClientActive(&glob_clients[id])) {
                printf("message for client %d: \n", id);
                bzero(usrInputBuffer, sizeof(usrInputBuffer));
                fgets(usrInputBuffer, sizeof(usrInputBuffer), stdin);
                pthread_mutex_lock(&_mut);
                memcpy(glob_clients[id].str128, usrInputBuffer, 80);
                glob_clients[id].hasMessage = true;
                pthread_mutex_unlock(&_mut);
            } else {
                printf("client ID doens't exist\n");
                continue;
            }
        }
        else if (usrInputBuffer[0] == '\n') {
            continue;
        }
        else {
            fprintf(stderr, "wrong input, expect q, c, d, or number\n");
        }
    };
}