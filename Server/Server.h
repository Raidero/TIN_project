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
//#include "PrivateCommunicationService.h"

#define DEFAULT_PORT 8001
#define DEFAULT_IP "10.0.2.15"

pthread_t threads[MAX_THREADS_COUNT];

int initServer(struct sockaddr_in* serveraddress);
int startServer(struct sockaddr_in serveraddress);
void intHandler(int sig);
void* services(void *i);
int createNewThread(int* socket);

#endif // SERVER_H
