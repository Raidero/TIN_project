#include "RoomService.h"
#include <stdio.h>
#include <string.h>

Room* rooms[MAX_ROOM_COUNT];

int findFreeRoomForAccount(AccountData* playerdata)
{
    uint16_t i, j;

    for(i = 0; i < MAX_ROOM_COUNT; ++i)
    {
        for(j = 0; j < MAX_PLAYER_COUNT; ++j)
        {
            if(rooms[i]->players[j] == NULL)
            {
                connectAccountToRoom(rooms[i], playerdata, j);
                return 0;
            }
        }
    }
    return FREE_ROOM_NOT_FOUND;
}

int connectAccountToRoom(Room* room, AccountData* playerdata, uint16_t index)
{
    //lock
    room->players[index] = playerdata; //many players can try to take place in room at once
    //unlock
	return 0;
}

int createRoomForAccount(AccountData* playerdata)
{
	return 0;
}

int refreshRoom(uint16_t roomid) // have to check if id is not out of range of max possible id to don't check out of table range data
{
	return 0;
}
