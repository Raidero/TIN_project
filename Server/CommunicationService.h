#ifndef PRIVATE_COMMUNICATION_SERVICE_H
#define PRIVATE_COMMUNICATION_SERVICE_H

#include <sys/types.h>
#include <sys/socket.h>

#include "AccountService.h"
#include "RoomService.h"
#include "Defines.h"

int sendMessageToPlayerService(char* login, char* message);
int send_all(int socket, char *buffer, size_t length);

int sendMessageToRoomService(char* senderlogin, int roomid, char* message);

#endif // PRIVATE_COMMUNICATION_SERVICE_H
