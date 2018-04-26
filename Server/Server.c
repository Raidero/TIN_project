#include "Server.h"
#include "Defines.h"

int* newclientsocketfd;
pthread_t newClientThread;
int serversocketfd;

int initServer(int* portnumber, struct sockaddr_in* serveraddress)
{
    /*first call to socket function*/
    int option = 1;
    serversocketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serversocketfd < 0)
    {
        fprintf(stderr, "Couldn't open a socket\n");
        return ERROR_OPENING_SOCKET;
    }
    if(setsockopt(serversocketfd, SOL_SOCKET, SO_REUSEADDR, (char*)&option, sizeof(option)) < 0)
    {
        fprintf(stderr, "setsockopt failed\n");
        return ERROR_SETTING_SOCKET_OPTIONS;
    }
    bzero((char *) serveraddress, sizeof(*serveraddress));
    *portnumber = DEFAULT_PORT;

    /*initialize socket structure*/
    serveraddress->sin_family = AF_INET;
    serveraddress->sin_addr.s_addr = INADDR_ANY;
    serveraddress->sin_port = htons(DEFAULT_PORT);

    /*bind host address using bind call*/
    if(bind(serversocketfd, (struct sockaddr *) serveraddress, sizeof(*serveraddress)) < 0)
    {
        fprintf(stderr, "Couldn't bind server to address\n");
        return ERROR_BINDING_SOCKET;
    }
    listen(serversocketfd, MAX_CONNECTION_LIMIT);
    return 0;
}

int startServer(int portnumber, struct sockaddr_in server_address)
{
    struct sockaddr_in client_address;
    unsigned int client_length = sizeof(client_address);
    signal(SIGINT, intHandler);

    while(1)
    {
        newclientsocketfd = (int*)malloc(sizeof(int));
        *newclientsocketfd = accept(serversocketfd, (struct sockaddr *) &client_address, &client_length);
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

    }
    return 0;
}

void intHandler(int sig_num)
{
    if(newClientThread != NULL)
    {
        pthread_kill(newClientThread, 0);
        if(newclientsocketfd != NULL)
            close(*newclientsocketfd);
    }
    close(serversocketfd);
    exit(0);
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
        if (write(socket, "got your message", 17)==-1)
            break;
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
