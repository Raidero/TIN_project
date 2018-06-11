#include "RoomService.h"
#include <stdio.h>
#include <string.h>
#include "Serialization.h"
#include "CommunicationService.h"

AccountData* loggedaccounts[MAX_ACCOUNTS_COUNT];
int communicationsockets[MAX_SOCKETS_COUNT];
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
        room->isplayerready[i] = 2;
        room->hostindex = MAX_PLAYER_COUNT;
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
        if(rooms[roomid] != NULL && rooms[roomid]->players[i] != NULL)
        {
            empty = 0;
        }
    }
    if(empty)
    {
        disposeRoom(roomid);
    }
}

int findFreeRoomForAccount(int accountid)
{
    int i, j;
    if(accountid >= 0 && accountid < MAX_ACCOUNTS_COUNT && loggedaccounts[accountid] != NULL)
    {
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
                        connectAccountToRoom(rooms[i], accountid, j);
                        return i;
                    }
                }
            }
        }
        return FREE_ROOM_NOT_FOUND;
    }
    return OUT_OF_RANGE;
}

int connectAccountToRoom(Room* room, int accountid, int index)
{
    room->players[index] = loggedaccounts[accountid];
    room->isplayerready[index] = 0;
	return 0;
}

int createRoomForAccount(int accountid)
{
    int i;
    if(accountid >= 0 && accountid < MAX_ACCOUNTS_COUNT && loggedaccounts[accountid] != NULL)
    {
        for(i = 0; i < MAX_ROOM_COUNT; ++i)
        {
            if(rooms[i] == NULL)
            {
                rooms[i] = initRoom();
                connectAccountToRoom(rooms[i], accountid, 0);
                rooms[i]->hostindex  = 0;
                return i;
            }
        }
        return MAX_ROOM_LIMIT_ERROR;
    }
    return OUT_OF_RANGE;
}

int refreshRoomService(int accountid, int roomid, char* loginlist)
{
	int i;
	bool foundhim = 0;
    if(accountid >= 0 && accountid < MAX_ACCOUNTS_COUNT && loggedaccounts[accountid] != NULL)
    {
        bzero(loginlist, MAX_LOGIN_LENGTH*MAX_PLAYER_COUNT*sizeof(char));

        if(roomid < 0 || roomid >= MAX_ROOM_COUNT || rooms[roomid] == NULL)
        {
            fprintf(stderr, "Room ID out of range: %d\n", roomid);
            return OUT_OF_RANGE;
        }
        for (i = 0; i < MAX_PLAYER_COUNT; ++i)
        {
            if (rooms[roomid]->players[i] == NULL)	// taking care of not existing players in room
                continue;

            strcpy((loginlist+i*MAX_LOGIN_LENGTH*sizeof(char)), rooms[roomid]->players[i]->login);

            if(rooms[roomid]->players[i]->currentip == loggedaccounts[accountid]->currentip)
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
            strcpy((loginlist+i*MAX_LOGIN_LENGTH*sizeof(char)), rooms[roomid]->players[i]->login);
        }
        return 0;
    }
    return OUT_OF_RANGE;
}

char* refreshReadyUpService(int accountid, int roomid)
{
	int i;
	bool foundhim = 0;
    if(accountid >= 0 && accountid < MAX_ACCOUNTS_COUNT && loggedaccounts[accountid] != NULL)
    {
        if(roomid < 0 || roomid >= MAX_ROOM_COUNT || rooms[roomid] == NULL)
        {
            fprintf(stderr, "Room ID out of range: %d\n", roomid);
            return NULL;
        }
        for (i = 0; i < MAX_PLAYER_COUNT; ++i)
        {
            if (rooms[roomid]->players[i] == NULL)	// taking care of not existing players in room
                continue;

            if(rooms[roomid]->players[i]->currentip == loggedaccounts[accountid]->currentip)
            {
                foundhim = 1;
                break;
            }
        }

        if (!foundhim)		// gotta check it if we don't want to give info about rooms to other players
            return NULL;

        return rooms[roomid]->isplayerready;
    }
    return NULL;
}

int toggleReadyService(int accountid, int roomid)
{
    int i;

    if(roomid < 0 || roomid >= MAX_ROOM_COUNT || rooms[roomid] == NULL || accountid < 0 || accountid >= MAX_ACCOUNTS_COUNT || loggedaccounts[accountid] == NULL)
    {
        fprintf(stderr, "Room ID out of range: %d\n", roomid);
        return OUT_OF_RANGE;
    }

    for (i = 0; i < MAX_PLAYER_COUNT; ++i)
    {
        if (rooms[roomid]->players[i] == NULL)	// taking care of not existing players in room
            continue;

        if(rooms[roomid]->players[i]->currentip == loggedaccounts[accountid]->currentip &&
        !strcmp(rooms[roomid]->players[i]->login, loggedaccounts[accountid]->login))
        {
            if (rooms[roomid]->isplayerready[i] == 1)
                rooms[roomid]->isplayerready[i] = 0;
            else
                rooms[roomid]->isplayerready[i] = 1;
            return 0;
        }
    }

    return PLAYER_NOT_FOUND;
}

int exitRoomService(int accountid, int* roomid)
{
    int result = sweepPlayer(accountid, *roomid);
    checkIfRoomIsEmptyAndDispose(*roomid);
    close(communicationsockets[accountid]);
    communicationsockets[accountid] = -1;
    *roomid = -1;
    return result;
}

