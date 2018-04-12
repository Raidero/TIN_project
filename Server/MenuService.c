#include "MenuService.h"
#include <stdio.h>
#include <string.h>
int connectAccountToRoomService(Room* rooms, int numberofrooms, char* ip)
{
    if(!isLoggedIn(ip))
    {
        fprintf(stderr, "Player is not logged in\n");
        return LOGGED_IN_ERROR;
    }
    if(findFreeRoomForAccount(rooms, numberofrooms, ip))
    {
        createRoomForAccount(rooms, numberofrooms, ip);

    }
    //TODO
    return 0;
}

void voteForHostService(Room* room, char* login)
{
    int i;
    char* ip = loginToIp(room->players, room->currentsize, login);
    if(ip == NULL)
    {
        fprintf(stderr, "Failed to find player with given ip: %s", ip);
    }
    for(i = 0; i < room->currentsize; ++i)
    {
        if(strcmp(room->players[i].currentip, ip))
        {
            ++room->players[i].votercounter;
        }
    }
    fprintf(stderr, "Failed to find player with given ip: %s", ip);
}




