#include "Server.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>
#include "Defines.h"


int initServer(int* serversocketfd, int* portnumber, struct sockaddr_in* serveraddress)
{
    /*first call to socket function*/
    *serversocketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serversocketfd < 0)
    {
        fprintf(stderr, "Couldn't open a socket");
        return ERROR_OPENING_SOCKET;
    }
    bzero((char *) serveraddress, sizeof(*serveraddress));
    *portnumber = 5001;
    /*initialize socket structure*/
    serveraddress->sin_family = AF_INET;
    serveraddress->sin_addr.s_addr = INADDR_ANY;

    serveraddress->sin_port = htons(*portnumber);

    /*bind host addresss using bind call*/
    if(bind(*serversocketfd, (struct sockaddr *) serveraddress, sizeof(*serveraddress)) < 0)
    {
        fprintf(stderr, "Couldn't bind server to address");
        return ERROR_BINDING_SOCKET;
    }
    listen(*serversocketfd, MAX_CONNECTION_LIMIT);
    return 0;

}

int startServer(int serversocketfd, int portnumber, struct sockaddr_in server_address)
{
    struct sockaddr_in client_address;
    unsigned int client_length = sizeof(client_address);
    int newclientsocketfd;
    pthread_t newClientThread;

    while(1)
    {
        newclientsocketfd = accept(serversocketfd, (struct sockaddr *) &client_address, &client_length);
        if (newclientsocketfd < 0)
        {
            fprintf(stderr, "Couldn't start server to address");
            return ERROR_STARTING_SERVER;
        }
        if (pthread_create(&newClientThread, NULL, services, NULL))
        {
            fprintf(stderr, "Couldn't create new thread");
            return ERROR_CREATING_THREAD;
        }
        else
        {
            close(newclientsocketfd);
        }
    }
}

void* services()
{
    printf("Services function");
    return NULL;
}


