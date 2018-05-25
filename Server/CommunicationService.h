#ifndef PRIVATE_COMMUNICATION_SERVICE_H
#define PRIVATE_COMMUNICATION_SERVICE_H

#include <sys/types.h>
#include <sys/socket.h>

#include "AccountService.h"
#include "RoomService.h"
#include "Defines.h"
#include "Server.h"

//function to send message from player to player (1 to 1)
int sendMessageToPlayerService(char* login, char* message);

//function to be sure that every character in message got sent
int send_all(int socket, char *buffer, size_t length);

//function to send message to all players in room (1 to multiple)
int sendMessageToRoomService(int accountid, int roomid, char* message);

#endif // PRIVATE_COMMUNICATION_SERVICE_H
