#include "MenuService.h"
#include <stdio.h>
#include <string.h>


int connectAccountToRoomService(AccountData* account)
{
    if(!isLoggedIn(account->currentip))
    {
        fprintf(stderr, "Player is not logged in\n");
        return LOGGED_IN_ERROR;
    }
    if(findFreeRoomForAccount(account))
    {
        createRoomForAccount(account);
    }
    //TODO
    return 0;
}

int setReadyToStartService(uint32_t ip, unsigned short roomid)
{
    if(roomid >= MAX_ROOM_COUNT)
    {
        fprintf(stderr, "Specified roomid is out of range");
        return OUT_OF_RANGE;
    }
    int i;
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

int voteForHostService(char* login, unsigned short roomid)
{
    if(roomid >= MAX_ROOM_COUNT)
    {
        fprintf(stderr, "Specified roomid is out of range");
        return OUT_OF_RANGE;
    }
    int i;
    uint32_t ip = loginToIp(rooms[roomid]->players, login);
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








