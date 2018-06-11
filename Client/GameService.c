#include "GameService.h"

player* players[MAX_PLAYER_COUNT];

void initPlayers(int numberofplayers)
{
    int i;
    for(i = 0; i < numberofplayers; ++i)
    {
        players[i] = (player*)malloc(sizeof(player));
    }
}
