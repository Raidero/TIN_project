#ifndef SERVER_INIT_H
#define SERVER_INIT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include "Defines.h"


void initServer(int* serversocketfd, int* portnumber, struct sockaddr_in* server_address)
{
    /*first call to socket function*/
    serversocketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serversocketfd < 0)
    {
        fprintf(stderr, "Couldn't open a socket");
        return ERROR_OPENING_SOCKET;
    }
    bzero((char *) &server_address, sizeof(server_address));
    portnumber = 5001;

    /*initialize socket structure*/
    server_address->sin_family = AF_INET;
    server_address->sin_addr.s_addr = INADDR_ANY;
    server_address->sin_port = htons(portnumber);

    /*bind host addresss using bind call*/
    if(bind(serversocketfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
    {
        fprintf(stderr, "Couldn't bind server to address");
        return ERROR_BINDING_SOCKET;
    }
    listen(serversocketfd, MAX_CONNECTION_LIMIT);
    return 0;

}

void serverStart(int* serversocketfd, int* portnumber, struct sockaddr_in* server_address)
{
    struct sockaddr_in client_address;
    int client_length = sizeof(client_address);
    int newclientsocketfd;
    while(1)
    {
        newclientsocketfd = accept(serversocketfd, (struct sockaddr *) &client_address, &client_length);
        if (newclientsocketfd < 0)
        {
            fprintf(stderr, "Couldn't start server to address");
            return ERROR_STARTING_SERVER;
        }
    }
}

#endif // SERVER_INIT_H
