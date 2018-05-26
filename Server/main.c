#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "Server.h"

int main()
{
    struct sockaddr_in serveraddress;
    struct sockaddr_in servercommunicationaddress;
    initAccountService();
    initRoomService();
    initQueue();
    initServer(&serveraddress, &servercommunicationaddress);
    startServer();

    return 0;
}
