#include "RoomService.h"
#include <stdio.h>
#include <string.h>
int findFreeRoomForAccount(Room* rooms, int numberofrooms, char* ip)
{
    int i;
    if(rooms == NULL)
    {
        //printf("There are no free rooms\n");
        return FREE_ROOM_NOT_FOUND;
    }
    for(i = 0; i < numberofrooms; ++i)
    {
        if(rooms[i].currentsize < rooms[i].capacity)
        {
            connectAccountToRoom(&rooms[i], ip);
            return 0;
        }
    }
    return FREE_ROOM_NOT_FOUND;
}

void connectAccountToRoom(Room* room, char* ip)
{
//    room->playersips[room->currentsize++] = ip;
}

void createRoomForAccount(Room* rooms, int numberofrooms, char* ip)
{

}
