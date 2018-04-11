#ifndef ROOM_SERVICE_GUARD
#define ROOM_SERVICE_GUARD
#include <stdio.h>

#define FREE_ROOM_NOT_FOUND 1

struct Room
{
    short capacity;
    short currentsize;
    char** playersips;
};

int findFreeRoomForAccount(struct Room* rooms, int numberofrooms, char* ip);
void connectAccountToRoom(struct Room* room, char* ip);
void createRoomForAccount(struct Room* rooms, int numberofrooms, char* ip);

#endif // ROOM_SERVICE_GUARD
