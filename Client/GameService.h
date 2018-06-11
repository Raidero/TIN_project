#ifndef GAMESERVICE_H
#define GAMESERVICE_H
#include "Client.h"

typedef struct
{
    float x;
    float y;
    int hp;
} player;

extern player* players[MAX_PLAYER_COUNT];

void initPlayers(int numberofplayers);

#endif // GAMESERVICE_H
