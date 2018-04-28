#include "MenuService.h"
#include <stdio.h>
#include <string.h>


int connectAccountToRoomService(AccountData* account)
{
    int roomid = 0;
    if(!isLoggedIn(account->currentip))
    {
        fprintf(stderr, "Player is not logged in\n");
        return LOGGED_IN_ERROR;
    }
    roomid = findFreeRoomForAccount(account);
    if(roomid < 0)
    {
        roomid = createRoomForAccount(account);
        if(roomid < 0)
        {
            fprintf(stderr, "Couldn't connect account to any room\n");
            return roomid;
        }
    }
    return roomid;
}

int setReadyToStartService(uint32_t ip, int roomid)
{
    int i;
    if(roomid < 0 && roomid >= MAX_ROOM_COUNT)
    {
        fprintf(stderr, "Specified roomid is out of range: %d", roomid);
        return OUT_OF_RANGE;
    }
    for(i = 0; i < MAX_PLAYER_COUNT; ++i)
    {
        if(rooms[roomid]->players[i] != NULL && rooms[roomid]->players[i]->currentip == ip)
        {
            rooms[roomid]->isplayerready[i] = 1; //1 == TRUE
            return 0;
        }
    }
    return IP_NOT_FOUND;
}

int voteForHostService(char* login, int roomid)
{
    if(roomid < 0 && roomid >= MAX_ROOM_COUNT)
    {
        fprintf(stderr, "Specified roomid is out of range: %d", roomid);
        return OUT_OF_RANGE;
    }
    int i;
    uint32_t ip = loginToIp(rooms[roomid]->players, login);
    if(ip < 0)
    {
        fprintf(stderr, "Given ip is not correct: %d\n", ip);
        return PLAYER_NOT_FOUND;
    }
    for(i = 0; i < MAX_PLAYER_COUNT; ++i)
    {
        if(rooms[roomid]->players[i] != NULL && rooms[roomid]->players[i]->currentip == ip)
        {
            ++rooms[roomid]->players[i]->votercounter;
        }
    }
    fprintf(stderr, "Failed to find player with given ip: %d", ip);
    return PLAYER_NOT_FOUND;
}