int sweepPlayer(int accountid, int roomid)
{
    int i, j;

    if(roomid < 0 || roomid >= MAX_ROOM_COUNT || rooms[roomid] == NULL ||
    accountid < 0 || accountid >= MAX_ACCOUNTS_COUNT || loggedaccounts[accountid] == NULL)
	{
        fprintf(stderr, "Room ID or Account ID out of range: %d, %d\n", roomid, accountid);
        return OUT_OF_RANGE;
	}

    for(i = 0; i < MAX_PLAYER_COUNT; ++i)
    {
        if(rooms[roomid]->players[i] != NULL && rooms[roomid]->players[i]->currentip == loggedaccounts[accountid]->currentip &&
        !strcmp(rooms[roomid]->players[i]->login, loggedaccounts[accountid]->login))
        {
            rooms[roomid]->players[i] = NULL;
            rooms[roomid]->isplayerready[i] = 2;
            for (j = 0; j < MAX_PLAYER_COUNT; ++j)
            {
				if (rooms[roomid]->players[j] != NULL)
				{
					rooms[roomid]->hostindex = j;
				}
            }
            return 0;
        }
    }
	return PLAYER_NOT_FOUND;
}

void defragmentRoom(int roomid)
{
    int i, lastfree = 0;
    for(i = 0; i < MAX_PLAYER_COUNT; ++i)
    {
        if(rooms[roomid]->players[i] != NULL)
        {
            if(i == lastfree)
            {
                lastfree++;
            }
            else
            {
                rooms[roomid]->players[lastfree++] = rooms[roomid]->players[i];
                rooms[roomid]->players[i] = NULL;
            }
        }
    }
}

int startMatchService (int roomid, int accountid)
{
    int i;
    int nplayers = 0;
	// do we really need those checks?
    if(roomid < 0 || roomid >= MAX_ROOM_COUNT || rooms[roomid] == NULL)
    {
        fprintf(stderr, "Room ID out of range: %d\n", roomid);
        return OUT_OF_RANGE;
    }

    if(rooms[roomid]->isingame == 1)
    {
        fprintf(stderr, "Room already in game: %d\n", roomid);
        return ROOM_IN_GAME;
    }

    for(i = 0; i < MAX_PLAYER_COUNT; ++i)
    {
        if(rooms[roomid]->isplayerready[i] == 0)
        {
            fprintf(stderr, "Not all players ready in room: %d\n", roomid);
            return PLAYER_NOT_READY;
        }
        else if(rooms[roomid]->players[i] != NULL)
        {
            ++nplayers;
        }
    }
    if(nplayers < 2)
    {
        fprintf(stderr, "There are no enough players in room to start game\n");
        return NOT_ENOUGH_PLAYERS;
    }
	if (strcmp(loggedaccounts[accountid]->login, rooms[roomid]->players[rooms[roomid]->hostindex]->login) != 0)
	{
		fprintf(stderr, "Match start requested by not a host\n");
		return IS_NOT_HOST;
	}

    if (startMulticasting(roomid) < 0)	// this one is important
    {
        fprintf(stderr, "Could not prepare new multicast group\n");
        return MULTICAST_INIT_ERROR;
    }

    rooms[roomid]->isingame = 1;
    rooms[roomid]->numberofplayersingame = nplayers;
    defragmentRoom(roomid);

    char* message = (char*)malloc(FINAL_MESSAGE_LENGTH*sizeof(char));
    message[0] = SERVER_REQUESTS_TO_START_GAME; //bell
    message[1] = roomid;
    message[2] = nplayers;
    sendMessageToRoomService(roomid, message);
    free(message);

    prepareStartingInformations(roomid);
    return 0;
}

void prepareStartingInformations(int roomid)
{
    int x, y;
    int i;

    srand(time(NULL));

    for(i = 0, x = 100; x < WIDTH - 100; x += 100)
    {
        for(y = 100; y < HEIGHT - 100; y += 100)
        {
            rooms[roomid]->playersinfo[i].x = x + (rand() % 200) - 100;
            rooms[roomid]->playersinfo[i].y = y + (rand() % 200) - 100;
            rooms[roomid]->playersinfo[i++].hp = 100;
            if(i >= rooms[roomid]->numberofplayersingame)
            {
                break;
            }
        }
    }
}

unsigned char* getPlayersInformation(int roomid, int accountid, unsigned char* playersinfobuffer, int* bytessaved)
{
    int i;
    if(playersinfobuffer == NULL)
    {
        playersinfobuffer = (unsigned char*)malloc(rooms[roomid]->numberofplayersingame*sizeof(PlayerData) + sizeof(int)) ;
    }
    PlayerData* playersinfo = (PlayerData*)malloc(rooms[roomid]->numberofplayersingame*sizeof(PlayerData));

    for(i = 0; i < rooms[roomid]->numberofplayersingame; ++i)
    {
        playersinfo[i] = rooms[roomid]->playersinfo[i];
    }
    unsigned char* ptr = playersinfobuffer;
    for(i = 0; i < rooms[roomid]->numberofplayersingame; ++i)
    {
        ptr = serializePlayerData(ptr, &playersinfo[i]);
    }
    for(i = 0; i < rooms[roomid]->numberofplayersingame; ++i)
    {
        if(!strcmp(rooms[roomid]->players[i]->login, loggedaccounts[accountid]->login))
        {
            ptr = serializeInt(ptr, i);
        }
    }
    *bytessaved = ptr - playersinfobuffer;
    return playersinfobuffer;
}


