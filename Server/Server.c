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
        fprintf(stderr, "Couldn't open a socket\n");
        return ERROR_OPENING_SOCKET;
    }
    bzero((char *) serveraddress, sizeof(*serveraddress));
    *portnumber = DEFAULT_PORT;
    /*initialize socket structure*/
    serveraddress->sin_family = AF_INET;
    inet_pton(AF_INET, DEFAULT_IP, &serveraddress->sin_addr.s_addr);
    serveraddress->sin_port = htons(DEFAULT_PORT);

    /*bind host address using bind call*/
    if(bind(*serversocketfd, (struct sockaddr *) serveraddress, sizeof(*serveraddress)) < 0)
    {
        fprintf(stderr, "Couldn't bind server to address\n");
        return ERROR_BINDING_SOCKET;
    }
    listen(*serversocketfd, MAX_CONNECTION_LIMIT);
    return 0;

}

int startServer(int serversocketfd, int portnumber, struct sockaddr_in server_address)
{
    struct sockaddr_in client_address;
    unsigned int client_length = sizeof(client_address);
    int* newclientsocketfd;
    pthread_t newClientThread;

    while(1)
    {
        newclientsocketfd = (int*)malloc(sizeof(int));
        *newclientsocketfd = accept(serversocketfd, (struct sockaddr *) &client_address, &client_length);
        /*printf("xd");
        fflush(stdout);
        char b[256];
        char a[2] = { 'a', 'n'};
        read(newclientsocketfd, b, 256);
        printf("%s", b);
        fflush(stdout);
        write(newclientsocketfd, a, 2);*/

        if(*newclientsocketfd < 0)
        {
            fprintf(stderr, "Couldn't start server to address\n");
            //return ERROR_STARTING_SERVER;
        }
        if (pthread_create(&newClientThread, NULL, services, newclientsocketfd))
        {
            fprintf(stderr, "Couldn't create new thread");
            return ERROR_CREATING_THREAD;
        }
        else
        {
            //close(*newclientsocketfd);
        }
    }
}

void* services(void *i)
{
    int socket = *((int *)i);
    printf("Services function");
    char b[256];
    while(1)
    {
        read(socket, b, 256);
        printf("%s", b);
        write(socket, "got your message", 17);
    }

}


