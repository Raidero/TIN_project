#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#include "Queue.h"
#include "MenuService.h"
#include "CommunicationService.h"
#include "Multicast.h"

#define DEFAULT_PORT 8001
#define DEFAULT_IP "10.0.2.15"

pthread_t threads[MAX_THREADS_COUNT];
int sockets[MAX_SOCKETS_COUNT];
int communicationsockets[MAX_SOCKETS_COUNT];

int initServer(struct sockaddr_in* serveraddress, struct sockaddr_in* servercommunicationaddress);
int startServer();
void intHandler(int sig);
void* services(void *i);
void copyBuffer(unsigned char* source, unsigned char* destination, int size);
int createNewThread(int* socket);
int createNewSocket(); //return index of first free socket, it will be playerid
int socketToPlayerId(int socket);
int disposeSocket(int socket);
#endif // SERVER_H
