#include "MenuService.h"
#include <stdio.h>
#include <string.h>


int connectAccountToRoomService(int accountid)
{
    int roomid = 0;
    if(accountid >= 0 && accountid < MAX_ACCOUNTS_COUNT && loggedaccounts[accountid] != NULL)
    {
        roomid = findFreeRoomForAccount(accountid);
        if(roomid < 0)
        {
            roomid = createRoomForAccount(accountid);
            if(roomid < 0)
            {
                fprintf(stderr, "Couldn't connect account to any room\n");
                return roomid;
            }
        }
        return roomid;
    }
    return OUT_OF_RANGE;
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






