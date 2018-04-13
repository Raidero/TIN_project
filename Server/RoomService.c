#include "RoomService.h"
#include <stdio.h>
#include <string.h>
int findFreeRoomForAccount(Room* rooms, AccountData* playerdata)
{
    int i;

    for(i = 0; i < MAX_ROOM_COUNT; ++i)
    {
        if(rooms[i].currentsize < rooms[i].MAX_PLAYER_COUNT)
        {
			// mutex
            connectAccountToRoom(&rooms[i], playerdata);
            return 0;
        }
    }
    return FREE_ROOM_NOT_FOUND;
}

int connectAccountToRoom(Room* room, AccountData* playerdata)
{
    room->players[room->currentsize++] = playerdata;

	return 0;
}

int createRoomForAccount(Room* rooms, char* ip)
{
	return 0;
}

int refreshRoom(Room* room, char* ip)
{
	return 0;
}
