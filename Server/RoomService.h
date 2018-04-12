#ifndef ROOM_SERVICE_H
#define ROOM_SERVICE_H
#include "AccountService.h"
#define FREE_ROOM_NOT_FOUND 1



typedef struct
{
    short capacity;
    short currentsize;
    AccountData* playersips;
} Room;

int findFreeRoomForAccount(Room* rooms, int numberofrooms, char* ip);
void connectAccountToRoom(Room* room, char* ip);
void createRoomForAccount(Room* rooms, int numberofrooms, char* ip);

#endif // ROOM_SERVICE_H
