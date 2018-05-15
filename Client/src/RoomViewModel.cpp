#include "RoomViewModel.h"
#define RADIUS 10

RoomViewModel::RoomViewModel(ViewModel* mvm)
{
    this->mvm = mvm;
    initButtons(3);
    logins.setPosition(10+MARGIN+RADIUS, 10);
    exit.setPosition(WIDTH, 10);
    for(int i = 0; i < MAX_PLAYER_COUNT; ++i)
    {
        ready[i].setRadius(RADIUS/2);
        ready[i].setPosition(10, (float)((RADIUS+MARGIN)*i));
        ready[i].setFillColor(sf::Color(255,0,0));
        ready[i].setOutlineColor(sf::Color(255,0,0));
    }
}

RoomViewModel::~RoomViewModel()
{
    //dtor
}
