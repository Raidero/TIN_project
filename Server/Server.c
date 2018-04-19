#include "Server.h"

/*#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>*/
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
    //char b[256]; - we problably will need that later for checking if message isn't "exit"
    //pthread_t threadsVector[]; - we will need that later

    while(1)
    {
        newclientsocketfd = (int*)malloc(sizeof(int));
        *newclientsocketfd = accept(serversocketfd, (struct sockaddr *) &client_address, &client_length);

        //char b[256];
        //char a[2] = { 'a', 'n'};
        //read(newclientsocketfd, b, 256);
        //printf("%s", b);
        //fflush(stdout);
        //write(newclientsocketfd, a, 2);

        if(*newclientsocketfd < 0)
        {
            fprintf(stderr, "Couldn't start server\n");
            return ERROR_STARTING_SERVER;
        }
        if (pthread_create(&newClientThread, NULL, services, newclientsocketfd))
        {
            fprintf(stderr, "Couldn't create new thread");
            return ERROR_CREATING_THREAD;

        }

        /*if(strcmp(read(newclientsocketfd, b, 256), "exit") == 0)
            {
                printf("Closing socket");
                //pthread_join(newClientThread, NULL);
                close(*newclientsocketfd); // should stay this way for now
            }*/
    }
    pthread_join(newClientThread, NULL);
    close(serversocketfd);
}

void* services(void *i)
{
    int socket = *((int *)i);
    printf("Services function\n");
    char b[256];
    while(1)
    {
        bzero(b, 256);
        read(socket, b, 256);
        printf("%s", b);
        bzero(b, 256);
        write(socket, "got your message", 17);
    }
}

pthread_t* requestNewMemory(pthread_t* threads, int numberofthreads)
{
    pthread_t* temp;
    int i;
    numberofthreads <<=1;
    if(threads!=NULL)
    {
        temp = (pthread_t*)malloc(numberofthreads*sizeof(pthread_t));
        for(i = 0; threads[i]; i++)
        {
            temp[i]=threads[i];
            free(&threads[i]);

        }
    }
    threads = (pthread_t*)malloc(numberofthreads * sizeof(pthread_t));
    for(i = 0; temp[i]; i++)
    {
        threads[i]=temp[i];
    }
    return threads;
}

