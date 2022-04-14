
#pragma once

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

#define SERVERS_MAX_NUM 50
#define CLIENTS_MAX_NUM 5



static pthread_mutex_t _mut;

typedef union Message {
    char str128[128];
} Message;

typedef struct Mailbox {
    bool hasMessage;
    Message message;
} Mailbox;

typedef struct Server {
    bool isActive;
    Mailbox mailbox;
    int port;
    int sockfd;
} Server;

typedef struct Client {
    bool isActive;
    char str128[128];
    bool hasMessage;
    int localPort;
    int sockfd;
    char remoteIp[15];
    int remotePort;
} Client;

typedef union Thread {
    Client client;
    Server server;
} Thread;

Server glob_servers[SERVERS_MAX_NUM];
Client glob_clients[CLIENTS_MAX_NUM];


bool thr_isClientActive(Client* client_ptr) {
    bool isClientActive = false;
    pthread_mutex_lock(&_mut);
    isClientActive = client_ptr->isActive;
    pthread_mutex_unlock(&_mut);
    return isClientActive;
}



void thr_stopClientThread(Client* client_ptr) {
    pthread_mutex_lock(&_mut);
    client_ptr->isActive = false;
    pthread_mutex_unlock(&_mut);
}


int thr_findFreeServerSlot(Server* servers, int maxNumServers) {
	int result = -1;
    pthread_mutex_lock(&_mut);
    for(int i = 0; i < maxNumServers; i++) {
        if (!servers[i].isActive) {
            result = i;
            break;
        }
    }
    pthread_mutex_unlock(&_mut);
    return result;
}


int thr_findFreeClientSlot(Client* clients, int maxNumClients) {
	int result = -1;
    pthread_mutex_lock(&_mut);
    for(int i = 0; i < maxNumClients; i++) {
        if (!clients[i].isActive) {
            result = i;
            break;
        }
    }
    pthread_mutex_unlock(&_mut);
    return result;
}


bool thr_mailboxHasMessage(Mailbox* mailbox_ptr) {
    pthread_mutex_lock(&_mut);
    bool hasMessage = mailbox_ptr->hasMessage;
    pthread_mutex_unlock(&_mut);
    return hasMessage;
}


Message thr_getMessage(Mailbox* mailbox_ptr) {
    pthread_mutex_lock(&_mut);
    mailbox_ptr->hasMessage = false;
    Message message = mailbox_ptr->message;
    pthread_mutex_unlock(&_mut);
    return message;
}


bool thr_sendMessage(Mailbox* rxMailbox_ptr, Message message) {
    bool is_delivered = false;
    pthread_mutex_lock(&_mut);
    if (!rxMailbox_ptr->hasMessage) {
        rxMailbox_ptr->message = message;
        rxMailbox_ptr->hasMessage = true;
        is_delivered = true;
    } else {
        is_delivered = false;
    }
    pthread_mutex_unlock(&_mut);
    return is_delivered;
}


void thr_spawnThread(void* function_ptr, Thread* thread_ptr) {
    pthread_mutex_lock(&_mut);
    pthread_t se;
    pthread_create(&se, NULL, (function_ptr), (void*)thread_ptr);
    pthread_mutex_unlock(&_mut);
}


