#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "Server.h"


int main()
{

    int serversocket;
    int serverport;
    struct sockaddr_in serveraddress;
    initServer(&serversocket, &serverport, &serveraddress);
    startServer(serversocket, serverport, serveraddress);

    return 0;
}
