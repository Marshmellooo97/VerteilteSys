#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

#define NUM_THREADS 51
pthread_t threads[NUM_THREADS];
int connections[NUM_THREADS];
#define MESG_SIZE 80
#define SA struct sockaddr
struct sockaddr_in servaddr, client;

const int Distance = 'a'-'A';

void ser_changeCase(char* _str) {
	while (*_str != 0) {
		if(*_str >= 'a'  && *_str <= 'z') {
			*_str -= Distance;
		} else if(*_str >= 'A'  && *_str <= 'Z') {
			*_str += Distance;
		}
		_str++;
	}
}	

void ser_serverFunction(int sockfd) {
	char buffer[MESG_SIZE];

	// infinite loop for chat
	while (1) {
		// clear buffer
		bzero(buffer, MESG_SIZE);
		// read the message from client and copy it in buffer
		read(sockfd, buffer, sizeof(buffer));
		// exchange upper-case letters by lower-case letter and vice versa.
		ser_changeCase(buffer);

		// and send that buffer to client
		write(sockfd, buffer, sizeof(buffer));

		// if msg contains "QUIT" then server exit and chat ended.
		if (strncmp("QUIT", buffer, 4) == 0) {
			printf("Server Exit...\n");
			break;
		}
	}
}

int ser_setupConnectionfirst(int _port) {
	int sockfd, connfd;
	printf("--setupConnectionfirst--beginn--\n");
	int serverSocket, newSocket;
	struct sockaddr_storage serverStorage;
	const int y = 1;
	socklen_t addr_size;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		fprintf(stderr, "Error: Cannot create socket --- exit\n");
		exit(3);
	} else {
		printf("Socket created.\n");
	}
	bzero(&servaddr, sizeof(servaddr));

	// set up socket
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(_port);

	// Binding socket to any IP
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		fprintf(stderr, "Error: Cannot bind socket --- exit\n");
		exit(4);
	} else {
		printf("Socket bound.\n");
	}
	printf("--setupConnectionfirst--end--%d\n",sockfd);
	return sockfd;
}

int ser_setupConnectionsecond(int sockfd) {
	printf("--setupConnectionsecond--beginn--%d\n",sockfd);
    int connfd, len;
    //struct sockaddr_in servaddr, client;

    // Server listens
	if ((listen(sockfd, 60)) != 0) {
		fprintf(stderr, "Error: Cannot listen --- exit\n");
	    exit(5);
	} else { 
		printf("Server listening.\n");
	}
	len = sizeof(client);

	// Accept data packet from client

	connfd = accept(sockfd, (SA*)&client, &len);

	if (connfd < 0) {
		fprintf(stderr, "Error: Server accept failed --- exit\n");
		exit(6);
	} else {
		printf("Server accept client.\n");
	}
	printf("--setupConnectionsecond--end--\n");
    return connfd;
}

int ser_firstTouch(int sockfd, int _port) {
	int connfd = 0;
	printf("--firstTouch--beginn--%d\n",sockfd);
	//int sockfd = setupConnectionfirst(_port); 
    connfd = ser_setupConnectionsecond(sockfd);
	char buffer[MESG_SIZE];
	bzero(buffer, MESG_SIZE);
	read(connfd, buffer, MESG_SIZE);
	printf("received: %s\n", buffer);
	((int*)buffer)[0] = _port + 1;
	write(connfd, buffer, MESG_SIZE);
	close(connfd);
    //int sockfdForPermanet = setupConnectionfirst(_port + 1); 
	printf("--firstTouch--end--\n");
	return connfd;
}
void *connectionWorkers(void* workerId){
	printf("--permantConnection--serverFunktion--\n");
	int _workerId = (int)workerId;
	int _connfd = 0;
	while (1){
		do{
			//pthread_sleep(10);
			_connfd = connections[_workerId];
		}while(_connfd == -1);
		ser_serverFunction(_connfd);
		printf("--permantConnection--end--\n");
		close(_connfd);
		printf("--permantConnection--end--end--\n");
		connections[_workerId] = -1;
	}
}

int ser_permantConnection(int _sockfd) {
	int connfd = 0;
	printf("--permantConnection--beginn--%d\n",_sockfd);
	//int _sockfd = setupConnectionfirst(_port); 
    connfd = ser_setupConnectionsecond(_sockfd);
	return connfd;
#if 0
	printf("--permantConnection--serverFunktion--\n");
	serverFunction(connfd);
	printf("--permantConnection--end--\n");
	close(connfd);
	printf("--permantConnection--end--end--\n");
#endif
	
}



void* serverMainThread(void* nothing) {
	int port = 4567;

    int sockfd = ser_setupConnectionfirst(port);
	int sockfd2 = ser_setupConnectionfirst(port + 1);
	for (int i = 0; i<NUM_THREADS; i++){
		connections[i] = -1;
		pthread_create(&threads[i], NULL, connectionWorkers, (void*)i);
	}
	int t = 0;
   	while(1) {
		ser_firstTouch(sockfd, port);
		int connfd = ser_permantConnection(sockfd2);
		int coonectionFree = 0;
			for (int iIdx = 0; iIdx< NUM_THREADS; iIdx++){
				if (connections[iIdx] == -1) {
					connections[iIdx] = connfd;
					coonectionFree = 1;
					printf("run iIdx%d\n", iIdx);
					break;
				}
			}
			if (coonectionFree == 0){
				close(connfd);
			}
	}
	return 0;
}

