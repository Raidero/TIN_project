#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "Server.h"

int main()
{
    int serverport;
    struct sockaddr_in serveraddress;
    initServer(&serverport, &serveraddress);
    startServer(serverport, serveraddress);

    return 0;
}
