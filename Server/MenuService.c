#include "MenuService.h"

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

void voteForHostService(Room* room, AccountData* loggedaccounts, int numberofaccouns, char* login)
{
    int i;
    char* ip = loginToIp(loggedaccounts, numberofaccouns, login);
    if(ip == NULL)
    {
        //TODO
    }
    for(i = 0; i < room->currentsize; ++i)
    {
        //if(room->playersips[i] == ip)
        //{
            //TODO
        //}
    }
}




