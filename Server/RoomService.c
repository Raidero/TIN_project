#include "RoomService.h"
#include <stdio.h>
#include <string.h>

extern AccountData* loggedaccounts[MAX_ACCOUNTS_COUNT];

Room* rooms[MAX_ROOM_COUNT];

Room* initRoom()
{
    int i;
    Room* room = (Room*)malloc(sizeof(Room));
    room->isingame = 0;
    for(i = 0; i < MAX_PLAYER_COUNT; ++i)
    {
        room->players[i] = NULL;
        room->isplayerready[i] = 0;
    }
    return room;
}

void disposeRoom(int roomid)
{
    if(roomid >= 0 && roomid < MAX_ROOM_COUNT && rooms[roomid] != NULL)
    {
        free(rooms[roomid]);
        rooms[roomid] = NULL;
    }
}

void checkIfRoomIsEmptyAndDispose(int roomid)
{
    bool empty = 1;
    int i;
    if(roomid < 0 || roomid >= MAX_ROOM_COUNT || rooms[roomid] == NULL)
	{
        fprintf(stderr, "Room ID out of range: %d\n", roomid);
        return;
	}
    for(i = 0; i < MAX_PLAYER_COUNT; ++i)
    {
        if(rooms[roomid]->players[i] != NULL)
        {
            empty = 0;
        }
    }
    if(empty)
    {
        disposeRoom(roomid);
    }
}

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
    room->players[index] = playerdata;
    room->isplayerready[index] = 0;
	return 0;
}

int createRoomForAccount(AccountData* playerdata)
{
    int i;
    for(i = 0; i < MAX_ROOM_COUNT; ++i)
    {
        if(rooms[i] == NULL)
        {
            rooms[i] = initRoom();
            connectAccountToRoom(rooms[i], playerdata, 0);
            return i;
        }
    }
	return MAX_ROOM_LIMIT_ERROR;
}

char* refreshRoomService(AccountData* playerdata, int roomid)
{
	int i;
	bool foundhim = 0;
	static char loginlist[MAX_PLAYER_COUNT][MAX_LOGIN_LENGTH];
	if(roomid < 0 || roomid >= MAX_ROOM_COUNT || rooms[roomid] == NULL)
	{
        fprintf(stderr, "Room ID out of range: %d\n", roomid);
        return NULL;
	}
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

int sweepPlayer(AccountData* playerdata, int roomid)
{
    int i;
    if(roomid < 0 || roomid >= MAX_ROOM_COUNT || rooms[roomid] == NULL)
	{
        fprintf(stderr, "Room ID out of range: %d\n", roomid);
        return OUT_OF_RANGE;
	}

    for(i = 0; i < MAX_PLAYER_COUNT; ++i)
    {
        if(rooms[roomid]->players[i] != NULL && rooms[roomid]->players[i]->currentip == playerdata->currentip)
        {
            rooms[roomid]->players[i] = NULL;
            rooms[roomid]->isplayerready[i] = 0;
            return 0;
        }
    }
	return PLAYER_NOT_FOUND;
}


