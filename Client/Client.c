#include "Client.h"

int initSocket(int* clientsocketfd, struct sockaddr_in* serveraddress, struct timeval* timeout)
{
    struct hostent *server;

    /*first call to socket function*/
    *clientsocketfd = socket(AF_INET, SOCK_STREAM, 0);

    if (clientsocketfd < 0)
    {
        fprintf(stderr, "ERROR opening socket\n");
        return ERROR_OPENING_SOCKET;
    }

    timeout->tv_sec = 3;
    timeout->tv_usec = 0;
    setsockopt(*clientsocketfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)timeout, sizeof(struct timeval));

    server = gethostbyname(DEFAULT_IP);

    if (server == NULL)
    {
        fprintf(stderr,"ERROR, no such host\n");
        return ERROR_GETTING_HOST_NAME;
    }
    bzero((char *) serveraddress, sizeof(*serveraddress));

    /*initialize socket structure*/
    serveraddress->sin_family = AF_INET;
    inet_pton(AF_INET, DEFAULT_IP, &serveraddress->sin_addr.s_addr);
    serveraddress->sin_port = htons(DEFAULT_PORT);
    return 0;
}
int startClient(int clientsocketfd, struct sockaddr_in serveraddress)
{
    //int n;
    //char buffer[BUFFER_SIZE];

    /*connect client to server using connect call*/
    if (connect(clientsocketfd,(struct sockaddr *) &serveraddress, sizeof(serveraddress)) < 0)
    {
        fprintf(stderr, "Couldn't connect client to server\n");
        return ERROR_CONNECTING_CLIENT;
    }
    /*printf("Please enter the message: ");
    while(1)
    {

        bzero(buffer, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, stdin);
        n = write(clientsocketfd, buffer, strlen(buffer));
        if (n < 0)
        {
            fprintf(stderr, "Couldn't write to socket");
            return ERROR_WRITING_TO_SOCKET;
        }
        bzero(buffer, BUFFER_SIZE);
        n = read(clientsocketfd, buffer, BUFFER_SIZE);
        if (n < 0)
        {
            fprintf(stderr, "Couldn't read from socket");
            return ERROR_READING_FROM_SOCKET;
        }
        printf("%s\n", buffer);
    }
    close(clientsocketfd); // added
    exit(0); // added*/
    return 0;
}


