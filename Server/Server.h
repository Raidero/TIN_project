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

#include "MenuService.h"
//#include "PrivateCommunicationService.h"

#define ERROR_OPENING_SOCKET 1
#define ERROR_BINDING_SOCKET 2
#define ERROR_STARTING_SERVER 3
#define ERROR_CREATING_THREAD 4
#define ERROR_SETTING_SOCKET_OPTIONS 5
#define MAX_CONNECTION_LIMIT 128

#define DEFAULT_PORT 8001
#define DEFAULT_IP "10.0.2.15"

int initServer(int* portnumber, struct sockaddr_in* serveraddress);
int startServer(int portnumber, struct sockaddr_in serveraddress);
void intHandler(int sig);
void* services();
pthread_t* requestNewMemory(pthread_t* threads, int numberofthreads);

#endif // SERVER_H
