#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

//#include <sleep>

char buffer[128];
int running = 1;
int newData = 0;
// server 8677
void* client(void* _arg) {
    while (running != 0) {
        while (newData == 1) 
            sleep(1);
        
        printf("gib was ein: ");
        scanf("%s", buffer);
        if (strcmp(buffer, "quit") == 0)
            running = 0;
        newData = 1;
    }
    return 0;
}

void* server(void* _arg) {
    while (running != 0) {
        while (newData == 0) 
            sleep(1);

        printf("newData: %s\n", buffer);
        newData = 0;
        

    }
}

int main(int argc, char** argv){
    int numberOfThreads = 2;
    if (argc > 2) {
        printf("Error: usage: %s <port> --- exit\n", argv[0]);
    }
    if (argc == 2) {
        numberOfThreads = atoi(argv[1]);
    }
    printf("Number of Threads: %d \n", numberOfThreads);

    pthread_t threads[numberOfThreads];

    pthread_create(&threads[0], NULL, client, NULL);
    pthread_create(&threads[1], NULL, server, NULL);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    return 0;
}
//     gcc .\VerteiltesSystemServerClient.c -o .\VerteiltesSystemServerClient.exe -lpthread