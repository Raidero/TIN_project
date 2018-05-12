#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "Server.h"

int main()
{
    struct sockaddr_in serveraddress;
    initAccountService();
    initRoomService();
    initQueue();
    initServer(&serveraddress);
    startServer(serveraddress);

    return 0;
}
