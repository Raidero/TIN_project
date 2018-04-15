#ifndef ROOM_SERVICE_H
#define ROOM_SERVICE_H

#include "AccountService.h"
#include "Defines.h"



typedef struct
{
    //short capacity;
    //short currentsize; //why? because we can just check if players[i] is null, or if not, that will tell us how many player exists, < MAX_PLAYER_COUNT
    AccountData* players[MAX_PLAYER_COUNT];
    bool isplayerready[MAX_PLAYER_COUNT]; //so that players can say "i'm ready to start playing game"
} Room;

extern Room* rooms[MAX_ROOM_COUNT];

int findFreeRoomForAccount(AccountData* playerdata);
int connectAccountToRoom(Room* room, AccountData* playerdata, uint16_t index);
int createRoomForAccount(AccountData* playerdata);
//create function that check if all the players are still in the room
#endif // ROOM_SERVICE_H
