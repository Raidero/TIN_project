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
#include "Defines.h"




#define DEFAULT_PORT 8001
#define DEFAULT_IP "10.0.2.15"

int initClient(int* serversocketfd, struct sockaddr_in* serveraddress);
int startClient(int serversocketfd, struct sockaddr_in serveraddress);

#endif // CLIENT_H
