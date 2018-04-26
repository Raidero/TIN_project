#include "Client.h"

int initClient(int* clientsocketfd, short* portnumber, struct sockaddr_in* serveraddress)
{
    struct hostent *server;

    /*first call to socket function*/
    *clientsocketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientsocketfd < 0)
    {
        fprintf(stderr, "ERROR opening socket\n");
        return ERROR_OPENING_SOCKET;
    }

    server = gethostbyname(DEFAULT_IP);

    if (server == NULL)
    {
        fprintf(stderr,"ERROR, no such host\n");
        return ERROR_GETTING_HOST_NAME;
    }
    bzero((char *) serveraddress, sizeof(*serveraddress));
    *portnumber = DEFAULT_PORT;

    /*initialize socket structure*/
    serveraddress->sin_family = AF_INET;
    inet_pton(AF_INET, DEFAULT_IP, &serveraddress->sin_addr.s_addr);
    serveraddress->sin_port = htons(DEFAULT_PORT);
    return 0;
}
int startClient(int clientsocketfd, struct sockaddr_in serveraddress)
{
    int n;
    char buffer[256];

    /*connect client to server using connect call*/
    if (connect(clientsocketfd,(struct sockaddr *) &serveraddress, sizeof(serveraddress)) < 0)
    {
        fprintf(stderr, "Couldn't connect client to server\n");
        return ERROR_CONNECTING_CLIENT;
    }
    printf("Please enter the message: ");
    while(1)
    {

        bzero(buffer, 256);
        fgets(buffer, 255, stdin);
        n = write(clientsocketfd, buffer, strlen(buffer));
        if (n < 0)
        {
            fprintf(stderr, "Couldn't write to socket");
            return ERROR_WRITING_TO_SOCKET;
        }
        bzero(buffer, 256);
        n = read(clientsocketfd, buffer, 255);
        if (n < 0)
        {
            fprintf(stderr, "Couldn't read from socket");
            return ERROR_READING_FROM_SOCKET;
        }
        printf("%s\n", buffer);
    }
    close(clientsocketfd); // added
    exit(0); // added
    return 0;
}
