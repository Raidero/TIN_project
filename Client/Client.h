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
#include <errno.h>
#include "Defines.h"

#define DEFAULT_PORT 8001
#define DEFAULT_IP "10.0.2.15"

int initSocket(int* serversocketfd, struct sockaddr_in* serveraddress, struct timeval* timeout);
int startClient(int serversocketfd, struct sockaddr_in serveraddress);
int joinMulticastGroup(int* recvsocketfd, struct sockaddr_in* localsock, const char* mcastgroupaddr, uint16_t mcastgroupport, const char* myinterfaceaddr, int reuse);	// this is for the client..


#endif // CLIENT_H
