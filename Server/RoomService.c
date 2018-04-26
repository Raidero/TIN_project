#include "RoomService.h"
#include <stdio.h>
#include <string.h>

extern AccountData* loggedaccounts[MAX_ACCOUNTS_COUNT];

Room* rooms[MAX_ROOM_COUNT];

int findFreeRoomForAccount(AccountData* playerdata)
{
    int i, j;

    for(i = 0; i < MAX_ROOM_COUNT; ++i)
    {
        for(j = 0; j < MAX_PLAYER_COUNT; ++j)
        {
            if(rooms[i]->players[j] == NULL)
            {
				if(!rooms[i]->isingame)
				{
					connectAccountToRoom(rooms[i], playerdata, j);
					return i;
				}
            }
        }
    }
    return FREE_ROOM_NOT_FOUND;
}

int connectAccountToRoom(Room* room, AccountData* playerdata, int index)
{
    //lock
    room->players[index] = playerdata; //many players can try to take place in room at once
    room->isplayerready[index] = 0;
    //unlock
	return 0;
}

int createRoomForAccount(AccountData* playerdata)
{
	return 0;
}

char* refreshRoomService(AccountData* playerdata, int roomid) // have to check if id is not out of range of max possible id to don't check out of table range data
{
	int i;
	bool foundhim = 0;
	static char loginlist[MAX_PLAYER_COUNT][MAX_LOGIN_LENGTH];

	for (i = 0; i < MAX_PLAYER_COUNT; ++i)
	{
		strcpy(loginlist[i], rooms[roomid]->players[i]->login);

		if(rooms[roomid]->players[i]->currentip == playerdata->currentip)
		{
			foundhim = 1;
			break;
		}
	}

	if (!foundhim)		// if you couldnt find the player then its a trap - dont give him anything
		return NULL;

	for (; i < MAX_PLAYER_COUNT; ++i)	// continue copying
		strcpy(loginlist[i], rooms[roomid]->players[i]->login);

	return 0;
}

int sweepPlayer(AccountData* playerdata)
{
	int i, j;

    for(i = 0; i < MAX_ROOM_COUNT; ++i)
    {
        for(j = 0; j < MAX_PLAYER_COUNT; ++j)
        {
            if(rooms[i]->players[j] == playerdata)
            {
				rooms[i]->players[j] = NULL;
				rooms[i]->isplayerready[j] = 0;
				return 0;
            }
        }
    }

	return PLAYER_NOT_FOUND;
}
