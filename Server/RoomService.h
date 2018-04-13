#ifndef ROOM_SERVICE_H
#define ROOM_SERVICE_H

#include "AccountService.h"

#define FREE_ROOM_NOT_FOUND 1
#define MAX_ROOM_COUNT 16
#define MAX_PLAYER_COUNT 16



typedef struct
{
    //short capacity;
    short currentsize;
    AccountData* players[MAX_PLAYER_COUNT];
} Room;

int findFreeRoomForAccount(Room* rooms, AccountData* playerdata);
int connectAccountToRoom(Room* room, AccountData* playerdata);
int createRoomForAccount(Room* rooms, char* ip);

#endif // ROOM_SERVICE_H
