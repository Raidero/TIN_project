#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define ERROR_OPENING_SOCKET 1
#define ERROR_GETTING_HOST_NAME 2
#define ERROR_CONNECTING_CLIENT 3
#define ERROR_WRITING_TO_SOCKET 4
#define ERROR_READING_FROM_SOCKET 5

#define DEFAULT_PORT 8001
#define DEFAULT_IP "10.0.2.15"

int initClient(int* serversocketfd, short* portnumber, struct sockaddr_in* serveraddress);
int startClient(int serversocketfd, struct sockaddr_in serveraddress);

#endif // CLIENT_H
