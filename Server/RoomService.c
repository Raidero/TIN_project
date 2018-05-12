#include "RoomService.h"
#include <stdio.h>
#include <string.h>

extern AccountData* loggedaccounts[MAX_ACCOUNTS_COUNT];

Room* rooms[MAX_ROOM_COUNT];

void initRoomService()
{
    int i;
    for(i = 0; i < MAX_ROOM_COUNT; ++i)
    {
        rooms[i] = NULL;
    }
}

Room* initRoom()
{
    int i;
    Room* room = (Room*)malloc(sizeof(Room));
    if (room == NULL)
    {
		fprintf(stderr, "malloc() failed");
		return NULL;
	}

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

void disposeAllRooms()
{
	int i;
	for(i = 0; i < MAX_ROOM_COUNT; ++i)
	{
		free(rooms[i]);
        rooms[i] = NULL;
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
			if(rooms[i] == NULL)
			{
				break;	// room doesn't exist yet
			}

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

int refreshRoomService(AccountData* playerdata, int roomid, char** loginlist)
{
	int i;
	bool foundhim = 0;

	for (i = 0; i < MAX_PLAYER_COUNT; ++i)
		bzero(*(loginlist+i*sizeof(char)), MAX_LOGIN_LENGTH*sizeof(char));

	if(roomid < 0 || roomid >= MAX_ROOM_COUNT || rooms[roomid] == NULL)
	{
        fprintf(stderr, "Room ID out of range: %d\n", roomid);
        return OUT_OF_RANGE;
	}
	for (i = 0; i < MAX_PLAYER_COUNT; ++i)
	{
		if (rooms[roomid]->players[i] == NULL)	// taking care of not existing players in room
			continue;

		strcpy(*(loginlist+i*sizeof(char)), rooms[roomid]->players[i]->login);

		if(rooms[roomid]->players[i]->currentip == playerdata->currentip)
		{
			foundhim = 1;
			++i;
			break;
		}
	}

	if (!foundhim)		// gotta check it if we don't want to give info about rooms to other players
		return PLAYER_NOT_FOUND;

	for (; i < MAX_PLAYER_COUNT; ++i)	// continue copying
	{
		if (rooms[roomid]->players[i] == NULL)	// taking care of not existing players in room
			continue;
		strcpy(*(loginlist+i*sizeof(char)), rooms[roomid]->players[i]->login);
	}

	return 0;
}

int exitRoomService(AccountData* playerdata, int roomid)
{
    return sweepPlayer(playerdata, roomid);     // idk if sweepPlayer will be used somewhere else
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


