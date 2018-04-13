#include <stdlib.h>
#include <stdio.h>

#include "Server.h"

int main()
{
    int serversocket;
    int serverport;
    struct sockaddr_in serveraddress;
     serveraddress.sin_family = AF_INET;
    initServer(&serversocket, &serverport, &serveraddress);
    startServer(serversocket, serverport, serveraddress);

    return 0;
}
