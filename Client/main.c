#include <stdio.h>
#include <stdlib.h>

#include "Client.h"

int main()
{
    int clientsocket;
    short clientport;
    struct sockaddr_in serveraddress;

    initClient(&clientsocket, &clientport, &serveraddress);

    startClient(clientsocket, serveraddress);

    return 0;
}
